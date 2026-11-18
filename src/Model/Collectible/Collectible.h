#pragma once

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Model/Collectible/CollectibleType.h"
#include "Model/Entity/Entity.h"
#include "Model/Entity/Triggerable.h"

class Collectible final : public Entity, public Triggerable
{
private:
	sf::Vector2f m_position;
	sf::FloatRect m_bounds;
	std::vector<CollectibleEffect> m_effects;
	bool m_collected;

public:
	Collectible(
		sf::Vector2f startPosition,
		sf::Vector2f hitboxSize,
		VisualId initialVisualId,
		const CollectibleEffect& initialEffect
	);

	Collectible(
		sf::Vector2f startPosition,
		sf::Vector2f hitboxSize,
		VisualId initialVisualId,
		std::vector<CollectibleEffect> initialEffects
	);

	void Update(float dt, EntityContext& context) override;

	sf::Vector2f GetPosition() const override;
	sf::FloatRect GetBounds() const override;
	bool IsPendingRemoval() const override;

	sf::FloatRect GetTriggerBounds() const override;

	void AddEffect(const CollectibleEffect& effect);
	const std::vector<CollectibleEffect>& GetEffects() const;

	bool IsCollected() const;
	void MarkCollected();
};
