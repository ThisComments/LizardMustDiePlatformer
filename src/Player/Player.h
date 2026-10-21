#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Player/EnvironmentState.h"
#include "Player/PlayerStateManager.h"

class Player
{
private:
	sf::Vector2f m_position;
	sf::Vector2f m_collisionSize;
	sf::Vector2f m_velocity;
	float m_facingDirection;
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
	Player(sf::Vector2f position, sf::Vector2f collisionSize);

	void PushInput(const PlayerInputEvent& event);
	void Update(const EnvironmentState& environmentState, float dt);

	sf::FloatRect GetBounds() const;
	sf::Vector2f GetVelocity() const;
	sf::Vector2f GetPosition() const;
	float GetFacingDirection() const;
	const PlayerState& GetState() const;
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
