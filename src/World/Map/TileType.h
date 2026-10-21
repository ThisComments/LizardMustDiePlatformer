#pragma once

#include <cstddef>
#include <cstdint>

enum class TileType: int
{
	Empty = 0,
	Ground = 1,
	Stone = 2,
	WoodPlanks = 3,
	Water = 4,
	Ice = 5,
	Lava = 6
};

inline constexpr int TILE_TYPE_COUNT = 7;
