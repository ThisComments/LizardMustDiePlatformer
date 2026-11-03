#include "Screens/MainMenuScreen/MainMenuScreen.h"

#include "UI/ScreenLayout.h"
#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

namespace
{
	constexpr std::size_t ITEM_START = 0;
	constexpr std::size_t ITEM_HOW_TO_PLAY = 1;
	constexpr std::size_t ITEM_EXIT = 2;
	constexpr const char* TITLE = "РУСЫ ПРОТИВ ЯЩЕРОВ";
}

MainMenuScreen::MainMenuScreen(const sf::Font& font, const sf::Vector2f screenSize)
	: 	m_font(font),
		m_screenSize(screenSize),
		m_menu(font, {"Рубить ящеров", "Как мечём махать?", "Демобилизоваться"}, screenSize)
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

void MainMenuScreen::Render(sf::RenderWindow& window)
{
	window.setView(window.getDefaultView());
	window.clear(UiStyle::SCREEN_BACKGROUND_COLOR);

	DrawCenteredText(
		window,
		m_font,
		TITLE,
		UiStyle::TITLE_FONT_SIZE,
		ScreenLayout::TitleCenter(m_screenSize),
		UiStyle::TEXT_COLOR
	);
	m_menu.Render(window);
}

void MainMenuScreen::OnEnter()
{
	m_menu.Reset();
}
