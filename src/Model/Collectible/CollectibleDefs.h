#pragma once

#include <stdexcept>

#include <SFML/System/Vector2.hpp>

#include "Model/Collectible/CollectibleType.h"
#include "Resources/AssetPaths.h"

// Gameplay configuration of a collectible type. Adding a new type = new enum value + new entry
// here + a sprite sheet with "idle" and "collect" animations.
struct CollectibleDef
{
	int points;
	sf::Vector2f size;           // hitbox
	const char* spriteSheetPath;
};

inline const CollectibleDef& GetCollectibleDef(const CollectibleType type)
{
	static const CollectibleDef COIN{10, {40.f, 40.f}, AssetPaths::CoinSheet};
	static const CollectibleDef GEM{50, {40.f, 40.f}, AssetPaths::GemSheet};

	switch (type)
	{
		case CollectibleType::Gem:
			return GEM;
		case CollectibleType::Coin:
			return COIN;
	}
	throw std::logic_error("Unknown collectible type");
}
