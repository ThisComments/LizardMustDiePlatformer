#include "UI/HUD/HUD.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

#include "UI/ScreenLayout.h"
#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

namespace
{
	constexpr float PANEL_INNER_PADDING = 16.f;
	constexpr float ELEMENT_GAP = 12.f;
	constexpr float ROW_ONE_OFFSET_Y = 16.f;
	constexpr float ROW_TWO_OFFSET_Y = 60.f;
	constexpr float HEALTH_HIGHLIGHT_HEIGHT = 6.f;
	constexpr float SLOT_INSET = 6.f;
}

HUD::AbilitySlotWidget::AbilitySlotWidget(
	const sf::Font& font,
	const std::string& keyLabel,
	const std::string& titleLabel,
	const sf::Vector2f position
)
	: 	box(UiStyle::HUD_ABILITY_BOX_SIZE),
		keySlot(UiStyle::HUD_SLOT_SIZE),
		keyText(font, ToSfString(keyLabel), UiStyle::SLOT_KEY_FONT_SIZE),
		titleText(font, ToSfString(titleLabel), UiStyle::SLOT_KEY_FONT_SIZE),
		statusText(font, "", UiStyle::HUD_SMALL_FONT_SIZE)
{
	box.setPosition(position);
	box.setFillColor(UiStyle::PANEL_FILL_COLOR);
	box.setOutlineThickness(-2.f);
	box.setOutlineColor(UiStyle::PANEL_OUTLINE_COLOR);

	keySlot.setPosition({ position.x + SLOT_INSET, position.y + SLOT_INSET });
	keySlot.setOutlineThickness(-2.f);

	keyText.setFillColor(UiStyle::GOLD_TEXT_COLOR);
	CenterTextAt(keyText, keySlot.getPosition() + keySlot.getSize() / 2.f);

	titleText.setFillColor(UiStyle::MUTED_TEXT_COLOR);
	AlignTextLeftCenter(
		titleText,
		{ position.x + SLOT_INSET + UiStyle::HUD_SLOT_SIZE.x + 10.f, position.y + 16.f }
	);
}

void HUD::AbilitySlotWidget::UpdateCooldown(const float cooldown)
{
	const sf::Vector2f boxPos = box.getPosition();
	const sf::Vector2f statusPos = {
		boxPos.x + SLOT_INSET + UiStyle::HUD_SLOT_SIZE.x + 10.f,
		boxPos.y + 36.f
	};

	if (cooldown <= 0.f)
	{
		keySlot.setFillColor(UiStyle::HUD_SLOT_ACTIVE_FILL_COLOR);
		keySlot.setOutlineColor(UiStyle::HUD_SLOT_ACTIVE_OUTLINE_COLOR);
		statusText.setString(ToSfString("ГОТОВО"));
		statusText.setFillColor(UiStyle::ABILITY_READY_COLOR);
	}
	else
	{
		std::ostringstream stream;
		stream << std::fixed << std::setprecision(1) << cooldown << " с";
		keySlot.setFillColor(UiStyle::HUD_SLOT_FILL_COLOR);
		keySlot.setOutlineColor(UiStyle::HUD_SLOT_OUTLINE_COLOR);
		statusText.setString(ToSfString(stream.str()));
		statusText.setFillColor(UiStyle::ABILITY_COOLDOWN_COLOR);
	}

	AlignTextLeftCenter(statusText, statusPos);
}

void HUD::AbilitySlotWidget::Render(sf::RenderTarget& target) const
{
	target.draw(box);
	target.draw(keySlot);
	DrawTextWithShadow(target, keyText, UiStyle::TEXT_SHADOW_COLOR, { 1.f, 1.f });
	DrawTextWithShadow(target, titleText, UiStyle::TEXT_SHADOW_COLOR, { 1.f, 1.f });
	DrawTextWithShadow(target, statusText, UiStyle::TEXT_SHADOW_COLOR, { 1.f, 1.f });
}

