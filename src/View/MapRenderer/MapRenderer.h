#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Systems/RenderDataBuilder/RenderSnapshot.h"

class MapRenderer
{
private:
    sf::Texture m_textureGround;
    sf::Texture m_textureIce;
    sf::Texture m_textureWoodPlanks;
    sf::Texture m_textureStone;
    sf::Texture m_textureWater;
    sf::Texture m_textureLava;
    sf::Texture m_textureFallback;

	const sf::Texture& GetTexture(TileType tileType) const;
	void DrawTile(
		TileType tileType,
		sf::Vector2f position,
		float tileSize,
		sf::RenderTarget& target
	) const;

public:
    MapRenderer();

    MapRenderer(const MapRenderer&) = delete;
    MapRenderer& operator=(const MapRenderer&) = delete;

    void Draw(
        const MapRenderData& map, 
        sf::RenderTarget& target
    ) const;
};