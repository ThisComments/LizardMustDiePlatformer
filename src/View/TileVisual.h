#pragma once

#include <array>
#include <cstddef>

#include <SFML/Graphics/Color.hpp>

#include "World/Map/TileType.h"

struct TileVisual
{
	bool isVisible = false;
	sf::Color color = sf::Color::Transparent;
};

inline const std::array<TileVisual, TILE_TYPE_COUNT> TILE_VISUALS = {{
	{false, sf::Color::Transparent},
	{true, sf::Color(158, 106, 54)},
	{true, sf::Color(111, 111, 111)},
	{true, sf::Color(0, 80, 255, 150)},
	{true, sf::Color(140, 220, 255)},
	{true, sf::Color(255, 80, 0, 200)},
	{true, sf::Color(245, 73, 39)}
}};

inline const TileVisual& GetTileVisual(const TileType tileType)
{
	const auto index = static_cast<std::size_t>(tileType);
	return index < TILE_VISUALS.size() ? TILE_VISUALS[index] : TILE_VISUALS[0];
}
