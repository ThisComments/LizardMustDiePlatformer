#pragma once

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>

struct ParallaxLayer
{
	sf::Texture *texture;
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

    sf::Texture m_backgroundTexture1;
	sf::Texture m_backgroundTexture2;
	sf::Texture m_backgroundTexture3;
	std::vector<ParallaxLayer> layers;
    sf::Vector2f m_worldSize;

	ParallaxDrawData CalculateParallax(
		const ParallaxLayer& layer,
		float cameraLeft,
		float cameraRight
	) const;

public:
	explicit ParallaxBackground(const sf::Vector2f worldSize);

	void Draw(sf::RenderTarget& target, const sf::View& worldView) const;
};