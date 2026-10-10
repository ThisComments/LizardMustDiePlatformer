#pragma once
#include <cstdint>

enum class TileType : uint8_t
{
	Empty = 0,
	Ground = 1,
	Stone = 2,
	Water = 3
};

enum TileVisualId : uint32_t
{
	TVI_NONE = 0,
	TVI_GROUND = 1,
	TVI_STONE = 2,
	TVI_WATER = 3
};
