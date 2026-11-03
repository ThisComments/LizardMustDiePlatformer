#include "Screens/PauseScreen/PauseScreen.h"

#include "UI/ScreenLayout.h"
#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

namespace
{
	constexpr std::size_t ITEM_RESUME = 0;
	constexpr std::size_t ITEM_MENU = 1;
	constexpr const char* TITLE = "ПЕРЕДЫШКА";
}

PauseScreen::PauseScreen(const sf::Font& font, const sf::Vector2f screenSize)
	: 	m_font(font),
		m_screenSize(screenSize),
		m_overlay(screenSize),
		m_menu(font, {"В бой", "Вернутся в избу"}, screenSize)
{
}

ScreenCommand PauseScreen::HandleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
	if (keyPressed && keyPressed->code == sf::Keyboard::Key::Escape)
	{
		return ScreenCommand::Resume;
	}

	const std::optional<std::size_t> activated = m_menu.HandleEvent(event, window);
	if (!activated)
	{
		return ScreenCommand::None;
	}

	if (*activated == ITEM_RESUME)
	{
		return ScreenCommand::Resume;
	}

	if (*activated == ITEM_MENU)
	{
		return ScreenCommand::ReturnToMenu;
	}

	return ScreenCommand::None;
}

ScreenCommand PauseScreen::Update(const float)
{
	return ScreenCommand::None;
}

void PauseScreen::Render(sf::RenderWindow& window)
{
	m_overlay.Render(window);
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

void PauseScreen::OnEnter()
{
	m_menu.Reset();
}

bool PauseScreen::IsOverlay() const
{
	return true;
}
