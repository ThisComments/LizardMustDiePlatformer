#include "Player.h"

#include <algorithm>
#include <cmath>

namespace
{
	const float JUMP_RELEASE_GRAVITY_MULTIPLIER = 2.f;
	const float DASH_SPEED = 1000.f;
	const float DASH_DURATION = 0.15f;
	const float DASH_COOLDOWN = 2.f;
	const float MAX_SPEED = 300.f;
	const float JUMP_SPEED = -600.f;
	const float ACCELERATION = 1800.f;
	const float MAX_FALL_SPEED = 1500.f;
	const float SPRITE_HEIGHT = 100.f;
	const float DIRECTION_SPEED_THRESHOLD = 10.f;
}

Player::Player()
	: m_texture("./assets/hero.png"),
	  m_sprite(m_texture),
	  m_velocity({0.f, 0.f}),
	  m_facingDirection(1.f),
	  m_isGrounded(false),
	  m_canAirJump(false),
	  m_isDashing(false),
	  m_dashTimer(0.f),
	  m_dashDirection(0.f),
	  m_canAirDash(false),
	  m_dashCooldown(0.f)
{
	const sf::Vector2u textureSize = m_texture.getSize();
	if (textureSize.y > 0)
	{
		const float scale = SPRITE_HEIGHT / static_cast<float>(textureSize.y);
		m_sprite.setScale({scale, scale});
	}

	const sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
	m_sprite.setPosition({300.f, 250.f});
}

void Player::Update(PlayerInput& input, EnvironmentState& environmentState, const float dt)
{
	SetIsGrounded(environmentState.isGrounded);

	if (m_isDashing)
	{
		UpdateDash(dt);
		return;
	}

	StartDash(input.dash, environmentState.movementModifiers.allowsDash);
	if (m_isDashing)
	{
		UpdateDirection();
		return;
	}

	Move(
		input,
		environmentState.movementModifiers.movementSpeedScale,
		environmentState.groundFriction,
		dt
	);
	ApplyGravity(
		environmentState.gravity,
		environmentState.movementModifiers.gravityScale,
		input.jumpHeld,
		dt
	);
	Jump(input.jump, environmentState.movementModifiers.allowsJump);
	UpdateDirection();

	if (m_dashCooldown > 0.f)
	{
		m_dashCooldown = std::max(0.f, m_dashCooldown - dt);
	}
}

void Player::Draw(sf::RenderTarget& window) const
{
	window.draw(m_sprite);
}

void Player::Jump(bool& isJump, const bool allowsJump)
{
	if (!allowsJump)
	{
		return;
	}

	if (isJump && m_isGrounded)
	{
		m_velocity.y = JUMP_SPEED;
		m_isGrounded = false;
		m_canAirJump = true;
		isJump = false;
	}
	else if (isJump && !m_isGrounded && m_canAirJump)
	{
		m_velocity.y = JUMP_SPEED;
		m_canAirJump = false;
		isJump = false;
	}
}

void Player::ApplyGravity(
	const float gravity,
	const float gravityScale,
	const bool isJumpHeld,
	const float dt
)
{
	if (m_isGrounded)
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

void Player::StartDash(bool& isDash, const bool allowsDash)
{
	if (!allowsDash || !isDash || m_isDashing || m_dashCooldown > 0.f ||
		(!m_isGrounded && !m_canAirDash))
	{
		return;
	}

	if (!m_isGrounded)
	{
		m_canAirDash = false;
	}

	m_dashCooldown = DASH_COOLDOWN;
	m_isDashing = true;
	m_dashTimer = DASH_DURATION;
	m_dashDirection = m_facingDirection;
	m_velocity.x = m_dashDirection * DASH_SPEED;
	m_velocity.y = 0.f;
	isDash = false;
}

void Player::UpdateDash(const float dt)
{
	if (!m_isDashing)
	{
		return;
	}

	m_dashTimer -= dt;
	m_dashTimer = std::max(0.f, m_dashTimer - dt);
	m_velocity.x = m_dashDirection * DASH_SPEED;
	m_velocity.y = 0.f;

	if (m_dashTimer <= 0.f)
	{
		m_isDashing = false;
	}
}

void Player::StopDash()
{
	m_isDashing = false;
	m_dashTimer = 0.f;
	m_velocity.x = 0.f;
}

void Player::Move(
	const PlayerInput& input,
	const float movementSpeedScale,
	const float friction,
	const float dt
)
{
	bool isMoving = false;
	float moveDirection = 0.f;

	if (input.moveLeft)
	{
		moveDirection -= 1.f;
	}
	if (input.moveRight)
	{
		moveDirection += 1.f;
	}

	if (moveDirection != 0.f)
	{
		isMoving = true;
		m_velocity.x += moveDirection * ACCELERATION * dt;
		m_facingDirection = moveDirection;
	}

	if (m_isGrounded && !isMoving)
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
	const sf::Vector2f scale = m_sprite.getScale();
	if (m_velocity.x > DIRECTION_SPEED_THRESHOLD)
	{
		m_sprite.setScale({std::abs(scale.x), scale.y});
	}
	else if (m_velocity.x < -DIRECTION_SPEED_THRESHOLD)
	{
		m_sprite.setScale({-std::abs(scale.x), scale.y});
	}
}

sf::FloatRect Player::GetBounds() const
{
	return m_sprite.getGlobalBounds();
}

bool Player::IsGrounded() const
{
	return m_isGrounded;
}

bool Player::IsDashing() const
{
	return m_isDashing;
}

sf::Vector2f Player::GetPosition() const
{
	return m_sprite.getPosition();
}

sf::Vector2f Player::GetVelocity() const
{
	return m_velocity;
}

void Player::MoveBy(const sf::Vector2f offset)
{
	m_sprite.move(offset);
}

void Player::ResolveHorizontalCollision()
{
	if (m_isDashing)
	{
		StopDash();
		return;
	}

	m_velocity.x = 0.f;
}

void Player::ResolveVerticalCollision(const float movementDirection)
{
	if (movementDirection > 0.f && m_velocity.y > 0.f)
	{
		m_velocity.y = 0.f;
	}
	else if (movementDirection < 0.f && m_velocity.y < 0.f)
	{
		m_velocity.y = 0.f;
	}
}

void Player::ApplyEnvironmentState(const EnvironmentState& environmentState)
{
	SetIsGrounded(environmentState.isGrounded);
}

void Player::SetIsGrounded(const bool newIsGrounded)
{
	m_isGrounded = newIsGrounded;
	if (m_isGrounded)
	{
		m_canAirJump = true;
		m_canAirDash = true;
	}
}

void Player::SetVelocity(const sf::Vector2f newVelocity)
{
	m_velocity = newVelocity;
}

void Player::SetPosition(const sf::Vector2f newPosition)
{
	m_sprite.setPosition(newPosition);
}
