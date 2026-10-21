#include "View/WorldRenderer/WorldRenderer.h"

void WorldRenderer::Draw(const Map& map, const Player& player, sf::RenderTarget& target)
{
	m_tileRenderer.Draw(map, target);
	m_playerRenderer.Draw(player, target);
}

sf::Vector2f WorldRenderer::GetPlayerSpriteSize() const
{
	return m_playerRenderer.GetSpriteSize();
}
