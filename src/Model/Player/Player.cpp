#include "Model/Player/Player.h"

#include <algorithm>
#include <cmath>

#include "Model/Player/PlayerConstants.h"
#include "World/EntityContext.h"

Player::Player(const sf::Vector2f startPosition)
	: 	Actor(
			startPosition,
			PlayerConstants::COLLISION_SIZE,
			PlayerConstants::MAX_HEALTH,
			VISUAL_HERO_IDLE
		),
		m_speedMultiplier(PlayerConstants::DEFAULT_SPEED_MULTIPLIER),
		m_speedBoostTimer(0.f)
{
}

void Player::PushInput(const PlayerInputEvent& event)
{
	m_stateManager.PushInput(event);
}

void Player::Update(const float dt, EntityContext& context)
{
	UpdateStatusTimers(dt);
	UpdateSpeedBoost(dt);
	environment.gravity = context.gravity;

	const PlayerStateActions actions = UpdateVelocity(dt);
	if (actions.jumpStarted)
	{
		context.events.push_back({ EVENT_PLAYER_JUMPED, 0 });
	}
	if (actions.healStarted && Heal(PlayerConstants::BAIKAL_WATER_HEAL_AMOUNT))
	{
		context.events.push_back({ EVENT_PLAYER_HEALED, PlayerConstants::BAIKAL_WATER_HEAL_AMOUNT });
	}

	ApplyPhysicsStep(dt, context);
	m_stateManager.ApplyPhysicsResult(environment.isGrounded, velocity.y);
	UpdateVisualState();
}

bool Player::IsPendingRemoval() const
{
	return false;
}

void Player::ApplySpeedBoost(const float multiplier, const float duration)
{
	if (multiplier <= 0.f || duration <= 0.f)
	{
		return;
	}

	m_speedMultiplier = multiplier;
	m_speedBoostTimer = duration;
}

float Player::GetHealCooldown() const
{
	return m_stateManager.GetState().healCooldown;
}

float Player::GetDashCooldown() const
{
	return m_stateManager.GetState().dashCooldown;
}

void Player::OnHorizontalCollision(const float surfaceNormalX)
{
	if (m_stateManager.GetState().IsDashing())
	{
		StopDash();
		return;
	}

	Actor::OnHorizontalCollision(surfaceNormalX);
}

PlayerStateActions Player::UpdateVelocity(const float dt)
{
	const bool wasGrounded = m_stateManager.GetState().IsGrounded();
	const bool canHeal = IsAlive() && health < maxHealth;

	const PlayerStateActions actions = m_stateManager.ProcessInputQueue(
		environment.movementModifiers.allowsJump,
		environment.movementModifiers.allowsDash,
		canHeal
	);
	const PlayerState& state = m_stateManager.GetState();

	if (state.IsDashing())
	{
		velocity.x = state.dashDirection * PlayerConstants::DASH_SPEED;
		velocity.y = 0.f;
	}
	else
	{
		ApplyHorizontalMovement(
			state.MoveDirection(),
			environment.movementModifiers.movementSpeedScale * m_speedMultiplier,
			PlayerConstants::MAX_SPEED,
			PlayerConstants::ACCELERATION,
			environment.groundFriction,
			wasGrounded,
			dt
		);
		ApplyGravity(
			environment.gravity,
			environment.movementModifiers.gravityScale,
			PlayerConstants::JUMP_RELEASE_GRAVITY_MULTIPLIER,
			state.jumpHeld,
			wasGrounded,
			dt
		);

		if (actions.jumpStarted)
		{
			velocity.y = PlayerConstants::JUMP_SPEED;
		}
	}

	UpdateFacingFromVelocity();

	const bool wasDashing = state.IsDashing();
	m_stateManager.UpdateTimers(dt, actions.dashStarted);

	if (wasDashing && !state.IsDashing())
	{
		velocity.x = 0.f;
	}

	return actions;
}

void Player::UpdateSpeedBoost(const float dt)
{
	if (m_speedBoostTimer <= 0.f)
	{
		return;
	}

	m_speedBoostTimer = std::max(0.f, m_speedBoostTimer - dt);
	if (m_speedBoostTimer <= 0.f)
	{
		m_speedMultiplier = PlayerConstants::DEFAULT_SPEED_MULTIPLIER;
	}
}

void Player::StopDash()
{
	m_stateManager.StopDash();
	velocity.x = 0.f;
}

void Player::UpdateVisualState()
{
	if (m_stateManager.GetState().IsGrounded())
	{
		visualId = std::abs(velocity.x) > PlayerConstants::RUN_SPEED_THRESHOLD
			? VISUAL_HERO_RUN
			: VISUAL_HERO_IDLE;
	}
	else
	{
		visualId = velocity.y < -PlayerConstants::RISE_SPEED_THRESHOLD
			? VISUAL_HERO_JUMP
			: VISUAL_HERO_FALL;
	}
}
