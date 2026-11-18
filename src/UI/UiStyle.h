#pragma once

#include <cstdint>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

enum ButtonTheme
{
	PS_BUTTON_THEME_PRIMARY,
	PS_BUTTON_THEME_SUCCESS,
	PS_BUTTON_THEME_ALT,
	PS_BUTTON_THEME_DANGER,
	PS_BUTTON_THEME_GOLD
};

struct ButtonPalette
{
	sf::Color fill;
	sf::Color hoverFill;
	sf::Color highlightBevel;
	sf::Color shadowBevel;
};

namespace UiStyle
{
	constexpr sf::Vector2f BUTTON_SIZE = { 480.f, 68.f };
	constexpr float BUTTON_SPACING = 20.f;
	constexpr float BUTTON_OUTLINE_THICKNESS = 3.f;
	constexpr float BUTTON_BEVEL_SIZE = 4.f;
	constexpr float BUTTON_PRESS_OFFSET = 3.f;

	constexpr float CARD_OUTLINE_THICKNESS = 4.f;
	constexpr float CARD_BEVEL_SIZE = 3.f;
	constexpr sf::Vector2f CARD_SHADOW_OFFSET = { 0.f, 10.f };
	constexpr float PANEL_OUTLINE_THICKNESS = 3.f;
	constexpr float DIVIDER_THICKNESS = 2.f;

	constexpr float SCREEN_MARGIN = 22.f;
	constexpr float GRID_CELL_WIDTH = 32.f;
	constexpr float GRID_CELL_HEIGHT = 48.f;
	constexpr float GRID_LINE_THICKNESS = 2.f;

	constexpr sf::Vector2f HUD_PANEL_SIZE = { 416.f, 128.f };
	constexpr sf::Vector2f HEALTH_BAR_SIZE = { 236.f, 36.f };
	constexpr float HEALTH_BAR_OUTLINE_THICKNESS = 3.f;
	constexpr sf::Vector2f HUD_GOLD_BOX_SIZE = { 138.f, 36.f };
	constexpr sf::Vector2f HUD_ABILITY_BOX_SIZE = { 186.f, 54.f };
	constexpr sf::Vector2f HUD_SLOT_SIZE = { 42.f, 42.f };
	constexpr float COIN_ICON_RADIUS = 10.f;

	constexpr unsigned LOGO_FONT_SIZE = 50;
	constexpr unsigned SUBTITLE_FONT_SIZE = 20;
	constexpr unsigned CARD_TITLE_FONT_SIZE = 30;
	constexpr unsigned MESSAGE_FONT_SIZE = 20;
	constexpr unsigned HINT_FONT_SIZE = 18;
	constexpr unsigned BUTTON_FONT_SIZE = 20;
	constexpr unsigned HUD_FONT_SIZE = 20;
	constexpr unsigned HUD_SMALL_FONT_SIZE = 16;
	constexpr unsigned SLOT_KEY_FONT_SIZE = 14;

	constexpr std::uint8_t OVERLAY_ALPHA = 195;
	constexpr float TEXT_SHADOW_OFFSET = 3.f;

	constexpr sf::Color SCREEN_BACKGROUND_COLOR(26, 13, 7);
	constexpr sf::Color SCREEN_GRID_COLOR(21, 10, 4);

	constexpr sf::Color CARD_FILL_COLOR(43, 25, 16);
	constexpr sf::Color CARD_OUTLINE_COLOR(139, 90, 43);
	constexpr sf::Color CARD_HIGHLIGHT_BEVEL(77, 48, 28);
	constexpr sf::Color CARD_SHADOW_BEVEL(18, 9, 4);
	constexpr sf::Color CARD_DROP_SHADOW_COLOR(0, 0, 0, 165);

	constexpr sf::Color PANEL_FILL_COLOR(18, 9, 4);
	constexpr sf::Color PANEL_ALT_FILL_COLOR(33, 19, 10);
	constexpr sf::Color PANEL_OUTLINE_COLOR(60, 36, 21);
	constexpr sf::Color DIVIDER_COLOR(92, 61, 36);

	constexpr sf::Color TEXT_COLOR(243, 229, 171);
	constexpr sf::Color MUTED_TEXT_COLOR(189, 195, 199);
	constexpr sf::Color GOLD_TEXT_COLOR(212, 175, 55);
	constexpr sf::Color SELECTED_TEXT_COLOR(255, 235, 130);
	constexpr sf::Color TEXT_SHADOW_COLOR(28, 14, 7);

	constexpr sf::Color VICTORY_COLOR(110, 215, 85);
	constexpr sf::Color DEFEAT_COLOR(205, 60, 50);
	constexpr sf::Color DEFEAT_MESSAGE_COLOR(255, 153, 153);

	constexpr sf::Color BUTTON_OUTLINE_COLOR(28, 14, 7);
	constexpr sf::Color BUTTON_SELECTED_OUTLINE_COLOR(212, 175, 55);

	constexpr sf::Color HEALTH_BAR_BACKGROUND_COLOR(18, 9, 4);
	constexpr sf::Color HEALTH_BAR_FILL_COLOR(163, 44, 36);
	constexpr sf::Color HEALTH_BAR_HIGHLIGHT_COLOR(205, 75, 65);
	constexpr sf::Color HEALTH_BAR_OUTLINE_COLOR(92, 61, 36);

	constexpr sf::Color HUD_SLOT_FILL_COLOR(12, 6, 3);
	constexpr sf::Color HUD_SLOT_ACTIVE_FILL_COLOR(48, 36, 14);
	constexpr sf::Color HUD_SLOT_OUTLINE_COLOR(92, 61, 36);
	constexpr sf::Color HUD_SLOT_ACTIVE_OUTLINE_COLOR(212, 175, 55);

	constexpr sf::Color ABILITY_READY_COLOR(115, 220, 90);
	constexpr sf::Color ABILITY_COOLDOWN_COLOR(189, 195, 199);

	inline ButtonPalette GetButtonPalette(const ButtonTheme theme)
	{
		switch (theme)
		{
			case PS_BUTTON_THEME_PRIMARY:
				return {
					sf::Color(160, 72, 34),
					sf::Color(207, 110, 62),
					sf::Color(225, 135, 85),
					sf::Color(96, 37, 13)
				};
			case PS_BUTTON_THEME_SUCCESS:
				return {
					sf::Color(46, 76, 31),
					sf::Color(77, 122, 57),
					sf::Color(105, 160, 80),
					sf::Color(24, 40, 16)
				};
			case PS_BUTTON_THEME_ALT:
				return {
					sf::Color(26, 60, 84),
					sf::Color(42, 97, 135),
					sf::Color(70, 130, 175),
					sf::Color(11, 29, 43)
				};
			case PS_BUTTON_THEME_DANGER:
				return {
					sf::Color(112, 22, 16),
					sf::Color(163, 44, 36),
					sf::Color(200, 70, 60),
					sf::Color(56, 8, 5)
				};
			case PS_BUTTON_THEME_GOLD:
				return {
					sf::Color(184, 134, 11),
					sf::Color(212, 175, 55),
					sf::Color(238, 203, 95),
					sf::Color(92, 61, 36)
				};
		}

		return {
			sf::Color(160, 72, 34),
			sf::Color(207, 110, 62),
			sf::Color(225, 135, 85),
			sf::Color(96, 37, 13)
		};
	}
}