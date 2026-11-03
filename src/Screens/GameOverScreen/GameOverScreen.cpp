#include "Screens/GameOverScreen/GameOverScreen.h"

#include "UI/ScreenLayout.h"
#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

namespace
{
	constexpr std::size_t ITEM_RESTART = 0;
	constexpr std::size_t ITEM_MENU = 1;
	constexpr const char* VICTORY_TITLE = "ПОБЕДА!";
	constexpr const char* VICTORY_MESSAGE = "Уровень пройден";
	constexpr const char* DEFEAT_TITLE = "ПОРАЖЕНИЕ";
	constexpr const char* DEFEAT_MESSAGE = "Вы упали в пропасть";
	constexpr const char* HINT = "R - попробовать снова    Esc - изба";
}

GameOverScreen::GameOverScreen(const sf::Font& font, const sf::Vector2f screenSize)
	: 	m_font(font),
		m_screenSize(screenSize),
		m_overlay(screenSize),
		m_menu(font, {"Ещё раз", "Вернуться в избу"}, screenSize),
		m_reason(GameOverReason::Defeat)
{
}

void GameOverScreen::SetReason(const GameOverReason reason)
{
	m_reason = reason;
}

ScreenCommand GameOverScreen::HandleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->code == sf::Keyboard::Key::R)
		{
			return ScreenCommand::Restart;
		}

		if (keyPressed->code == sf::Keyboard::Key::Escape)
		{
			return ScreenCommand::ReturnToMenu;
		}
	}

	const std::optional<std::size_t> activated = m_menu.HandleEvent(event, window);
	if (!activated)
	{
		return ScreenCommand::None;
	}

	if (*activated == ITEM_RESTART)
	{
		return ScreenCommand::Restart;
	}

	if (*activated == ITEM_MENU)
	{
		return ScreenCommand::ReturnToMenu;
	}

	return ScreenCommand::None;
}

ScreenCommand GameOverScreen::Update(const float)
{
	return ScreenCommand::None;
}

void GameOverScreen::Render(sf::RenderWindow& window)
{
	const bool isVictory = m_reason == GameOverReason::Victory;

	m_overlay.Render(window);
	DrawCenteredText(
		window,
		m_font,
		isVictory ? VICTORY_TITLE : DEFEAT_TITLE,
		UiStyle::TITLE_FONT_SIZE,
		ScreenLayout::TitleCenter(m_screenSize),
		isVictory ? UiStyle::VICTORY_COLOR : UiStyle::DEFEAT_COLOR
	);
	DrawCenteredText(
		window,
		m_font,
		isVictory ? VICTORY_MESSAGE : DEFEAT_MESSAGE,
		UiStyle::MESSAGE_FONT_SIZE,
		ScreenLayout::MessageCenter(m_screenSize),
		UiStyle::TEXT_COLOR
	);
	m_menu.Render(window);
	DrawCenteredText(
		window,
		m_font,
		HINT,
		UiStyle::HINT_FONT_SIZE,
		ScreenLayout::HintCenter(m_screenSize),
		UiStyle::TEXT_COLOR
	);
}

void GameOverScreen::OnEnter()
{
	m_menu.Reset();
}

bool GameOverScreen::IsOverlay() const
{
	return true;
}
