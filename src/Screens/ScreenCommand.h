#pragma once

#include <cstdint>

enum class ScreenCommand : std::uint8_t
{
	None,
	StartGame,
	HowToPlay,
	Pause,
	Resume,
	Restart,
	ReturnToMenu,
	Exit,
	Victory,
	Defeat
};
