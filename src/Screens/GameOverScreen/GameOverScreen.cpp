#include "Screens/GameOverScreen/GameOverScreen.h"

#include "UI/ScreenLayout.h"
#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

namespace
{
	constexpr std::size_t ITEM_RESTART = 0;
	constexpr std::size_t ITEM_MENU = 1;

	constexpr const char* VICTORY_TITLE = "ЯЩЕРЫ РАЗБИТЫ!";
	constexpr const char* VICTORY_MESSAGE = "Земли русов очищеныn\nот скверны!";
	constexpr const char* DEFEAT_TITLE = "БОГАТЫРЬ ПАЛ";
	constexpr const char* DEFEAT_MESSAGE = "Ящеры одолели дружину\nв этом бою...";
	constexpr const char* HINT = "R — новый поход    Esc — вернуться в избу";

	float CalculateGameOverMenuY(const sf::Vector2f screenSize)
	{
		const sf::Vector2f cardPos = ScreenLayout::CardPosition(
			screenSize,
			ScreenLayout::GAME_OVER_CARD_SIZE
		);
		return cardPos.y + 275.f;
	}
}

GameOverScreen::GameOverScreen(
	const sf::Font& font,
	const sf::Vector2f screenSize,
	const GameOverReason reason,
	AudioManager& audio
)
	: 	m_font(font),
		m_screenSize(screenSize),
		m_overlay(screenSize),
		m_audio(audio),
		m_menu(
			font,
			{
				{
					reason == GameOverReason::Victory ? "НОВЫЙ ПОХОД" : "ОТОМСТИТЬ ЯЩЕРАМ",
					reason == GameOverReason::Victory ? PS_BUTTON_THEME_GOLD : PS_BUTTON_THEME_SUCCESS
				},
				{ "ВЫЙТИ В ИЗБУ", PS_BUTTON_THEME_DANGER }
			},
			screenSize,
			CalculateGameOverMenuY(screenSize)
		),
		m_reason(reason)
{
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

void GameOverScreen::Render(sf::RenderWindow& window) const
{
	const bool isVictory = m_reason == GameOverReason::Victory;

	m_overlay.Render(window);

	const sf::Vector2f cardPos = ScreenLayout::CardPosition(
		m_screenSize,
		ScreenLayout::GAME_OVER_CARD_SIZE
	);
	DrawCard(window, cardPos, ScreenLayout::GAME_OVER_CARD_SIZE);

	DrawCenteredText(
		window,
		m_font,
		isVictory ? VICTORY_TITLE : DEFEAT_TITLE,
		UiStyle::CARD_TITLE_FONT_SIZE,
		{ m_screenSize.x / 2.f, cardPos.y + 62.f },
		isVictory ? UiStyle::VICTORY_COLOR : UiStyle::DEFEAT_COLOR
	);
	DrawDivider(
		window,
		{ m_screenSize.x / 2.f, cardPos.y + 108.f },
		ScreenLayout::GAME_OVER_CARD_SIZE.x - 80.f
	);

	const sf::Vector2f bannerSize = { ScreenLayout::GAME_OVER_CARD_SIZE.x - 90.f, 105.f };
	const sf::Vector2f bannerPos = { cardPos.x + 45.f, cardPos.y + 135.f };
	DrawInsetPanel(
		window,
		bannerPos,
		bannerSize,
		UiStyle::PANEL_FILL_COLOR,
		isVictory ? UiStyle::CARD_OUTLINE_COLOR : UiStyle::DEFEAT_COLOR
	);

	DrawCenteredText(
		window,
		m_font,
		isVictory ? VICTORY_MESSAGE : DEFEAT_MESSAGE,
		UiStyle::MESSAGE_FONT_SIZE,
		bannerPos + bannerSize / 2.f,
		isVictory ? UiStyle::TEXT_COLOR : UiStyle::DEFEAT_MESSAGE_COLOR
	);

	m_menu.Render(window);

	DrawCenteredText(
		window,
		m_font,
		HINT,
		UiStyle::HINT_FONT_SIZE,
		{ m_screenSize.x / 2.f, cardPos.y + ScreenLayout::GAME_OVER_CARD_SIZE.y + 45.f },
		UiStyle::MUTED_TEXT_COLOR
	);
}

void GameOverScreen::OnEnter()
{
	m_menu.Reset();
	m_audio.StopMusic();
}

bool GameOverScreen::IsOverlay() const
{
	return true;
}