#pragma once

#include <SFML/Graphics.hpp>

#include "Infrastructure/KeyboardHandler/PlayerInput.h"

struct MovementModifiers
{
	float gravityScale = 1.f;
	float movementSpeedScale = 1.f;
	bool allowsJump = true;
	bool allowsDash = true;
};

struct EnvironmentState
{
	bool isGrounded = false;
	float gravity = 0.f;
	float groundFriction = 0.f;
	MovementModifiers movementModifiers;
};

class Player
{
private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_velocity;
	float m_facingDirection;
	bool m_isGrounded;
	bool m_canAirJump;
	bool m_isDashing;
	float m_dashTimer;
	float m_dashDirection;
	bool m_canAirDash;
	float m_dashCooldown;

	void Move(const PlayerInput& input, float movementSpeedScale, float friction, float dt);
	void UpdateDirection();
	void Jump(bool& isJump, bool allowsJump);
	void ApplyGravity(float gravity, float gravityScale, bool isJumpHeld, float dt);
	void StartDash(bool& isDash, bool allowsDash);
	void UpdateDash(float dt);
	void SetIsGrounded(bool newIsGrounded);

public:
	Player();

	void Update(PlayerInput& input, EnvironmentState& environmentState, float dt);
	void Draw(sf::RenderTarget& window) const;

	sf::FloatRect GetBounds() const;
	sf::Vector2f GetVelocity() const;
	sf::Vector2f GetPosition() const;
	bool IsGrounded() const;
	bool IsDashing() const;

	void MoveBy(sf::Vector2f offset);
	void ResolveHorizontalCollision();
	void ResolveVerticalCollision(float movementDirection);
	void ApplyEnvironmentState(const EnvironmentState& environmentState);
	void SetVelocity(sf::Vector2f newVelocity);
	void SetPosition(sf::Vector2f newPosition);
	void StopDash();
};
