#pragma once

#include <cstdint>

// Drawing order of dynamic entities inside the world pass. Lower layers are drawn first;
// inside a layer entities are ordered by GetRenderDepth().
enum class RenderLayer : std::uint8_t
{
	Background,
	Pickups,
	Characters,
	Foreground
};
