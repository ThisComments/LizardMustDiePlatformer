#pragma once

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Systems/RenderDataBuilder/RenderSnapshot.h"

class EntityRenderer
{
private:
	sf::Texture m_textureHero;
	sf::Texture m_textureFallback;

	const sf::Texture& GetTexture(EntityVisual visual) const;
	void DrawEntity(
		const EntityRenderData& entity, 
		sf::RenderTarget& target
	) const;

public:
	EntityRenderer();

	EntityRenderer(const EntityRenderer&) = delete;
	EntityRenderer& operator=(const EntityRenderer&) = delete;

	void Draw(
		const std::vector<EntityRenderData>& entities, 
		sf::RenderTarget& target
	) const;
};
