#include <SFML/Graphics/Sprite.hpp>
#include "View/MapRenderer/MapRenderer.h"
#include "View/RenderConstants.h"

MapRenderer::MapRenderer()
	: 	m_textureGround(RenderConstants::GroundTexturePath),
		m_textureIce(RenderConstants::IceTexturePath),
		m_textureWoodPlanks(RenderConstants::WoodPlanksTexturePath),
		m_textureStone(RenderConstants::StoneTexturePath),
		m_textureWater(RenderConstants::WaterTexturePath),
		m_textureLava(RenderConstants::LavaTexturePath),
		m_textureFallback(RenderConstants::FallbackTexturePath)
{
}

void MapRenderer::Draw(
	const MapRenderData& map, 
	sf::RenderTarget& target
) const
{
	for (int row = 0; row < map.height; ++row)
	{
		for (int col = 0; col < map.width; ++col)
		{
            const TileType tileType = map.tiles[map.GetIndex(col, row)];

			const sf::Vector2f position(
				static_cast<float>(col) * map.tileSize,
				static_cast<float>(row) * map.tileSize
			);

			DrawTile(tileType, position, map.tileSize, target);
		}
	}
}

const sf::Texture& MapRenderer::GetTexture(const TileType tileType) const
{
	switch (tileType)
	{
		case TileType::Ground:
			return m_textureGround;
		case TileType::Ice:
			return m_textureIce;
		case TileType::Stone:
			return m_textureStone;
		case TileType::WoodPlanks:
			return m_textureWoodPlanks;
		case TileType::Water:
			return m_textureWater;
		case TileType::Lava:
			return m_textureLava;
		case TileType::Empty:
			break;
	}
	return m_textureFallback;
}

void MapRenderer::DrawTile(
	const TileType tileType,
	const sf::Vector2f position,
	const float tileSize,
	sf::RenderTarget& target
) const
{
	if (tileType == TileType::Empty)
	{
		return;
	}

	sf::Sprite sprite(GetTexture(tileType));
	sprite.setPosition(position);
	
	const sf::FloatRect bounds = sprite.getLocalBounds();
	if (bounds.size.x <= 0.f || bounds.size.y <= 0.f)
	{
		return;
	}

	sprite.setScale({
		tileSize / bounds.size.x,
		tileSize / bounds.size.y
	});

	target.draw(sprite);
}
