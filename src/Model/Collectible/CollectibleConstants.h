#pragma once

#include <SFML/System/Vector2.hpp>

namespace CollectibleConstants
{
	constexpr sf::Vector2f HITBOX_SIZE = { 40.f, 40.f };

	constexpr float COIN_SCORE_VALUE = 10.f;
	constexpr float GEM_SCORE_VALUE = 50.f;

	constexpr float DEFAULT_HEAL_VALUE = 30.f;
	constexpr float DEFAULT_SPEED_BOOST_MULTIPLIER = 1.5f;
	constexpr float DEFAULT_SPEED_BOOST_DURATION = 5.f;
}
