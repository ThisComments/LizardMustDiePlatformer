#pragma once

#include "World/Map/TileType.h"
#include "Player/Player.h"

#include <SFML/Graphics/Color.hpp>
#include <array>
#include <cstddef>

struct TileProperties
{
    bool isSolid = false;
    float friction = 0.f;
    bool isLiquid = false;
    MovementModifiers movementModifiers;
    sf::Color color = sf::Color::Transparent;
};

inline constexpr float GROUND_FRICTION = 1800.f;
inline constexpr float STONE_FRICTION = 1800.f;
inline constexpr float ICE_FRICTION = 100.f;

inline constexpr MovementModifiers DEFAULT_MOVEMENT_MODIFIERS = {1.f, 1.f, true, true};
inline constexpr MovementModifiers WATER_MOVEMENT_MODIFIERS = {0.3f, 0.6f, true, false};
inline constexpr MovementModifiers LAVA_MOVEMENT_MODIFIERS = {1.f, 0.4f, false, false};

inline const std::array<TileProperties, 6> TILE_PROPERTIES = {{
    {false, 0.f, false, DEFAULT_MOVEMENT_MODIFIERS, sf::Color::Transparent},
    {true, GROUND_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, sf::Color(158, 106, 54)},
    {true, STONE_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, sf::Color(111, 111, 111)},
    {false, 0.f, true, WATER_MOVEMENT_MODIFIERS, sf::Color(0, 80, 255, 150)},
    {true, ICE_FRICTION, false, DEFAULT_MOVEMENT_MODIFIERS, sf::Color(140, 220, 255)},
    {false, 0.f, true, LAVA_MOVEMENT_MODIFIERS, sf::Color(255, 80, 0, 200)}
}};

inline const TileProperties& GetTileProperties(const TileType tileType)
{
    const auto index = static_cast<std::size_t>(tileType);
    return index < TILE_PROPERTIES.size() ? TILE_PROPERTIES[index] : TILE_PROPERTIES[0];
}
