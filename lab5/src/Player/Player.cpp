#include <algorithm>
#include "Player.h"

const float JUMP_RELEASE_GRAVITY_MULTIPLIER = 2.f;
const float DASH_SPEED = 1000.f;
const float DASH_DURATION = 0.15f;
const float DASH_COOLDOWN = 2.f;

Player::Player() 
    :   m_texture("../assets/hero.png"),
        m_sprite(m_texture),
        m_velocity({0, 0}),
        m_facingDirection(1.f),
        m_acceleration(1800.f),
        m_maxSpeed(300.f),
        m_jumpSpeed(-600.f),
        m_canAirJump(false),
        m_isGrounded(false),
        m_isDashing(false),
        m_dashDirection(0.f),
        m_dashTimer(0.f),
        m_canAirDash(false),
        m_cooldownDash(0)
{
    sf::Vector2u textureSize = m_texture.getSize();
    float scale = 100.f / textureSize.y;
    m_sprite.setScale({scale, scale});

    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin({
        bounds.size.x / 2.f,
        bounds.size.y / 2.f
    });
    m_sprite.setPosition({300.f, 250.f});
}

void Player::Update(PlayerInput& input, const float friction, const float gravity, const float dt)
{
    if (m_isDashing)
	{
		UpdateDash(dt);
        m_sprite.move(m_velocity * dt);
		return;
	}
    StartDash(input.dash);

    Move(input, friction, dt);
    ApplyGravity(gravity, input.jumpHeld, dt);
    Jump(input.jump);
    UpdateDirection();

    if (m_cooldownDash > 0)
    {
        m_cooldownDash -= dt;
    }
    m_sprite.move(m_velocity * dt);
}

void Player::Draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
}

void Player::Jump(bool& isJump)
{
    if (isJump && m_isGrounded)
    {
        m_velocity.y = m_jumpSpeed;

        m_isGrounded = false;
        m_canAirJump = true;

        isJump = false;
    }
    else if (isJump && !m_isGrounded && m_canAirJump)
    {
        m_velocity.y = m_jumpSpeed;
        m_canAirJump = false;
        isJump = false;
    }
}

void Player::ApplyGravity(const float gravity, const bool isJumpHeld, const float dt)
{
    if (m_isGrounded)
    {
        return;
    }

    float currentGravity = gravity;

    if (m_velocity.y < 0.f && !isJumpHeld)
    {
        currentGravity *= JUMP_RELEASE_GRAVITY_MULTIPLIER;
    }

    m_velocity.y += currentGravity * dt;
}

void Player::StartDash(bool& isDash)
{
	if (!isDash || m_isDashing || m_cooldownDash > 0 || (!m_isGrounded && !m_canAirDash))
	{
		return;
	}

    if (!m_isGrounded && m_canAirDash)
    {
        m_canAirDash = false;
    }

    m_cooldownDash = DASH_COOLDOWN;
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

	m_velocity.x = m_dashDirection * DASH_SPEED;
	m_velocity.y = 0.f;

	if (m_dashTimer <= 0.f)
	{
		m_isDashing = false;
		m_velocity.x = 0.f;
	}
}

void Player::StopDash()
{
	m_isDashing = false;
	m_dashTimer = 0.f;
	m_velocity.x = 0.f;
}

void Player::Move(const PlayerInput& input, const float friction, const float dt)
{
    bool isMoving = false;

    if (input.moveRight) {
        m_velocity.x += m_acceleration * dt;
        m_facingDirection = 1.f;
        isMoving = true;

    }
    if (input.moveLeft) {
        m_velocity.x -= m_acceleration * dt;
        m_facingDirection = -1.f;
        isMoving = true;
    }

    if (!isMoving)
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

    m_velocity.x = std::clamp(m_velocity.x, -m_maxSpeed, m_maxSpeed);
}

void Player::UpdateDirection()
{
    sf::Vector2f scale = m_sprite.getScale();
    if (m_velocity.x > 10.f)
    {
        m_sprite.setScale({
            std::abs(scale.x),
            scale.y
        });
    }
    else if (m_velocity.x < -10.f)
    {
        m_sprite.setScale({
            -std::abs(scale.x),
            scale.y
        });
    }
}

sf::FloatRect Player::GetBounds() const
{
    return m_sprite.getGlobalBounds();
}

bool Player::GetIsGrounded() const
{
    return m_isGrounded;
}

sf::Vector2f Player::GetPosition() const
{
    return m_sprite.getPosition();
}

sf::Vector2f Player::GetVelocity() const
{
    return m_velocity;
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