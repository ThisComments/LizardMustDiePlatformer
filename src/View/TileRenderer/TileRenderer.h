#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "World/Map/Map.h"

class TileRenderer
{
private:
    sf::Texture m_textureGround;
    sf::Sprite m_spriteGround;

    sf::Texture m_textureIce;
    sf::Sprite m_spriteIce;

    sf::Texture m_textureWoodPlanks;
    sf::Sprite m_spriteWoodPlanks;

	sf::Texture m_textureStone;
    sf::Sprite m_spriteStone;

	sf::RectangleShape m_fallbackShape;
	void DrawTile(
		const TileType tileType, 
		const sf::Vector2f position, 
		sf::RenderTarget& target
	);

public:
    TileRenderer();

    TileRenderer(const TileRenderer&) = delete;
    TileRenderer& operator=(const TileRenderer&) = delete;

    void Draw(const Map& map, sf::RenderTarget& target);
};