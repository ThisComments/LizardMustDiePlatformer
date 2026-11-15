#include "Model/Player/Player.h"

#include <algorithm>
#include <cmath>

#include "World/EntityContext.h"

namespace
{
	constexpr float JUMP_RELEASE_GRAVITY_MULTIPLIER = 2.f;
	constexpr float DASH_SPEED = 1000.f;
	constexpr float MAX_SPEED = 300.f;
	constexpr float JUMP_SPEED = -650.f;
	constexpr float ACCELERATION = 1800.f;
	constexpr float MAX_FALL_SPEED = 1500.f;
	constexpr float DIRECTION_SPEED_THRESHOLD = 10.f;

	// Speeds above which the hero is shown running / rising instead of standing / falling.
	constexpr float RUN_SPEED_THRESHOLD = 10.f;
	constexpr float RISE_SPEED_THRESHOLD = 1.f;

	constexpr const char* IDLE_CLIP = "idle";
	constexpr const char* RUN_CLIP = "run";
	constexpr const char* JUMP_CLIP = "jump";
	constexpr const char* FALL_CLIP = "fall";

	const SpriteSheetAsset& RequireHeroClips(const SpriteSheetAsset& sheet)
	{
		sheet.RequireClips({IDLE_CLIP, RUN_CLIP, JUMP_CLIP, FALL_CLIP});
		return sheet;
	}
}

Player::Player(
	const sf::Vector2f position,
	const sf::Vector2f collisionSize,
	const SpriteSheetAsset& sheet
)
	: m_position(position),
	  m_collisionSize(collisionSize),
	  m_velocity({0.f, 0.f}),
	  m_facingDirection(1.f),
	  m_animation(RequireHeroClips(sheet), IDLE_CLIP)
{
	m_animation.SetPosition(GetFeetPosition());
}

void Player::PushInput(const PlayerInputEvent& event)
{
	m_stateManager.PushInput(event);
}

void Player::Update(const float dt, EntityContext& context)
{
	m_environment.gravity = context.gravity;

	if (UpdateVelocity(dt))
	{
		context.events.push_back({WorldEventType::PlayerJumped});
	}

	// The order matters: horizontal first, then vertical, then liquid and the grounded state
	// that the next update will use.
	MoveHorizontally(dt, context);
	const AxisCollisionResult verticalResult = MoveVertically(dt, context);
	const LiquidResult liquidResult = context.liquidSystem.Resolve(GetBounds(), context.map);

	m_environment = BuildEnvironmentState(verticalResult, liquidResult, context.gravity);
	m_stateManager.ApplyPhysicsResult(m_environment.isGrounded, m_velocity.y);

	UpdateAnimation(dt);
}

bool Player::UpdateVelocity(const float dt)
{
	bool jumpStarted = false;
	const bool wasGrounded = m_stateManager.GetState().IsGrounded();

	const PlayerStateActions actions = m_stateManager.ProcessInputQueue(
		m_environment.movementModifiers.allowsJump,
		m_environment.movementModifiers.allowsDash
	);
	const PlayerState& state = m_stateManager.GetState();

	if (state.IsDashing())
	{
		m_velocity.x = state.dashDirection * DASH_SPEED;
		m_velocity.y = 0.f;
	}
	else
	{
		Move(
			state.MoveDirection(),
			m_environment.movementModifiers.movementSpeedScale,
			m_environment.groundFriction,
			wasGrounded,
			dt
		);
		ApplyGravity(
			m_environment.gravity,
			m_environment.movementModifiers.gravityScale,
			state.jumpHeld,
			wasGrounded,
			dt
		);

		if (actions.jumpStarted)
		{
			m_velocity.y = JUMP_SPEED;
			jumpStarted = true;
		}
	}

	UpdateDirection();

	const bool wasDashing = state.IsDashing();
	m_stateManager.UpdateTimers(dt, actions.dashStarted);

	if (wasDashing && !state.IsDashing())
	{
		m_velocity.x = 0.f;
	}

	return jumpStarted;
}

void Player::ApplyGravity(
	const float gravity,
	const float gravityScale,
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
	if (m_velocity.y < 0.f && !isJumpHeld)
	{
		currentGravity *= JUMP_RELEASE_GRAVITY_MULTIPLIER;
	}

	m_velocity.y += currentGravity * dt;
	m_velocity.y = std::min(m_velocity.y, MAX_FALL_SPEED);
}

void Player::StopDash()
{
	m_stateManager.StopDash();
	m_velocity.x = 0.f;
}

