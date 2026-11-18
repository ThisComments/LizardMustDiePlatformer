#include "Model/Actor/Actor.h"

#include <algorithm>

#include "World/EntityContext.h"

namespace
{
	constexpr float DEFAULT_INVULNERABILITY_DURATION = 1.0f;
	constexpr float DEFAULT_HEAL_FLASH_DURATION = 0.45f;
	constexpr float MAX_FALL_SPEED = 1500.f;
	constexpr float DIRECTION_SPEED_THRESHOLD = 10.f;
}

Actor::Actor(
	const sf::Vector2f startPosition,
	const sf::Vector2f hitboxSize,
	const int initialMaxHealth,
	const VisualId initialVisualId
)
	: 	Entity(initialVisualId),
		position(startPosition),
		collisionSize(hitboxSize),
		velocity({ 0.f, 0.f }), 
		facingDirection(1.f),
		health(std::max(1, initialMaxHealth)),
		maxHealth(std::max(1, initialMaxHealth)),
		invulnerabilityTimer(0.f),
		healFlashTimer(0.f)
{
}

sf::Vector2f Actor::GetPosition() const
{
	return position;
}

sf::Vector2f Actor::GetVisualPosition() const
{
	return GetFeetPosition();
}

sf::FloatRect Actor::GetBounds() const
{
	return sf::FloatRect(
		{ position.x - collisionSize.x / 2.f, position.y - collisionSize.y / 2.f },
		collisionSize
	);
}

float Actor::GetRenderDepth() const
{
	return GetFeetPosition().y;
}

bool Actor::IsFacingRight() const
{
	return facingDirection >= 0.f;
}

float Actor::GetDamageTimer() const
{
	return invulnerabilityTimer;
}

float Actor::GetHealTimer() const
{
	return healFlashTimer;
}

bool Actor::IsPendingRemoval() const
{
	return !IsAlive();
}

bool Actor::TakeDamage(const int amount)
{
	if (amount <= 0 || !IsAlive() || invulnerabilityTimer > 0.f)
	{
		return false;
	}

	health = std::max(0, health - amount);
	invulnerabilityTimer = DEFAULT_INVULNERABILITY_DURATION;
	return true;
}

bool Actor::Heal(const int amount)
{
	if (amount <= 0 || !IsAlive() || health >= maxHealth)
	{
		return false;
	}

	health = std::min(maxHealth, health + amount);
	healFlashTimer = DEFAULT_HEAL_FLASH_DURATION;
	return true;
}

int Actor::GetHealth() const
{
	return health;
}

int Actor::GetMaxHealth() const
{
	return maxHealth;
}

bool Actor::IsAlive() const
{
	return health > 0;
}

bool Actor::IsInvulnerable() const
{
	return invulnerabilityTimer > 0.f;
}

void Actor::UpdateStatusTimers(const float dt)
{
	if (dt <= 0.f)
	{
		return;
	}

	invulnerabilityTimer = std::max(0.f, invulnerabilityTimer - dt);
	healFlashTimer = std::max(0.f, healFlashTimer - dt);
}

void Actor::ApplyPhysicsStep(const float dt, EntityContext& context)
{
	MoveHorizontally(dt, context);
	const AxisCollisionResult verticalResult = MoveVertically(dt, context);
	const LiquidResult liquidResult = context.liquidSystem.Resolve(GetBounds(), context.map);

	environment = BuildEnvironmentState(verticalResult, liquidResult, context.gravity);

	if (liquidResult.contactDamage > 0 && TakeDamage(liquidResult.contactDamage))
	{
		context.events.push_back({ EVENT_PLAYER_DAMAGED, liquidResult.contactDamage });
	}
}

void Actor::ApplyHorizontalMovement(
	const float moveDirection,
	const float movementSpeedScale,
	const float maxSpeed,
	const float acceleration,
	const float friction,
	const bool isGrounded,
	const float dt
)
{
	const bool isMoving = moveDirection != 0.f;

	if (isMoving)
	{
		velocity.x += moveDirection * acceleration * dt;
	}

	if (isGrounded && !isMoving)
	{
		if (velocity.x > 0.f)
		{
			velocity.x = std::max(0.f, velocity.x - friction * dt);
		}
		else if (velocity.x < 0.f)
		{
			velocity.x = std::min(0.f, velocity.x + friction * dt);
		}
	}

	const float currentMaxSpeed = maxSpeed * movementSpeedScale;
	velocity.x = std::clamp(velocity.x, -currentMaxSpeed, currentMaxSpeed);
}

void Actor::ApplyGravity(
	const float gravity,
	const float gravityScale,
	const float jumpReleaseMultiplier,
	const bool isJumpHeld,
	const bool isGrounded,
	const float dt
)
{
	if (isGrounded)
	{
		return;
	}

	float currentGravity = gravity * gravityScale;
	if (velocity.y < 0.f && !isJumpHeld)
	{
		currentGravity *= jumpReleaseMultiplier;
	}

	velocity.y += currentGravity * dt;
	velocity.y = std::min(velocity.y, MAX_FALL_SPEED);
}

void Actor::UpdateFacingFromVelocity()
{
	if (velocity.x > DIRECTION_SPEED_THRESHOLD)
	{
		facingDirection = 1.f;
	}
	else if (velocity.x < -DIRECTION_SPEED_THRESHOLD)
	{
		facingDirection = -1.f;
	}
}

sf::Vector2f Actor::GetFeetPosition() const
{
	return { position.x, position.y + collisionSize.y / 2.f };
}

void Actor::OnHorizontalCollision(const float surfaceNormalX)
{
	if (velocity.x * surfaceNormalX < 0.f)
	{
		velocity.x = 0.f;
	}
}

void Actor::OnVerticalCollision(const float surfaceNormalY)
{
	if (velocity.y * surfaceNormalY < 0.f)
	{
		velocity.y = 0.f;
	}
}

void Actor::MoveHorizontally(const float dt, const EntityContext& context)
{
	const AxisCollisionResult result = context.collisionSystem.ResolveHorizontal(
		GetBounds(),
		context.map,
		velocity.x * dt
	);
	position.x += result.allowedMovement;
	if (result.collided)
	{
		OnHorizontalCollision(result.normal);
	}
}

AxisCollisionResult Actor::MoveVertically(const float dt, const EntityContext& context)
{
	const AxisCollisionResult result = context.collisionSystem.ResolveVertical(
		GetBounds(),
		context.map,
		velocity.y * dt
	);
	position.y += result.allowedMovement;
	if (result.collided)
	{
		OnVerticalCollision(result.normal);
	}
	return result;
}

EnvironmentState Actor::BuildEnvironmentState(
	const AxisCollisionResult& verticalResult,
	const LiquidResult& liquidResult,
	const float gravity
) const
{
	EnvironmentState state;
	state.gravity = gravity;
	state.isGrounded = verticalResult.isGrounded;
	state.groundFriction = verticalResult.groundFriction;
	state.movementModifiers = DEFAULT_MOVEMENT_MODIFIERS;

	if (liquidResult.isInLiquid)
	{
		state.movementModifiers = liquidResult.modifiers;
	}

	return state;
}
