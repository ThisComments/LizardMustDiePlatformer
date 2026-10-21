#include "Render.h"

#include "World/Map/TileProperties.h"

Render::Render()
    :   m_textureGround("./assets/ground.png"),
        m_spriteGround(m_textureGround),
        m_textureIce("./assets/ice.jpg"),
        m_spriteIce(m_textureIce),
        m_textureLava("./assets/lava.jpg"),
        m_spriteLava(m_textureLava)
{
    const sf::Vector2f tileSize(
        static_cast<float>(TILE_SIZE),
        static_cast<float>(TILE_SIZE)
    );

    m_spriteGround.setScale({
        tileSize.x / m_spriteGround.getLocalBounds().size.x,
        tileSize.y / m_spriteGround.getLocalBounds().size.y
    });

    m_spriteIce.setScale({
        tileSize.x / m_spriteIce.getLocalBounds().size.x,
        tileSize.y / m_spriteIce.getLocalBounds().size.y
    });

    m_spriteLava.setScale({
        tileSize.x / m_spriteLava.getLocalBounds().size.x,
        tileSize.y / m_spriteLava.getLocalBounds().size.y
    });
}

void Render::Draw(const Map& map, sf::RenderTarget& target) const
{
    const float tileSize = static_cast<float>(TILE_SIZE);

    for (uint32_t row = 0; row < map.height; ++row)
    {
        for (uint32_t col = 0; col < map.width; ++col)
        {
            const float worldX = (static_cast<float>(col) + 0.5f) * tileSize;
            const float worldY = (static_cast<float>(row) + 0.5f) * tileSize;

            const uint32_t visualId = MapGetVisualId(map, worldX, worldY);

            if (visualId == TVI_NONE)
            {
                continue;
            }

            sf::Sprite *sprite = nullptr;

            switch (visualId)
            {
                case TVI_GROUND:
                    sprite = const_cast<sf::Sprite *>(&m_spriteGround);
                    break;

                case TVI_ICE:
                    sprite = const_cast<sf::Sprite *>(&m_spriteIce);
                    break;

                case TVI_LAVA:
                    sprite = const_cast<sf::Sprite *>(&m_spriteLava);
                    break;

                default:
                    continue;
            }

            sprite->setPosition({
                static_cast<float>(col) * tileSize,
                static_cast<float>(row) * tileSize
            });

            target.draw(*sprite);
        }
    }
}