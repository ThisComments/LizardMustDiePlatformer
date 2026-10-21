#pragma once

struct MovementModifiers
{
	float gravityScale = 1.f;
	float movementSpeedScale = 1.f;
	bool allowsJump = true;
	bool allowsDash = true;
};

inline constexpr MovementModifiers DEFAULT_MOVEMENT_MODIFIERS = {1.f, 1.f, true, true};
