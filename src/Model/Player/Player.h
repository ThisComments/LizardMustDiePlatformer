#pragma once

#include <SFML/System/Vector2.hpp>

#include "Infrastructure/Input/PlayerInputEvent.h"
#include "Model/Actor/Actor.h"
#include "Model/Player/PlayerStateManager/PlayerStateManager.h"

class Player final : public Actor
{
private:
	PlayerStateActions UpdateVelocity(float dt);
	void UpdateSpeedBoost(float dt);
	void StopDash();
	void UpdateVisualState();

	PlayerStateManager m_stateManager;
	float m_speedMultiplier;
	float m_speedBoostTimer;

protected:
	void OnHorizontalCollision(float surfaceNormalX) override;

public:
	explicit Player(sf::Vector2f startPosition);

	void PushInput(const PlayerInputEvent& event);

	void Update(float dt, EntityContext& context) override;
	bool IsPendingRemoval() const override;

	void ApplySpeedBoost(float multiplier, float duration);
	float GetHealCooldown() const;
	float GetDashCooldown() const;
};
