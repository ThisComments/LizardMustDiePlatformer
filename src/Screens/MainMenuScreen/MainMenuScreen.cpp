#include "Screens/MainMenuScreen/MainMenuScreen.h"

#include "UI/ScreenLayout.h"
#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

namespace
{
	constexpr std::size_t ITEM_START = 0;
	constexpr std::size_t ITEM_HOW_TO_PLAY = 1;
	constexpr std::size_t ITEM_EXIT = 2;

	constexpr const char* LOGO_TITLE = "РУСЫ ПРОТИВ ЯЩЕРОВ";
	constexpr const char* SUBTITLE = "СЛАВЯНСКИЙ ПЛАТФОРМЕР";
	constexpr const char* CARD_HEADER = "КНЯЖЕСКАЯ ИЗБА";
	constexpr const char* FOOTER_HINT = "W / S / Стрелки — выбор    Enter — подтвердить";

	float CalculateMenuStartY(const sf::Vector2f screenSize)
	{
		const sf::Vector2f cardPos = ScreenLayout::CardPosition(
			screenSize,
			ScreenLayout::MAIN_MENU_CARD_SIZE
		);
		return cardPos.y + 150.f;
	}
}

MainMenuScreen::MainMenuScreen(const sf::Font& font, const sf::Vector2f screenSize)
	: 	m_font(font),
		m_screenSize(screenSize),
		m_menu(
			font,
			{
				{ "В БОЙ! РУБИТЬ ЯЩЕРОВ", PS_BUTTON_THEME_SUCCESS },
				{ "УСТАВ РАТНИКА", PS_BUTTON_THEME_ALT },
				{ "ПОКИНУТЬ ИЗБУ", PS_BUTTON_THEME_DANGER }
			},
			screenSize,
			CalculateMenuStartY(screenSize)
		)
{
}

ScreenCommand MainMenuScreen::HandleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	const std::optional<std::size_t> activated = m_menu.HandleEvent(event, window);
	if (!activated)
	{
		return ScreenCommand::None;
	}

	if (*activated == ITEM_START)
	{
		return ScreenCommand::StartGame;
	}

	if (*activated == ITEM_HOW_TO_PLAY)
	{
		return ScreenCommand::HowToPlay;
	}

	if (*activated == ITEM_EXIT)
	{
		return ScreenCommand::Exit;
	}

	return ScreenCommand::None;
}

ScreenCommand MainMenuScreen::Update(const float)
{
	return ScreenCommand::None;
}

void MainMenuScreen::Render(sf::RenderWindow& window) const
{
	window.setView(window.getDefaultView());
	DrawPatternedBackground(window, m_screenSize);

	DrawCenteredText(
		window,
		m_font,
		LOGO_TITLE,
		UiStyle::LOGO_FONT_SIZE,
		ScreenLayout::LogoCenter(m_screenSize),
		UiStyle::GOLD_TEXT_COLOR
	);
	DrawCenteredText(
		window,
		m_font,
		SUBTITLE,
		UiStyle::SUBTITLE_FONT_SIZE,
		ScreenLayout::SubtitleCenter(m_screenSize),
		UiStyle::MUTED_TEXT_COLOR
	);

	const sf::Vector2f cardPos = ScreenLayout::CardPosition(
		m_screenSize,
		ScreenLayout::MAIN_MENU_CARD_SIZE
	);
	DrawCard(window, cardPos, ScreenLayout::MAIN_MENU_CARD_SIZE);

	DrawCenteredText(
		window,
		m_font,
		CARD_HEADER,
		UiStyle::CARD_TITLE_FONT_SIZE,
		{ m_screenSize.x / 2.f, cardPos.y + 62.f },
		UiStyle::GOLD_TEXT_COLOR
	);
	DrawDivider(
		window,
		{ m_screenSize.x / 2.f, cardPos.y + 108.f },
		ScreenLayout::MAIN_MENU_CARD_SIZE.x - 80.f
	);

	m_menu.Render(window);

	DrawCenteredText(
		window,
		m_font,
		FOOTER_HINT,
		UiStyle::HINT_FONT_SIZE,
		{ m_screenSize.x / 2.f, cardPos.y + ScreenLayout::MAIN_MENU_CARD_SIZE.y + 52.f },
		UiStyle::MUTED_TEXT_COLOR
	);
}

void MainMenuScreen::OnEnter()
{
	m_menu.Reset();
}