HUD::HUD(const sf::Font& font)
	: 	m_panelPosition(ScreenLayout::HudPosition()),
		m_healthBarBackground(UiStyle::HEALTH_BAR_SIZE),
		m_healthBarFill(UiStyle::HEALTH_BAR_SIZE),
		m_healthBarHighlight({ UiStyle::HEALTH_BAR_SIZE.x, HEALTH_HIGHLIGHT_HEIGHT }),
		m_goldBox(UiStyle::HUD_GOLD_BOX_SIZE),
		m_coinOuter(UiStyle::COIN_ICON_RADIUS),
		m_coinInner(UiStyle::COIN_ICON_RADIUS - 3.f),
		m_healthText(font, "", UiStyle::HUD_SMALL_FONT_SIZE),
		m_goldValueText(font, "0", UiStyle::HUD_FONT_SIZE),
		m_healWidget(
			font,
			"Q",
			"ВОДИЦА",
			{ m_panelPosition.x + PANEL_INNER_PADDING, m_panelPosition.y + ROW_TWO_OFFSET_Y }
		),
		m_dashWidget(
			font,
			"SHF",
			"РЫВОК",
			{
				m_panelPosition.x + PANEL_INNER_PADDING + UiStyle::HUD_ABILITY_BOX_SIZE.x + ELEMENT_GAP,
				m_panelPosition.y + ROW_TWO_OFFSET_Y
			}
		)
{
	const sf::Vector2f healthBarPos = {
		m_panelPosition.x + PANEL_INNER_PADDING,
		m_panelPosition.y + ROW_ONE_OFFSET_Y
	};

	m_healthBarBackground.setPosition(healthBarPos);
	m_healthBarBackground.setFillColor(UiStyle::HEALTH_BAR_BACKGROUND_COLOR);
	m_healthBarBackground.setOutlineThickness(-UiStyle::HEALTH_BAR_OUTLINE_THICKNESS);
	m_healthBarBackground.setOutlineColor(UiStyle::HEALTH_BAR_OUTLINE_COLOR);

	m_healthBarFill.setPosition(healthBarPos);
	m_healthBarFill.setFillColor(UiStyle::HEALTH_BAR_FILL_COLOR);

	m_healthBarHighlight.setPosition(healthBarPos);
	m_healthBarHighlight.setFillColor(UiStyle::HEALTH_BAR_HIGHLIGHT_COLOR);

	m_healthText.setFillColor(UiStyle::TEXT_COLOR);

	const sf::Vector2f goldBoxPos = {
		healthBarPos.x + UiStyle::HEALTH_BAR_SIZE.x + ELEMENT_GAP,
		healthBarPos.y
	};
	m_goldBox.setPosition(goldBoxPos);
	m_goldBox.setFillColor(UiStyle::PANEL_FILL_COLOR);
	m_goldBox.setOutlineThickness(-2.f);
	m_goldBox.setOutlineColor(UiStyle::PANEL_OUTLINE_COLOR);

	const sf::Vector2f coinCenter = {
		goldBoxPos.x + 22.f,
		goldBoxPos.y + UiStyle::HUD_GOLD_BOX_SIZE.y / 2.f
	};
	m_coinOuter.setOrigin({ UiStyle::COIN_ICON_RADIUS, UiStyle::COIN_ICON_RADIUS });
	m_coinOuter.setPosition(coinCenter);
	m_coinOuter.setFillColor(UiStyle::GOLD_TEXT_COLOR);
	m_coinOuter.setOutlineThickness(-2.f);
	m_coinOuter.setOutlineColor(UiStyle::CARD_OUTLINE_COLOR);

	m_coinInner.setOrigin({ UiStyle::COIN_ICON_RADIUS - 3.f, UiStyle::COIN_ICON_RADIUS - 3.f });
	m_coinInner.setPosition(coinCenter);
	m_coinInner.setFillColor(UiStyle::SELECTED_TEXT_COLOR);

	m_goldValueText.setFillColor(UiStyle::GOLD_TEXT_COLOR);

	Update(WorldStats{});
}

void HUD::Update(const WorldStats& stats)
{
	const int maxHealth = std::max(1, stats.playerMaxHealth);
	const int currentHealth = std::clamp(stats.playerHealth, 0, maxHealth);
	const float healthRatio = static_cast<float>(currentHealth) / static_cast<float>(maxHealth);
	const float fillWidth = UiStyle::HEALTH_BAR_SIZE.x * healthRatio;

	m_healthBarFill.setSize({ fillWidth, UiStyle::HEALTH_BAR_SIZE.y });
	m_healthBarHighlight.setSize({ fillWidth, HEALTH_HIGHLIGHT_HEIGHT });

	m_healthText.setString(
		ToSfString("ХП: " + std::to_string(currentHealth) + " / " + std::to_string(maxHealth))
	);
	CenterTextAt(m_healthText, m_healthBarBackground.getPosition() + UiStyle::HEALTH_BAR_SIZE / 2.f);

	m_goldValueText.setString(ToSfString(std::to_string(stats.score)));
	AlignTextLeftCenter(
		m_goldValueText,
		{
			m_goldBox.getPosition().x + 42.f,
			m_goldBox.getPosition().y + UiStyle::HUD_GOLD_BOX_SIZE.y / 2.f
		}
	);

	m_healWidget.UpdateCooldown(stats.healCooldown);
	m_dashWidget.UpdateCooldown(stats.dashCooldown);
}

void HUD::Render(sf::RenderTarget& target) const
{
	DrawCard(target, m_panelPosition, UiStyle::HUD_PANEL_SIZE);

	target.draw(m_healthBarBackground);
	target.draw(m_healthBarFill);
	target.draw(m_healthBarHighlight);
	DrawTextWithShadow(target, m_healthText, UiStyle::TEXT_SHADOW_COLOR, { 2.f, 2.f });

	target.draw(m_goldBox);
	target.draw(m_coinOuter);
	target.draw(m_coinInner);
	DrawTextWithShadow(target, m_goldValueText, UiStyle::TEXT_SHADOW_COLOR, { 2.f, 2.f });

	m_healWidget.Render(target);
	m_dashWidget.Render(target);
}