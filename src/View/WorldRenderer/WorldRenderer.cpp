#include "View/WorldRenderer/WorldRenderer.h"

constexpr sf::Color BACKGROUND_COLOR(135, 206, 235);

WorldRenderer::WorldRenderer(const sf::Vector2f worldSize)
	: 	m_parallaxBackground(worldSize)
{
}

void WorldRenderer::Draw(
	const RenderSnapshot& renderSnapshot, 
	sf::RenderTarget& target, 
	const sf::View& camera
) const
{
	target.clear(BACKGROUND_COLOR);
	m_parallaxBackground.Draw(target, camera);
	m_mapRenderer.Draw(renderSnapshot.map, target);
	m_entityRenderer.Draw(renderSnapshot.entities, target);
}
