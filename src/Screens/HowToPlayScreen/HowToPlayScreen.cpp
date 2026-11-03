#include "Screens/HowToPlayScreen/HowToPlayScreen.h"

#include "UI/ScreenLayout.h"
#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

namespace
{
	constexpr std::size_t ITEM_RETURN_TO_MENU = 0;
	constexpr const char* TITLE = "РУСЫ ПРОТИВ ЯЩЕРОВ";
    constexpr const char* HINT = "W/Space - прыжок \nA/D - Лево/Право\nShift - дэш";
}

HowToPlayScreen::HowToPlayScreen(const sf::Font& font, const sf::Vector2f screenSize)
	: 	m_font(font),
		m_screenSize(screenSize),
		m_menu(font, {"В избу"}, screenSize)
{
}

ScreenCommand HowToPlayScreen::HandleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	const std::optional<std::size_t> activated = m_menu.HandleEvent(event, window);
	if (!activated)
	{
		return ScreenCommand::None;
	}
    
    if (*activated == ITEM_RETURN_TO_MENU)
	{
		return ScreenCommand::ReturnToMenu;
	}

	return ScreenCommand::None;
}

ScreenCommand HowToPlayScreen::Update(const float)
{
	return ScreenCommand::None;
}

void HowToPlayScreen::Render(sf::RenderWindow& window)
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
    DrawCenteredText(
		window,
		m_font,
		HINT,
		UiStyle::MESSAGE_FONT_SIZE,
		ScreenLayout::MessageCenter(m_screenSize),
		UiStyle::TEXT_COLOR
	);
	m_menu.Render(window);
}

void HowToPlayScreen::OnEnter()
{
	m_menu.Reset();
}
