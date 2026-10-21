#include "Player/Player.h"

#include <algorithm>

const float JUMP_RELEASE_GRAVITY_MULTIPLIER = 2.f;
const float DASH_SPEED = 1000.f;
const float MAX_SPEED = 300.f;
const float JUMP_SPEED = -600.f;
const float ACCELERATION = 1800.f;
const float MAX_FALL_SPEED = 1500.f;
const float DIRECTION_SPEED_THRESHOLD = 10.f;

Player::Player(const sf::Vector2f position, const sf::Vector2f collisionSize)
	: m_position(position),
	  m_collisionSize(collisionSize),
	  m_velocity({0.f, 0.f}),
	  m_facingDirection(1.f)
{
}

void Player::PushInput(const PlayerInputEvent& event)
{
	m_stateManager.PushInput(event);
}

void Player::Update(const EnvironmentState& environmentState, const float dt)
{
	const bool wasGrounded = m_stateManager.GetState().IsGrounded();

	const PlayerStateActions actions = m_stateManager.ProcessInputQueue(
		environmentState.movementModifiers.allowsJump,
		environmentState.movementModifiers.allowsDash
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
			environmentState.movementModifiers.movementSpeedScale,
			environmentState.groundFriction,
			wasGrounded,
			dt
		);
		ApplyGravity(
			environmentState.gravity,
			environmentState.movementModifiers.gravityScale,
			state.jumpHeld,
			wasGrounded,
			dt
		);

		if (actions.jumpStarted)
		{
			m_velocity.y = JUMP_SPEED;
		}
	}

	UpdateDirection();

	const bool wasDashing = state.IsDashing();
	m_stateManager.UpdateTimers(dt, actions.dashStarted);

	if (wasDashing && !state.IsDashing())
	{
		m_velocity.x = 0.f;
	}
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

bool Player::IsGrounded() const
{
	return m_stateManager.GetState().IsGrounded();
}

bool Player::IsDashing() const
{
	return m_stateManager.GetState().IsDashing();
}

sf::Vector2f Player::GetPosition() const
{
	return m_position;
}

float Player::GetFacingDirection() const
{
	return m_facingDirection;
}

const PlayerState& Player::GetState() const
{
	return m_stateManager.GetState();
}

sf::Vector2f Player::GetVelocity() const
{
	return m_velocity;
}

void Player::MoveBy(const sf::Vector2f offset)
{
	m_position += offset;
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

void Player::ApplyEnvironmentState(const EnvironmentState& environmentState)
{
	m_stateManager.ApplyPhysicsResult(environmentState.isGrounded, m_velocity.y);
}

void Player::SetVelocity(const sf::Vector2f newVelocity)
{
	m_velocity = newVelocity;
}

void Player::SetPosition(const sf::Vector2f newPosition)
{
	m_position = newPosition;
}
