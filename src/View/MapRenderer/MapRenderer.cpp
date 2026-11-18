#include "View/MapRenderer/MapRenderer.h"

#include <algorithm>
#include <cmath>

#include <SFML/Graphics/Color.hpp>

#include "Model/Map/TileProperties.h"

namespace
{
	constexpr sf::Color BACKGROUND_TILE_TINT(145, 145, 165, 255);
}

MapRenderer::MapRenderer(ResourceManager& resources)
{
	for (std::size_t index = 0; index < m_tileSprites.size(); ++index)
	{
		const TileProperties& properties = GetTileProperties(static_cast<TileType>(index));
		if (properties.texturePath == nullptr)
		{
			continue;
		}

		const sf::Texture& texture = resources.GetTexture(properties.texturePath);
		const sf::Vector2f textureSize(texture.getSize());

		sf::Sprite sprite(texture);
		sprite.setScale({
			static_cast<float>(TILE_SIZE) / textureSize.x,
			static_cast<float>(TILE_SIZE) / textureSize.y
		});
		if (properties.isBackground)
		{
			sprite.setColor(BACKGROUND_TILE_TINT);
		}
		m_tileSprites[index] = sprite;
	}
}

void MapRenderer::Draw(sf::RenderTarget& target, const Map& map, const sf::View& view) const
{
	const float tileSize = static_cast<float>(TILE_SIZE);
	const sf::Vector2f topLeft = view.getCenter() - view.getSize() / 2.f;
	const sf::Vector2f bottomRight = view.getCenter() + view.getSize() / 2.f;

	const int firstCol = std::max(0, static_cast<int>(std::floor(topLeft.x / tileSize)));
	const int firstRow = std::max(0, static_cast<int>(std::floor(topLeft.y / tileSize)));
	const int lastCol = std::min(map.GetWidth() - 1, static_cast<int>(std::floor(bottomRight.x / tileSize)));
	const int lastRow = std::min(map.GetHeight() - 1, static_cast<int>(std::floor(bottomRight.y / tileSize)));

	for (int row = firstRow; row <= lastRow; ++row)
	{
		for (int col = firstCol; col <= lastCol; ++col)
		{
			const std::optional<sf::Sprite>& prototype =
				m_tileSprites[static_cast<std::size_t>(map.GetTileAtCell(col, row))];
			if (!prototype)
			{
				continue;
			}

			sf::Sprite sprite = *prototype;
			sprite.setPosition({static_cast<float>(col) * tileSize, static_cast<float>(row) * tileSize});
			target.draw(sprite);
		}
	}
}