#pragma once

#include <cstdint>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

class ScreenOverlay
{
private:
	sf::RectangleShape m_shape;

public:
	explicit ScreenOverlay(sf::Vector2f screenSize);

	void SetOpacity(std::uint8_t alpha);
	void Render(sf::RenderTarget& target) const;
};