void Player::Move(
	const float moveDirection,
	const float movementSpeedScale,
	const float friction,
	const bool isGrounded,
	const float dt
)
{
	const bool isMoving = moveDirection != 0.f;

	if (isMoving)
	{
		m_velocity.x += moveDirection * ACCELERATION * dt;
	}

	if (isGrounded && !isMoving)
	{
		if (m_velocity.x > 0.f)
		{
			m_velocity.x = std::max(0.f, m_velocity.x - friction * dt);
		}
		else if (m_velocity.x < 0.f)
		{
			m_velocity.x = std::min(0.f, m_velocity.x + friction * dt);
		}
	}

	const float currentMaxSpeed = MAX_SPEED * movementSpeedScale;
	m_velocity.x = std::clamp(m_velocity.x, -currentMaxSpeed, currentMaxSpeed);
}

void Player::UpdateDirection()
{
	if (m_velocity.x > DIRECTION_SPEED_THRESHOLD)
	{
		m_facingDirection = 1.f;
	}
	else if (m_velocity.x < -DIRECTION_SPEED_THRESHOLD)
	{
		m_facingDirection = -1.f;
	}
}

sf::FloatRect Player::GetBounds() const
{
	return sf::FloatRect(
		{m_position.x - m_collisionSize.x / 2.f, m_position.y - m_collisionSize.y / 2.f},
		m_collisionSize
	);
}

sf::Vector2f Player::GetPosition() const
{
	return m_position;
}

RenderLayer Player::GetRenderLayer() const
{
	return RenderLayer::Characters;
}

float Player::GetRenderDepth() const
{
	return GetFeetPosition().y;
}

bool Player::IsPendingRemoval() const
{
	// World keeps a non-owning pointer to the player, so it must stay for the whole level.
	return false;
}

void Player::Draw(sf::RenderTarget& target) const
{
	m_animation.Draw(target);
}

sf::Vector2f Player::GetFeetPosition() const
{
	// The sprite may be larger than the hitbox: it is only aligned to it (centred, feet on its bottom).
	return {m_position.x, m_position.y + m_collisionSize.y / 2.f};
}

void Player::MoveHorizontally(const float dt, const EntityContext& context)
{
	const AxisCollisionResult result = context.collisionSystem.ResolveHorizontal(
		GetBounds(),
		context.map,
		m_velocity.x * dt
	);
	m_position.x += result.allowedMovement;
	if (result.collided)
	{
		ResolveHorizontalCollision(result.normal);
	}
}

AxisCollisionResult Player::MoveVertically(const float dt, const EntityContext& context)
{
	const AxisCollisionResult result = context.collisionSystem.ResolveVertical(
		GetBounds(),
		context.map,
		m_velocity.y * dt
	);
	m_position.y += result.allowedMovement;
	if (result.collided)
	{
		ResolveVerticalCollision(result.normal);
	}
	return result;
}

void Player::ResolveHorizontalCollision(const float surfaceNormalX)
{
	if (m_stateManager.GetState().IsDashing())
	{
		StopDash();
		return;
	}

	if (m_velocity.x * surfaceNormalX < 0.f)
	{
		m_velocity.x = 0.f;
	}
}

void Player::ResolveVerticalCollision(const float surfaceNormalY)
{
	if (m_velocity.y * surfaceNormalY < 0.f)
	{
		m_velocity.y = 0.f;
	}
}

EnvironmentState Player::BuildEnvironmentState(
	const AxisCollisionResult& verticalResult,
	const LiquidResult& liquidResult,
	const float gravity
) const
{
	EnvironmentState environmentState;
	environmentState.gravity = gravity;
	environmentState.isGrounded = verticalResult.isGrounded;
	environmentState.groundFriction = verticalResult.groundFriction;
	environmentState.movementModifiers = DEFAULT_MOVEMENT_MODIFIERS;

	if (liquidResult.isInLiquid)
	{
		environmentState.movementModifiers = liquidResult.modifiers;
	}

	return environmentState;
}

void Player::UpdateAnimation(const float dt)
{
	// The animation only mirrors the physical state; it never feeds back into it.
	if (m_stateManager.GetState().IsGrounded())
	{
		m_animation.Play(std::abs(m_velocity.x) > RUN_SPEED_THRESHOLD ? RUN_CLIP : IDLE_CLIP);
	}
	else
	{
		m_animation.Play(m_velocity.y < -RISE_SPEED_THRESHOLD ? JUMP_CLIP : FALL_CLIP);
	}

	m_animation.SetFacingRight(m_facingDirection >= 0.f);
	m_animation.SetPosition(GetFeetPosition());
	m_animation.Update(dt);
}
