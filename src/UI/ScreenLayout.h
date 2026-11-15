#pragma once

#include <cstddef>

#include <SFML/System/Vector2.hpp>

#include "UI/UiStyle.h"

namespace ScreenLayout
{
	constexpr float TITLE_HEIGHT_RATIO = 0.22f;
	constexpr float MESSAGE_HEIGHT_RATIO = 0.32f;
	constexpr float MENU_TOP_HEIGHT_RATIO = 0.45f;
	constexpr float NOW_PLAYING_LABEL_HEIGHT_RATIO = 0.31f;
	constexpr float TRACK_TITLE_HEIGHT_RATIO = 0.36f;
	constexpr float TRACK_ARTIST_HEIGHT_RATIO = 0.41f;
	constexpr float HINT_MARGIN_SCALE = 2.f;

	inline sf::Vector2f TitleCenter(const sf::Vector2f screenSize)
	{
		return {screenSize.x / 2.f, screenSize.y * TITLE_HEIGHT_RATIO};
	}

	inline sf::Vector2f MessageCenter(const sf::Vector2f screenSize)
	{
		return {screenSize.x / 2.f, screenSize.y * MESSAGE_HEIGHT_RATIO};
	}

	inline sf::Vector2f NowPlayingLabelCenter(const sf::Vector2f screenSize)
	{
		return {screenSize.x / 2.f, screenSize.y * NOW_PLAYING_LABEL_HEIGHT_RATIO};
	}

	inline sf::Vector2f TrackTitleCenter(const sf::Vector2f screenSize)
	{
		return {screenSize.x / 2.f, screenSize.y * TRACK_TITLE_HEIGHT_RATIO};
	}

	inline sf::Vector2f TrackArtistCenter(const sf::Vector2f screenSize)
	{
		return {screenSize.x / 2.f, screenSize.y * TRACK_ARTIST_HEIGHT_RATIO};
	}

	inline sf::Vector2f HintCenter(const sf::Vector2f screenSize)
	{
		return {screenSize.x / 2.f, screenSize.y - UiStyle::SCREEN_MARGIN * HINT_MARGIN_SCALE};
	}

	inline sf::Vector2f ButtonPosition(const sf::Vector2f screenSize, const std::size_t index)
	{
		return {
			(screenSize.x - UiStyle::BUTTON_SIZE.x) / 2.f,
			screenSize.y * MENU_TOP_HEIGHT_RATIO +
				static_cast<float>(index) * (UiStyle::BUTTON_SIZE.y + UiStyle::BUTTON_SPACING)
		};
	}

	inline sf::Vector2f HudPosition()
	{
		return {UiStyle::SCREEN_MARGIN, UiStyle::SCREEN_MARGIN};
	}
}
