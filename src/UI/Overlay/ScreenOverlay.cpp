#include "UI/Overlay/ScreenOverlay.h"

#include <SFML/Graphics/Color.hpp>

#include "UI/UiStyle.h"

namespace
{
	constexpr sf::Color OVERLAY_BASE_COLOR(0, 0, 0);
}

ScreenOverlay::ScreenOverlay(const sf::Vector2f screenSize)
	: 	m_shape(screenSize)
{
	SetOpacity(UiStyle::OVERLAY_ALPHA);
}

void ScreenOverlay::SetOpacity(const std::uint8_t alpha)
{
	sf::Color color = OVERLAY_BASE_COLOR;
	color.a = alpha;
	m_shape.setFillColor(color);
}

void ScreenOverlay::Render(sf::RenderTarget& target) const
{
	target.setView(target.getDefaultView());
	target.draw(m_shape);
}
