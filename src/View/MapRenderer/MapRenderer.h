#pragma once

#include <array>
#include <optional>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>

#include "Model/Map/Map.h"
#include "Model/Map/TileType.h"
#include "Resources/ResourceManager.h"

class MapRenderer
{
private:
	std::array<std::optional<sf::Sprite>, TILE_TYPE_COUNT> m_tileSprites;

public:
	MapRenderer(ResourceManager& resources);

	void Draw(sf::RenderTarget& target, const Map& map, const sf::View& view) const;
};
