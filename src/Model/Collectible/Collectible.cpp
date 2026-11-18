#include "Model/Collectible/Collectible.h"

#include <utility>

Collectible::Collectible(
	const sf::Vector2f startPosition,
	const sf::Vector2f hitboxSize,
	const VisualId initialVisualId,
	const CollectibleEffect& initialEffect
)
	: 	Collectible(
			startPosition,
			hitboxSize,
			initialVisualId,
			std::vector<CollectibleEffect>{ initialEffect }
		)
{
}

Collectible::Collectible(
	const sf::Vector2f startPosition,
	const sf::Vector2f hitboxSize,
	const VisualId initialVisualId,
	std::vector<CollectibleEffect> initialEffects
)
	: 	Entity(initialVisualId),
		m_position(startPosition),
		m_bounds(startPosition - hitboxSize / 2.f, hitboxSize),
		m_effects(std::move(initialEffects)),
		m_collected(false)
{
}

void Collectible::Update(const float, EntityContext&)
{
}

sf::Vector2f Collectible::GetPosition() const
{
	return m_position;
}

sf::FloatRect Collectible::GetBounds() const
{
	return m_bounds;
}

bool Collectible::IsPendingRemoval() const
{
	return m_collected;
}

sf::FloatRect Collectible::GetTriggerBounds() const
{
	return m_bounds;
}

void Collectible::AddEffect(const CollectibleEffect& effect)
{
	m_effects.push_back(effect);
}

const std::vector<CollectibleEffect>& Collectible::GetEffects() const
{
	return m_effects;
}

bool Collectible::IsCollected() const
{
	return m_collected;
}

void Collectible::MarkCollected()
{
	m_collected = true;
}
