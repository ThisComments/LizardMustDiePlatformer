#pragma once

#include <cstdint>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace UiStyle
{
	constexpr sf::Vector2f BUTTON_SIZE = {520.f, 84.f};
	constexpr float BUTTON_SPACING = 28.f;
	constexpr float BUTTON_OUTLINE_THICKNESS = 4.f;
	constexpr float SCREEN_MARGIN = 30.f;

	constexpr unsigned TITLE_FONT_SIZE = 96;
	constexpr unsigned MESSAGE_FONT_SIZE = 40;
	constexpr unsigned HINT_FONT_SIZE = 28;
	constexpr unsigned BUTTON_FONT_SIZE = 40;
	constexpr unsigned HUD_FONT_SIZE = 32;
	constexpr float HUD_LINE_SPACING = 44.f;

	constexpr std::uint8_t OVERLAY_ALPHA = 170;

	constexpr sf::Color SCREEN_BACKGROUND_COLOR(20, 24, 38);
	constexpr sf::Color TEXT_COLOR(235, 235, 235);
	constexpr sf::Color SELECTED_TEXT_COLOR(255, 210, 60);
	constexpr sf::Color VICTORY_COLOR(120, 230, 120);
	constexpr sf::Color DEFEAT_COLOR(235, 80, 80);
	constexpr sf::Color HUD_TEXT_COLOR(255, 255, 255);

	constexpr sf::Color BUTTON_FILL_COLOR(45, 52, 74);
	constexpr sf::Color BUTTON_SELECTED_FILL_COLOR(70, 82, 120);
	constexpr sf::Color BUTTON_PRESSED_FILL_COLOR(35, 40, 60);
	constexpr sf::Color BUTTON_OUTLINE_COLOR(90, 100, 130);
	constexpr sf::Color BUTTON_SELECTED_OUTLINE_COLOR(255, 210, 60);
}
