#include <SFML/Graphics/Sprite.hpp>
#include <algorithm>
#include <cmath>

#include "View/ParallaxBackground/ParallaxBackground.h"
#include "View/ParallaxBackground/ParallaxConstans.h"

ParallaxBackground::ParallaxBackground(ResourceManager& resources, const sf::Vector2f worldSize)
    :   m_worldSize(worldSize)
{
    // The farthest layer (last in the config) is drawn first.
    for (auto it = ParallaxConstants::LAYERS.rbegin(); it != ParallaxConstants::LAYERS.rend(); ++it)
    {
        m_layers.push_back({&resources.GetTexture(it->texturePath), it->parallaxFactor, it->isHorizontalRepeat});
    }
}

void ParallaxBackground::Draw(
	sf::RenderTarget& target,
	const sf::View& worldView
) const
{
	const float cameraSizeX = worldView.getSize().x;

	const float cameraLeft = worldView.getCenter().x - cameraSizeX / 2.f;

	const float cameraRight = worldView.getCenter().x + cameraSizeX / 2.f;

	for (const ParallaxLayer& layer : m_layers)
	{
		const ParallaxDrawData drawData = CalculateParallax(
			layer,
			cameraLeft,
			cameraRight
		);

		sf::Sprite sprite(*layer.texture);

		sprite.setScale({drawData.scale, drawData.scale});
		sprite.setOrigin({
			0.f,
			static_cast<float>(layer.texture->getSize().y) / 2.f
		});

		for (std::size_t i = 0; i < drawData.tileCount; ++i)
		{
			const float x = drawData.firstX + i * drawData.backgroundWidth;

			sprite.setPosition({x, drawData.backgroundY});
			target.draw(sprite);
		}
	}
}

ParallaxBackground::ParallaxDrawData ParallaxBackground::CalculateParallax(
	const ParallaxLayer& layer,
	const float cameraLeft,
	const float cameraRight
) const
{
	const sf::Vector2u textureSize = layer.texture->getSize();

	const float scale = m_worldSize.y / static_cast<float>(textureSize.y);

	const float backgroundY = m_worldSize.y / 2.f;
	const float backgroundWidth = textureSize.x * scale;

	float firstX = 0.f;
	std::size_t tileCount = 1;

	if (layer.isHorizontalRepeat)
	{
		const float cameraSizeX = cameraRight - cameraLeft;

		float parallaxOffsetX = cameraLeft * layer.parallaxFactor;

		float phase = std::fmod(parallaxOffsetX, backgroundWidth);

		if (phase < 0.f)
		{
			phase += backgroundWidth;
		}

		firstX = cameraLeft - phase;

		tileCount = static_cast<std::size_t>(std::ceil((cameraSizeX + phase) / backgroundWidth)) + 1;
	}
	else
	{
		const float cameraSizeX = cameraRight - cameraLeft;
		float maxCameraTravel = m_worldSize.x - cameraSizeX;
		
		if (maxCameraTravel <= 0.f) 
		{
			maxCameraTravel = 1.f; 
		}

		float travelPercent = cameraLeft / maxCameraTravel;
		travelPercent = std::clamp(travelPercent, 0.f, 1.f);

		float maxBackgroundTravel = backgroundWidth - cameraSizeX;

		firstX = cameraLeft - travelPercent * maxBackgroundTravel;
	}

	return {
		scale,
		backgroundY,
		firstX,
		backgroundWidth,
		tileCount
	};
}
