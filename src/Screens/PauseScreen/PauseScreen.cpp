#include "Screens/PauseScreen/PauseScreen.h"

#include "UI/ScreenLayout.h"
#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

namespace
{
	constexpr std::size_t ITEM_RESUME = 0;
	constexpr std::size_t ITEM_MENU = 1;
	constexpr const char* TITLE = "ПЕРЕДЫШКА";
	constexpr const char* NOW_PLAYING_LABEL = "СЕЙЧАС ИГРАЕТ ГУСЛЯР";
	constexpr const char* NO_MUSIC_LABEL = "ТИШИНА В ПОХОДЕ";

	float CalculatePauseMenuY(const sf::Vector2f screenSize)
	{
		const sf::Vector2f cardPos = ScreenLayout::CardPosition(
			screenSize,
			ScreenLayout::PAUSE_CARD_SIZE
		);
		return cardPos.y + 320.f;
	}
}

PauseScreen::PauseScreen(const sf::Font& font, const sf::Vector2f screenSize, AudioManager& audio)
	: 	m_font(font),
		m_screenSize(screenSize),
		m_audio(audio),
		m_overlay(screenSize),
		m_menu(
			font,
			{
				{ "ПРОДОЛЖИТЬ СЕЧУ", PS_BUTTON_THEME_SUCCESS },
				{ "ВЕРНУТЬСЯ В ИЗБУ", PS_BUTTON_THEME_DANGER }
			},
			screenSize,
			CalculatePauseMenuY(screenSize)
		)
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

void PauseScreen::Render(sf::RenderWindow& window) const
{
	m_overlay.Render(window);

	const sf::Vector2f cardPos = ScreenLayout::CardPosition(
		m_screenSize,
		ScreenLayout::PAUSE_CARD_SIZE
	);
	DrawCard(window, cardPos, ScreenLayout::PAUSE_CARD_SIZE);

	DrawCenteredText(
		window,
		m_font,
		TITLE,
		UiStyle::CARD_TITLE_FONT_SIZE,
		{ m_screenSize.x / 2.f, cardPos.y + 60.f },
		UiStyle::GOLD_TEXT_COLOR
	);
	DrawDivider(
		window,
		{ m_screenSize.x / 2.f, cardPos.y + 106.f },
		ScreenLayout::PAUSE_CARD_SIZE.x - 80.f
	);

	const sf::Vector2f trackPanelSize = { ScreenLayout::PAUSE_CARD_SIZE.x - 90.f, 150.f };
	const sf::Vector2f trackPanelPos = { cardPos.x + 45.f, cardPos.y + 132.f };
	DrawInsetPanel(
		window,
		trackPanelPos,
		trackPanelSize,
		UiStyle::PANEL_FILL_COLOR,
		UiStyle::PANEL_OUTLINE_COLOR
	);

	if (!m_trackTitle.empty())
	{
		DrawCenteredText(
			window,
			m_font,
			NOW_PLAYING_LABEL,
			UiStyle::HUD_SMALL_FONT_SIZE,
			{ m_screenSize.x / 2.f, trackPanelPos.y + 34.f },
			UiStyle::MUTED_TEXT_COLOR
		);
		DrawCenteredText(
			window,
			m_font,
			m_trackTitle,
			UiStyle::MESSAGE_FONT_SIZE,
			{ m_screenSize.x / 2.f, trackPanelPos.y + 78.f },
			UiStyle::GOLD_TEXT_COLOR
		);
		if (!m_trackArtist.empty())
		{
			DrawCenteredText(
				window,
				m_font,
				m_trackArtist,
				UiStyle::HUD_SMALL_FONT_SIZE,
				{ m_screenSize.x / 2.f, trackPanelPos.y + 118.f },
				UiStyle::TEXT_COLOR
			);
		}
	}
	else
	{
		DrawCenteredText(
			window,
			m_font,
			NO_MUSIC_LABEL,
			UiStyle::MESSAGE_FONT_SIZE,
			trackPanelPos + trackPanelSize / 2.f,
			UiStyle::MUTED_TEXT_COLOR
		);
	}

	m_menu.Render(window);
}

void PauseScreen::OnEnter()
{
	m_menu.Reset();

	m_audio.PauseMusic();
	m_trackTitle = m_audio.GetCurrentTrackTitle();
	m_trackArtist = m_audio.GetCurrentArtist();
}

void PauseScreen::OnExit()
{
	m_audio.ResumeMusic();
}

bool PauseScreen::IsOverlay() const
{
	return true;
}