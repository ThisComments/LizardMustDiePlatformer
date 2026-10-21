#include "View/TileRenderer/TileRenderer.h"

#include "View/TileVisual.h"

namespace
{
	const char *const GROUND_TEXTURE_PATH = "./assets/ground.png";
	const char *const ICE_TEXTURE_PATH = "./assets/ice.png";
	const char *const STONE_TEXTURE_PATH = "./assets/stone.png";
	const char *const WOOD_PLANKS_TEXTURE_PATH = "./assets/wood_planks.png";

	void ScaleSpriteToTile(sf::Sprite& sprite)
	{
		const sf::FloatRect bounds = sprite.getLocalBounds();
		if (bounds.size.x <= 0.f || bounds.size.y <= 0.f)
		{
			return;
		}

		const float tileSize = static_cast<float>(TILE_SIZE);
		sprite.setScale({
			tileSize / bounds.size.x,
			tileSize / bounds.size.y
		});
	}
}

TileRenderer::TileRenderer()
	: m_textureGround(GROUND_TEXTURE_PATH),
	  m_spriteGround(m_textureGround),
	  m_textureIce(ICE_TEXTURE_PATH),
	  m_spriteIce(m_textureIce),
	  m_textureWoodPlanks(WOOD_PLANKS_TEXTURE_PATH),
	  m_spriteWoodPlanks(m_textureWoodPlanks),
	  m_textureStone(STONE_TEXTURE_PATH),
	  m_spriteStone(m_textureStone),
	  m_fallbackShape({static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)})
{
	ScaleSpriteToTile(m_spriteGround);
	ScaleSpriteToTile(m_spriteIce);
	ScaleSpriteToTile(m_spriteWoodPlanks);
	ScaleSpriteToTile(m_spriteStone);
}

void TileRenderer::Draw(const Map& map, sf::RenderTarget& target)
{
	const float tileSize = static_cast<float>(TILE_SIZE);

	for (int row = 0; row < map.GetHeight(); ++row)
	{
		for (int col = 0; col < map.GetWidth(); ++col)
		{
			const TileType tileType = map.GetTileAtCell(
				static_cast<int>(col),
				static_cast<int>(row)
			);
			const sf::Vector2f position(
				static_cast<float>(col) * tileSize,
				static_cast<float>(row) * tileSize
			);

			DrawTile(tileType, position, target);
		}
	}
}

void TileRenderer::DrawTile(
	const TileType tileType,
	const sf::Vector2f position,
	sf::RenderTarget& target
)
{
	sf::Sprite *sprite = nullptr;

	switch (tileType)
	{
		case TileType::Ground:
			sprite = &m_spriteGround;
			break;

		case TileType::Ice:
			sprite = &m_spriteIce;
			break;

		case TileType::Stone:
			sprite = &m_spriteStone;
			break;
	
		case TileType::WoodPlanks:
			sprite = &m_spriteWoodPlanks;
			break;

		case TileType::Empty:
			return;

		case TileType::Water:
		case TileType::Lava:
		{
			const TileVisual& tileVisual = GetTileVisual(tileType);
			if (!tileVisual.isVisible)
			{
				return;
			}

			m_fallbackShape.setPosition(position);
			m_fallbackShape.setFillColor(tileVisual.color);
			target.draw(m_fallbackShape);
			return;
		}
	}

	sprite->setPosition(position);
	target.draw(*sprite);
}
