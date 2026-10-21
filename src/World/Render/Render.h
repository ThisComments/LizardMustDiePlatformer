#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "World/Map/Map.h"

class Render
{
private:
    sf::Texture m_textureGround;
    sf::Sprite m_spriteGround;

    sf::Texture m_textureIce;
    sf::Sprite m_spriteIce;

    sf::Texture m_textureLava;
    sf::Sprite m_spriteLava;

public:
    Render();

    void Draw(const Map& map, sf::RenderTarget& target) const;
};