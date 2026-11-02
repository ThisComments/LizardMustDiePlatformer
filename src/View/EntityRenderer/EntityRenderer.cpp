#include <SFML/Graphics/Sprite.hpp>
#include "View/EntityRenderer/EntityRenderer.h"
#include "View/RenderConstants.h"

EntityRenderer::EntityRenderer()
	: 	m_textureHero(RenderConstants::HeroTexturePath),
	  	m_textureFallback(RenderConstants::FallbackTexturePath)
{
}

void EntityRenderer::Draw(
	const std::vector<EntityRenderData>& entities, 
	sf::RenderTarget& target
) const
{
	for (const EntityRenderData& entity: entities)
	{
		DrawEntity(entity, target);
	}
}

const sf::Texture& EntityRenderer::GetTexture(const EntityVisual visual) const
{
	switch (visual)
	{
		case EntityVisual::Hero:
			return m_textureHero;
		case EntityVisual::Unknown:
			break;
	}
	return m_textureFallback;
}

void EntityRenderer::DrawEntity(
	const EntityRenderData& entity,
	sf::RenderTarget& target
) const
{
	sf::Sprite sprite(GetTexture(entity.visual));

	const sf::FloatRect bounds = sprite.getLocalBounds();
	if (bounds.size.x <= 0.f || bounds.size.y <= 0.f)
	{
		return;
	}

	sprite.setOrigin(bounds.size / 2.f);
	sprite.setPosition(entity.position);
	sprite.setScale({
		entity.size.x / bounds.size.x * entity.facingDirection,
		entity.size.y / bounds.size.y
	});

	target.draw(sprite);
}