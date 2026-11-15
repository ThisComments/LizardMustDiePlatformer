#include "UI/HUD/HUD.h"

#include <string>

#include "UI/ScreenLayout.h"
#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

HUD::HUD(const sf::Font& font)
	: m_scoreText(font, "", UiStyle::HUD_FONT_SIZE)
{
	m_scoreText.setPosition(ScreenLayout::HudPosition());
	m_scoreText.setFillColor(UiStyle::HUD_TEXT_COLOR);
	SetScore(0);
}

void HUD::SetScore(const int score)
{
	m_scoreText.setString(ToSfString("Score: " + std::to_string(score)));
}

void HUD::Render(sf::RenderTarget& target) const
{
	target.draw(m_scoreText);
}
