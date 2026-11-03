#include "UI/HUD/HUD.h"

#include <string>

#include "UI/ScreenLayout.h"
#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

HUD::HUD(const sf::Font& font)
	: 	m_healthText(font, "", UiStyle::HUD_FONT_SIZE),
		m_potionsText(font, "", UiStyle::HUD_FONT_SIZE),
		m_levelText(font, "", UiStyle::HUD_FONT_SIZE)
{
	const sf::Vector2f origin = ScreenLayout::HudPosition();
	m_healthText.setPosition(origin);
	m_potionsText.setPosition({origin.x, origin.y + UiStyle::HUD_LINE_SPACING});
	m_levelText.setPosition({origin.x, origin.y + 2.f * UiStyle::HUD_LINE_SPACING});

	m_healthText.setFillColor(UiStyle::HUD_TEXT_COLOR);
	m_potionsText.setFillColor(UiStyle::HUD_TEXT_COLOR);
	m_levelText.setFillColor(UiStyle::HUD_TEXT_COLOR);
}

void HUD::SetData(const HudData& data)
{
	m_healthText.setString(ToSfString(
		"Health: " + std::to_string(data.health) + " / " + std::to_string(data.maxHealth)
	));
	m_potionsText.setString(ToSfString("Potions: " + std::to_string(data.potions)));
	m_levelText.setString(ToSfString("Level " + std::to_string(data.level)));
}

void HUD::Render(sf::RenderTarget& target) const
{
	target.setView(target.getDefaultView());
	target.draw(m_healthText);
	target.draw(m_potionsText);
	target.draw(m_levelText);
}
