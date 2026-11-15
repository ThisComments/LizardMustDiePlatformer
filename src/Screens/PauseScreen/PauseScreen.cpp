#include "Screens/PauseScreen/PauseScreen.h"

#include "UI/ScreenLayout.h"
#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

namespace
{
	constexpr std::size_t ITEM_RESUME = 0;
	constexpr std::size_t ITEM_MENU = 1;
	constexpr const char* TITLE = "ПЕРЕДЫШКА";
	constexpr const char* NOW_PLAYING_LABEL = "СЕЙЧАС ИГРАЕТ";
}

PauseScreen::PauseScreen(const sf::Font& font, const sf::Vector2f screenSize, AudioManager& audio)
	: 	m_font(font),
		m_screenSize(screenSize),
		m_audio(audio),
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

void PauseScreen::Render(sf::RenderWindow& window) const
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
	if (!m_trackTitle.empty())
	{
		DrawCenteredText(
			window,
			m_font,
			NOW_PLAYING_LABEL,
			UiStyle::HINT_FONT_SIZE,
			ScreenLayout::NowPlayingLabelCenter(m_screenSize),
			UiStyle::TEXT_COLOR
		);
		DrawCenteredText(
			window,
			m_font,
			m_trackTitle,
			UiStyle::MESSAGE_FONT_SIZE,
			ScreenLayout::TrackTitleCenter(m_screenSize),
			UiStyle::SELECTED_TEXT_COLOR
		);
		if (!m_trackArtist.empty())
		{
			DrawCenteredText(
				window,
				m_font,
				m_trackArtist,
				UiStyle::HINT_FONT_SIZE,
				ScreenLayout::TrackArtistCenter(m_screenSize),
				UiStyle::TEXT_COLOR
			);
		}
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
