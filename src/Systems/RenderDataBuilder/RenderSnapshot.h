#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Model/Map/TileType.h"

struct MapRenderData
{
    std::vector<TileType> tiles;
	int width = 0;
	int height = 0;
    float tileSize = 0.f;

    std::size_t GetIndex(const int col, const int row) const
	{
		return static_cast<std::size_t>(row) * static_cast<std::size_t>(width) +
			static_cast<std::size_t>(col);
	}
};

enum class EntityVisual
{
	Unknown = 0,
	Hero = 1
};

struct EntityRenderData
{
    sf::Vector2f position;
	sf::Vector2f size;
    float facingDirection = 1.f;
    EntityVisual visual = EntityVisual::Unknown;
};

struct RenderSnapshot
{
    MapRenderData map;
    std::vector<EntityRenderData> entities;
    sf::Vector2f cameraTargetPosition;
};
