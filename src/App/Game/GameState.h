#pragma once

#include <cstdint>

enum class GameState : std::uint8_t
{
	Menu,
	Playing,
	Paused,
	GameOver,
	//HowToPlay
};

enum class GameOverReason : std::uint8_t
{
	Victory,
	Defeat
};
