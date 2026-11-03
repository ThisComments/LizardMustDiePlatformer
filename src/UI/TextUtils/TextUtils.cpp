#include "UI/TextUtils/TextUtils.h"

sf::String ToSfString(const std::string& utf8Text)
{
	return sf::String::fromUtf8(utf8Text.begin(), utf8Text.end());
}

void CenterTextAt(sf::Text& text, const sf::Vector2f center)
{
	const sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin({
		bounds.position.x + bounds.size.x / 2.f,
		bounds.position.y + bounds.size.y / 2.f
	});
	text.setPosition(center);
}

void DrawCenteredText(
	sf::RenderTarget& target,
	const sf::Font& font,
	const std::string& content,
	const unsigned characterSize,
	const sf::Vector2f center,
	const sf::Color color
)
{
	sf::Text text(font, ToSfString(content), characterSize);
	text.setFillColor(color);
	CenterTextAt(text, center);
	target.draw(text);
}
