#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "Player/Player.h"

class PlayerRenderer
{
private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	float m_scale;
	sf::Vector2f m_spriteSize;

public:
	PlayerRenderer();
	PlayerRenderer(const PlayerRenderer&) = delete;
	PlayerRenderer& operator=(const PlayerRenderer&) = delete;

	void Draw(const Player& player, sf::RenderTarget& target);
	sf::Vector2f GetSpriteSize() const;
};
