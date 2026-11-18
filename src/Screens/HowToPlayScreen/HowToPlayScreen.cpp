#include "Screens/HowToPlayScreen/HowToPlayScreen.h"

#include <array>

#include "UI/ScreenLayout.h"
#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

namespace
{
	constexpr std::size_t ITEM_RETURN_TO_MENU = 0;
	constexpr const char* CARD_TITLE = "УСТАВ РАТНИКА";
	constexpr const char* SUBTITLE = "ПАМЯТКА ПЕРЕД ПОХОДОМ НА ГИПЕРБОРЕЮ";

	struct ControlRow
	{
		const char* keys;
		const char* description;
	};

	constexpr std::array<ControlRow, 4> CONTROL_ROWS = {{
		{ "A / D  или  Стрелки", "Движение богатыря влево и вправо" },
		{ "W / Space / Вверх", "Богатырский прыжок и двойной прыжок" },
		{ "Left Shift", "Славянский прорыв (стремительный рывок)" },
		{ "Q", "Байкальская водица (исцеление ран)" }
	}};

	float CalculateButtonY(const sf::Vector2f screenSize)
	{
		const sf::Vector2f cardPos = ScreenLayout::CardPosition(
			screenSize,
			ScreenLayout::HOW_TO_PLAY_CARD_SIZE
		);
		return cardPos.y + ScreenLayout::HOW_TO_PLAY_CARD_SIZE.y - UiStyle::BUTTON_SIZE.y - 42.f;
	}
}

HowToPlayScreen::HowToPlayScreen(const sf::Font& font, const sf::Vector2f screenSize)
	: 	m_font(font),
		m_screenSize(screenSize),
		m_menu(
			font,
			{ { "ВЕРНУТЬСЯ В ИЗБУ", PS_BUTTON_THEME_PRIMARY } },
			screenSize,
			CalculateButtonY(screenSize)
		)
{
}

ScreenCommand HowToPlayScreen::HandleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
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

void HowToPlayScreen::Render(sf::RenderWindow& window) const
{
	window.setView(window.getDefaultView());
	DrawPatternedBackground(window, m_screenSize);

	const sf::Vector2f cardPos = ScreenLayout::CardPosition(
		m_screenSize,
		ScreenLayout::HOW_TO_PLAY_CARD_SIZE
	);
	DrawCard(window, cardPos, ScreenLayout::HOW_TO_PLAY_CARD_SIZE);

	DrawCenteredText(
		window,
		m_font,
		CARD_TITLE,
		UiStyle::CARD_TITLE_FONT_SIZE,
		{ m_screenSize.x / 2.f, cardPos.y + 56.f },
		UiStyle::GOLD_TEXT_COLOR
	);
	DrawCenteredText(
		window,
		m_font,
		SUBTITLE,
		UiStyle::HUD_SMALL_FONT_SIZE,
		{ m_screenSize.x / 2.f, cardPos.y + 98.f },
		UiStyle::MUTED_TEXT_COLOR
	);
	DrawDivider(
		window,
		{ m_screenSize.x / 2.f, cardPos.y + 128.f },
		ScreenLayout::HOW_TO_PLAY_CARD_SIZE.x - 80.f
	);

	const float rowWidth = ScreenLayout::HOW_TO_PLAY_CARD_SIZE.x - 90.f;
	const float rowHeight = 82.f;
	const float rowSpacing = 16.f;
	const float firstRowY = cardPos.y + 154.f;
	const float rowLeft = cardPos.x + 45.f;

	for (std::size_t i = 0; i < CONTROL_ROWS.size(); ++i)
	{
		const float rowY = firstRowY + static_cast<float>(i) * (rowHeight + rowSpacing);
		DrawInsetPanel(
			window,
			{ rowLeft, rowY },
			{ rowWidth, rowHeight },
			UiStyle::PANEL_FILL_COLOR,
			UiStyle::PANEL_OUTLINE_COLOR
		);

		sf::Text keyText(m_font, ToSfString(CONTROL_ROWS[i].keys), UiStyle::MESSAGE_FONT_SIZE);
		keyText.setFillColor(UiStyle::GOLD_TEXT_COLOR);
		AlignTextLeftCenter(keyText, { rowLeft + 24.f, rowY + 28.f });
		DrawTextWithShadow(window, keyText, UiStyle::TEXT_SHADOW_COLOR, { 2.f, 2.f });

		sf::Text descText(m_font, ToSfString(CONTROL_ROWS[i].description), UiStyle::HUD_SMALL_FONT_SIZE);
		descText.setFillColor(UiStyle::TEXT_COLOR);
		AlignTextLeftCenter(descText, { rowLeft + 24.f, rowY + 58.f });
		DrawTextWithShadow(window, descText, UiStyle::TEXT_SHADOW_COLOR, { 2.f, 2.f });
	}

	m_menu.Render(window);
}

void HowToPlayScreen::OnEnter()
{
	m_menu.Reset();
}