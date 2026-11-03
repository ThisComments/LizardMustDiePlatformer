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
void DrawCenteredText(
	sf::RenderTarget& target,
	const sf::Font& font,
	const std::string& content,
	unsigned characterSize,
	sf::Vector2f center,
	sf::Color color
);
