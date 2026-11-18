#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Model/Entity/Entity.h"
#include "Model/Player/EnvironmentState.h"

struct AxisCollisionResult;
struct LiquidResult;

class Actor : public Entity
{
private:
	void MoveHorizontally(float dt, const EntityContext& context);
	AxisCollisionResult MoveVertically(float dt, const EntityContext& context);
	EnvironmentState BuildEnvironmentState(
		const AxisCollisionResult& verticalResult,
		const LiquidResult& liquidResult,
		float gravity
	) const;

protected:
	void UpdateStatusTimers(float dt);
	void ApplyPhysicsStep(float dt, EntityContext& context);

	void ApplyHorizontalMovement(
		float moveDirection,
		float movementSpeedScale,
		float maxSpeed,
		float acceleration,
		float friction,
		bool isGrounded,
		float dt
	);

	void ApplyGravity(
		float gravity,
		float gravityScale,
		float jumpReleaseMultiplier,
		bool isJumpHeld,
		bool isGrounded,
		float dt
	);

	void UpdateFacingFromVelocity();
	sf::Vector2f GetFeetPosition() const;

	virtual void OnHorizontalCollision(float surfaceNormalX);
	virtual void OnVerticalCollision(float surfaceNormalY);

	sf::Vector2f position;
	sf::Vector2f collisionSize;
	sf::Vector2f velocity;
	float facingDirection;
	EnvironmentState environment;
	int health;
	int maxHealth;
	float invulnerabilityTimer;
	float healFlashTimer;

public:
	Actor(
		sf::Vector2f startPosition,
		sf::Vector2f hitboxSize,
		int initialMaxHealth,
		VisualId initialVisualId = VISUAL_NONE
	);

	sf::Vector2f GetPosition() const override;
	sf::Vector2f GetVisualPosition() const override;
	sf::FloatRect GetBounds() const override;
	float GetRenderDepth() const override;
	bool IsFacingRight() const override;
	float GetDamageTimer() const override;
	float GetHealTimer() const override;
	bool IsPendingRemoval() const override;

	bool TakeDamage(int amount);
	bool Heal(int amount);

	int GetHealth() const;
	int GetMaxHealth() const;
	bool IsAlive() const;
	bool IsInvulnerable() const;
};
