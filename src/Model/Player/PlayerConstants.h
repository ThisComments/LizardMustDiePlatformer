#pragma once

#include <SFML/System/Vector2.hpp>

namespace PlayerConstants
{
	constexpr sf::Vector2f COLLISION_SIZE = { 40.f, 80.f };
	constexpr int MAX_HEALTH = 100;
	constexpr int BAIKAL_WATER_HEAL_AMOUNT = 40;
	constexpr float DEFAULT_SPEED_MULTIPLIER = 1.f;

	constexpr float JUMP_RELEASE_GRAVITY_MULTIPLIER = 2.f;
	constexpr float DASH_SPEED = 1000.f;
	constexpr float MAX_SPEED = 300.f;
	constexpr float JUMP_SPEED = -650.f;
	constexpr float ACCELERATION = 1800.f;

	constexpr float RUN_SPEED_THRESHOLD = 10.f;
	constexpr float RISE_SPEED_THRESHOLD = 1.f;
}
