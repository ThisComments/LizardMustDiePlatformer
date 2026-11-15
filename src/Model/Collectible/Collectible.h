#pragma once

#include "Animation/AnimationComponent.h"
#include "Model/Collectible/CollectibleType.h"
#include "Model/Entity/Entity.h"
#include "Model/Entity/Triggerable.h"
#include "Resources/SpriteSheetAsset.h"

// A pickup. It stays visible while its "collect" animation plays and is removed afterwards.
class Collectible final : public Entity, public Triggerable
{
private:
	CollectibleState m_state;
	sf::Vector2f m_position;
	sf::FloatRect m_bounds;
	int m_points;
	AnimationComponent m_animation;

	void Collect(EntityContext& context);

public:
	// animationPhase in [0, 1) offsets the idle animation so neighbouring items do not blink in lockstep.
	Collectible(
		CollectibleType type,
		sf::Vector2f position,
		const SpriteSheetAsset& sheet,
		float animationPhase
	);

	void Update(float dt, EntityContext& context) override;
	void Draw(sf::RenderTarget& target) const override;

	sf::Vector2f GetPosition() const override;
	sf::FloatRect GetBounds() const override;
	RenderLayer GetRenderLayer() const override;
	float GetRenderDepth() const override;
	bool IsPendingRemoval() const override;

	sf::FloatRect GetTriggerBounds() const override;
	void OnPlayerEnter(EntityContext& context) override;
};
