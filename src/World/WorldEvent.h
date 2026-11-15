#pragma once

#include <cstdint>

// Facts about things that happened in the world. Gameplay code only reports them;
// reactions (sound, HUD, screen changes) are decided by whoever consumes the events.
enum class WorldEventType : std::uint8_t
{
	PlayerJumped,
	CollectibleCollected,
	VictoryReached,
	PlayerDefeated
};

struct WorldEvent
{
	WorldEventType type;
	int value = 0;  // e.g. points for CollectibleCollected
};
