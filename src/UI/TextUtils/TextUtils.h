#pragma once

#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>

sf::String ToSfString(const std::string& utf8Text);
void CenterTextAt(sf::Text& text, sf::Vector2f center);
void AlignTextLeftCenter(sf::Text& text, sf::Vector2f leftCenter);
void DrawCenteredText(
	sf::RenderTarget& target,
	const sf::Font& font,
	const std::string& content,
	unsigned characterSize,
	sf::Vector2f center,
	sf::Color color
);
void DrawShadowedCenteredText(
	sf::RenderTarget& target,
	const sf::Font& font,
	const std::string& content,
	unsigned characterSize,
	sf::Vector2f center,
	sf::Color color,
	sf::Color shadowColor
);
void DrawTextWithShadow(
	sf::RenderTarget& target,
	const sf::Text& text,
	sf::Color shadowColor,
	sf::Vector2f shadowOffset
);
void DrawCard(sf::RenderTarget& target, sf::Vector2f position, sf::Vector2f size);
void DrawInsetPanel(
	sf::RenderTarget& target,
	sf::Vector2f position,
	sf::Vector2f size,
	sf::Color fillColor,
	sf::Color outlineColor
);
void DrawDivider(sf::RenderTarget& target, sf::Vector2f center, float width);
void DrawPatternedBackground(sf::RenderTarget& target, sf::Vector2f screenSize);