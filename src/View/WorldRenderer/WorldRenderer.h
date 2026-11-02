#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "View/EntityRenderer/EntityRenderer.h"
#include "View/MapRenderer/MapRenderer.h"
#include "View/ParallaxBackground/ParallaxBackground.h"
#include "Systems/RenderDataBuilder/RenderSnapshot.h"

class WorldRenderer
{
private:
	MapRenderer m_mapRenderer;
	EntityRenderer m_entityRenderer;
	ParallaxBackground m_parallaxBackground;

public:
	WorldRenderer(const sf::Vector2f worldSize);
	void Draw(
		const RenderSnapshot& renderSnapshot, 
		sf::RenderTarget& target, 
		const sf::View& camera
	) const;
};
