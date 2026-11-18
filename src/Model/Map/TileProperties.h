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
	int contactDamage = 0;
	bool isBackground = false;
};

constexpr float GROUND_FRICTION = 1800.f;
constexpr float STONE_FRICTION = 1800.f;
constexpr float SNOW_FRICTION = 1200.f;
constexpr float ICE_FRICTION = 100.f;
constexpr float WOOD_PLANKS_FRICTION = 1800.f;
constexpr int LAVA_CONTACT_DAMAGE = 25;

constexpr MovementModifiers WATER_MOVEMENT_MODIFIERS = { 0.3f, 0.6f, true, false };
constexpr MovementModifiers LAVA_MOVEMENT_MODIFIERS = { 1.f, 0.4f, false, false };

const std::array<TileProperties, TILE_TYPE_COUNT> TILE_PROPERTIES = {{
	{ false, 0.f, false, DEFAULT_MOVEMENT_MODIFIERS, nullptr, 0, false },
	{ true, GROUND_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::GroundTexture, 0, false },
	{ true, STONE_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::StoneTexture, 0, false },
	{ true, WOOD_PLANKS_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::WoodPlanksTexture, 0, false },
	{ false, 0.f, true, WATER_MOVEMENT_MODIFIERS, AssetPaths::WaterTexture, 0, false },
	{ true, ICE_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::IceTexture, 0, false },
	{ false, 0.f, true, LAVA_MOVEMENT_MODIFIERS, AssetPaths::LavaTexture, LAVA_CONTACT_DAMAGE, false },
	{ true, SNOW_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::SnowTexture, 0, false },
	{ true, STONE_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::SnowBrickTexture, 0, false },
	{ true, STONE_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::LizardTechTexture, 0, false },
	{ false, 0.f, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::WoodWallBgTexture, 0, true },
	{ false, 0.f, false, DEFAULT_MOVEMENT_MODIFIERS, AssetPaths::StoneWallBgTexture, 0, true }
}};

inline const TileProperties& GetTileProperties(const TileType tileType)
{
	const auto index = static_cast<std::size_t>(tileType);
	return index < TILE_PROPERTIES.size() ? TILE_PROPERTIES[index] : TILE_PROPERTIES[0];
}
