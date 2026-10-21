#include "View/PlayerRenderer/PlayerRenderer.h"

const char *const PLAYER_TEXTURE_PATH = "./assets/hero.png";
const float SPRITE_HEIGHT = 100.f;

PlayerRenderer::PlayerRenderer()
	: m_texture(PLAYER_TEXTURE_PATH),
	  m_sprite(m_texture),
	  m_scale(1.f)
{
	const sf::Vector2u textureSize = m_texture.getSize();
	if (textureSize.y > 0)
	{
		m_scale = SPRITE_HEIGHT / static_cast<float>(textureSize.y);
		m_sprite.setScale({m_scale, m_scale});
	}

	const sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

	m_spriteSize = m_sprite.getGlobalBounds().size;
}

void PlayerRenderer::Draw(const Player& player, sf::RenderTarget& target)
{
	m_sprite.setScale({player.GetFacingDirection() * m_scale, m_scale});
	m_sprite.setPosition(player.GetPosition());
	target.draw(m_sprite);
}

sf::Vector2f PlayerRenderer::GetSpriteSize() const
{
	return m_spriteSize;
}
