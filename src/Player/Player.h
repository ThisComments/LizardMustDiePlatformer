#pragma once

#include <SFML/Graphics.hpp>
#include "PlayerStateManager.h"
#include "EnvironmentState.h"

class Player
{
private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_position;
	sf::Vector2f m_collisionSize;
	sf::Vector2f m_velocity;
	PlayerStateManager m_stateManager;

	void Move(
		float moveDirection,
		float movementSpeedScale,
		float friction,
		bool isGrounded,
		float dt
	);
	void UpdateDirection();
	void ApplyGravity(
		float gravity,
		float gravityScale,
		bool isJumpHeld,
		bool isGrounded,
		float dt
	);

public:
	Player();

	void PushInput(const PlayerInputEvent& event);
	void Update(EnvironmentState& environmentState, float dt);
	void Draw(sf::RenderTarget& window) const;

	sf::FloatRect GetBounds() const;
	sf::Vector2f GetVelocity() const;
	sf::Vector2f GetPosition() const;
	bool IsGrounded() const;
	bool IsDashing() const;

	void MoveBy(sf::Vector2f offset);
	void ResolveHorizontalCollision(float surfaceNormalX);
	void ResolveVerticalCollision(float surfaceNormalY);
	void ApplyEnvironmentState(const EnvironmentState& environmentState);
	void SetVelocity(sf::Vector2f newVelocity);
	void SetPosition(sf::Vector2f newPosition);
	void StopDash();
};
