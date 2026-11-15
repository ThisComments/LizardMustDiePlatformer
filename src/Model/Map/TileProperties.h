#pragma once

#include <array>
#include <cstddef>

#include "Common/PhysicsTypes.h"
#include "Model/Map/TileType.h"
#include "Resources/AssetPaths.h"

struct TileProperties
{
	bool isSolid = false;
	float friction = 0.f;
	bool isLiquid = false;
	MovementModifiers movementModifiers;
	const char* texturePath = nullptr;
};

constexpr float GROUND_FRICTION = 1800.f;
constexpr float STONE_FRICTION = 1800.f;
constexpr float ICE_FRICTION = 100.f;
constexpr float WOOD_PLANKS_FRICTION = 1800.f;

constexpr MovementModifiers WATER_MOVEMENT_MODIFIERS = {0.3f, 0.6f, true, false};
constexpr MovementModifiers LAVA_MOVEMENT_MODIFIERS = {1.f, 0.4f, false, false};

const std::array<TileProperties, TILE_TYPE_COUNT> TILE_PROPERTIES = {{
	{false, 0.f, false, DEFAULT_MOVEMENT_MODIFIERS, nullptr},
	{true, GROUND_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::GroundTexture},
	{true, STONE_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::StoneTexture},
	{true, WOOD_PLANKS_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::WoodPlanksTexture},
	{false, 0.f, true, WATER_MOVEMENT_MODIFIERS, AssetPaths::WaterTexture},
	{true, ICE_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::IceTexture},
	{false, 0.f, true, LAVA_MOVEMENT_MODIFIERS, AssetPaths::LavaTexture}
}};

inline const TileProperties& GetTileProperties(const TileType tileType)
{
	const auto index = static_cast<std::size_t>(tileType);
	return index < TILE_PROPERTIES.size() ? TILE_PROPERTIES[index] : TILE_PROPERTIES[0];
}
