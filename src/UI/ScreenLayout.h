#pragma once

#include <cstddef>

#include <SFML/System/Vector2.hpp>

#include "UI/UiStyle.h"

namespace ScreenLayout
{
	constexpr sf::Vector2f MAIN_MENU_CARD_SIZE = { 620.f, 520.f };
	constexpr sf::Vector2f PAUSE_CARD_SIZE = { 620.f, 540.f };
	constexpr sf::Vector2f GAME_OVER_CARD_SIZE = { 640.f, 520.f };
	constexpr sf::Vector2f HOW_TO_PLAY_CARD_SIZE = { 820.f, 680.f };

	inline sf::Vector2f CardPosition(const sf::Vector2f screenSize, const sf::Vector2f cardSize)
	{
		return {
			(screenSize.x - cardSize.x) / 2.f,
			(screenSize.y - cardSize.y) / 2.f
		};
	}

	inline sf::Vector2f LogoCenter(const sf::Vector2f screenSize)
	{
		return { screenSize.x / 2.f, screenSize.y * 0.14f };
	}

	inline sf::Vector2f SubtitleCenter(const sf::Vector2f screenSize)
	{
		return { screenSize.x / 2.f, screenSize.y * 0.20f };
	}

	inline sf::Vector2f ButtonPosition(
		const sf::Vector2f screenSize,
		const std::size_t index,
		const float startY
	)
	{
		return {
			(screenSize.x - UiStyle::BUTTON_SIZE.x) / 2.f,
			startY + static_cast<float>(index) * (UiStyle::BUTTON_SIZE.y + UiStyle::BUTTON_SPACING)
		};
	}

	inline sf::Vector2f HudPosition()
	{
		return { UiStyle::SCREEN_MARGIN, UiStyle::SCREEN_MARGIN };
	}
}
