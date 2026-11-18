#pragma once

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>

#include "Resources/ResourceManager.h"

struct ParallaxLayer
{
	const sf::Texture* texture;
	float parallaxFactor;
	bool isHorizontalRepeat;
};

class ParallaxBackground
{
private:
	struct ParallaxDrawData
	{
		float scale;
		float backgroundY;
		float firstX;
		float backgroundWidth;
		std::size_t tileCount;
	};

	std::vector<ParallaxLayer> m_layers;
    sf::Vector2f m_worldSize;

	ParallaxDrawData CalculateParallax(
		const ParallaxLayer& layer,
		float cameraLeft,
		float cameraRight
	) const;

public:
	ParallaxBackground(ResourceManager& resources, sf::Vector2f worldSize);

	void Draw(sf::RenderTarget& target, const sf::View& worldView) const;
};