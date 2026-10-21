#pragma once

#include <array>
#include <cstddef>

#include "Common/PhysicsTypes.h"
#include "World/Map/TileType.h"

struct TileProperties
{
	bool isSolid = false;
	float friction = 0.f;
	bool isLiquid = false;
	MovementModifiers movementModifiers;
};

inline constexpr float GROUND_FRICTION = 1800.f;
inline constexpr float STONE_FRICTION = 1800.f;
inline constexpr float ICE_FRICTION = 100.f;
inline constexpr float WOOD_PLANKS_FRICTION = 1800.f;

inline constexpr MovementModifiers WATER_MOVEMENT_MODIFIERS = {0.3f, 0.6f, true, false};
inline constexpr MovementModifiers LAVA_MOVEMENT_MODIFIERS = {1.f, 0.4f, false, false};

inline const std::array<TileProperties, TILE_TYPE_COUNT> TILE_PROPERTIES = {{
	{false, 0.f, false, DEFAULT_MOVEMENT_MODIFIERS},
	{true, GROUND_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS},
	{true, STONE_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS},
	{true, WOOD_PLANKS_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS},
	{false, 0.f, true, WATER_MOVEMENT_MODIFIERS},
	{true, ICE_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS},
	{false, 0.f, true, LAVA_MOVEMENT_MODIFIERS}
}};

inline const TileProperties& GetTileProperties(const TileType tileType)
{
	const auto index = static_cast<std::size_t>(tileType);
	return index < TILE_PROPERTIES.size() ? TILE_PROPERTIES[index] : TILE_PROPERTIES[0];
}
