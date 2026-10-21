#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Player/Player.h"
#include "View/PlayerRenderer/PlayerRenderer.h"
#include "View/TileRenderer/TileRenderer.h"
#include "World/Map/Map.h"

class WorldRenderer
{
private:
	TileRenderer m_tileRenderer;
	PlayerRenderer m_playerRenderer;

public:
	void Draw(const Map& map, const Player& player, sf::RenderTarget& target);
	sf::Vector2f GetPlayerSpriteSize() const;
};
