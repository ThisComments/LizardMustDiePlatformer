#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Animation/AnimationComponent.h"
#include "Infrastructure/Input/PlayerInputEvent.h"
#include "Model/Entity/Entity.h"
#include "Model/Player/EnvironmentState.h"
#include "Model/Player/PlayerStateManager/PlayerStateManager.h"
#include "Resources/SpriteSheetAsset.h"

struct AxisCollisionResult;
struct LiquidResult;

// The hero. Owns its movement, its collision with the map and the choice of its animation.
// Input arrives only as PlayerInputEvent; sounds and HUD are driven by the events it reports.
class Player final : public Entity
{
private:
	sf::Vector2f m_position;
	sf::Vector2f m_collisionSize;
	sf::Vector2f m_velocity;
	float m_facingDirection;
	PlayerStateManager m_stateManager;
	// Result of the previous update's physics; drives friction, gravity and jump rules of the next one.
	EnvironmentState m_environment;
	AnimationComponent m_animation;

	// Returns true when a jump actually started.
	bool UpdateVelocity(float dt);
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

	void MoveHorizontally(float dt, const EntityContext& context);
	AxisCollisionResult MoveVertically(float dt, const EntityContext& context);
	void ResolveHorizontalCollision(float surfaceNormalX);
	void ResolveVerticalCollision(float surfaceNormalY);
	void StopDash();

	EnvironmentState BuildEnvironmentState(
		const AxisCollisionResult& verticalResult,
		const LiquidResult& liquidResult,
		float gravity
	) const;

	void UpdateAnimation(float dt);
	sf::Vector2f GetFeetPosition() const;

public:
	Player(sf::Vector2f position, sf::Vector2f collisionSize, const SpriteSheetAsset& sheet);

	void PushInput(const PlayerInputEvent& event);

	void Update(float dt, EntityContext& context) override;
	void Draw(sf::RenderTarget& target) const override;

	sf::Vector2f GetPosition() const override;
	sf::FloatRect GetBounds() const override;
	RenderLayer GetRenderLayer() const override;
	float GetRenderDepth() const override;
	bool IsPendingRemoval() const override;
};
