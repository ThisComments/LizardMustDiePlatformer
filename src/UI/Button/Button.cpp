#include "UI/Button/Button.h"

#include "UI/TextUtils/TextUtils.h"

Button::Button(
	const sf::Font& font,
	const std::string& label,
	const sf::Vector2f position,
	const sf::Vector2f size,
	const ButtonTheme theme
)
	: 	m_basePosition(position),
		m_size(size),
		m_theme(theme),
		m_shape(size),
		m_label(font, ToSfString(label), UiStyle::BUTTON_FONT_SIZE),
		m_isSelected(false),
		m_isPressed(false)
{
	m_shape.setOutlineThickness(-UiStyle::BUTTON_OUTLINE_THICKNESS);
	ApplyState();
}

void Button::SetSelected(const bool isSelected)
{
	m_isSelected = isSelected;
	ApplyState();
}

void Button::SetPressed(const bool isPressed)
{
	m_isPressed = isPressed;
	ApplyState();
}

bool Button::Contains(const sf::Vector2f point) const
{
	return m_shape.getGlobalBounds().contains(point);
}

void Button::Render(sf::RenderTarget& target) const
{
	target.draw(m_shape);
	target.draw(m_topBevel);
	target.draw(m_leftBevel);
	target.draw(m_bottomBevel);
	target.draw(m_rightBevel);
	DrawTextWithShadow(
		target,
		m_label,
		UiStyle::TEXT_SHADOW_COLOR,
		{ 2.f, 2.f }
	);
}

Button::State Button::GetState() const
{
	if (m_isPressed)
	{
		return PS_STATE_PRESSED;
	}

	if (m_isSelected)
	{
		return PS_STATE_SELECTED;
	}

	return PS_STATE_NORMAL;
}

void Button::UpdateGeometry(const sf::Vector2f offset)
{
	const sf::Vector2f pos = m_basePosition + offset;
	const float border = UiStyle::BUTTON_OUTLINE_THICKNESS;
	const float bevel = UiStyle::BUTTON_BEVEL_SIZE;

	m_shape.setPosition(pos);

	m_topBevel.setSize({ m_size.x - border * 2.f, bevel });
	m_topBevel.setPosition({ pos.x + border, pos.y + border });

	m_leftBevel.setSize({ bevel, m_size.y - border * 2.f });
	m_leftBevel.setPosition({ pos.x + border, pos.y + border });

	m_bottomBevel.setSize({ m_size.x - border * 2.f, bevel });
	m_bottomBevel.setPosition({ pos.x + border, pos.y + m_size.y - border - bevel });

	m_rightBevel.setSize({ bevel, m_size.y - border * 2.f });
	m_rightBevel.setPosition({ pos.x + m_size.x - border - bevel, pos.y + border });

	CenterTextAt(m_label, pos + m_size / 2.f);
}

void Button::ApplyState()
{
	const ButtonPalette palette = UiStyle::GetButtonPalette(m_theme);

	switch (GetState())
	{
		case PS_STATE_NORMAL:
			UpdateGeometry({ 0.f, 0.f });
			m_shape.setFillColor(palette.fill);
			m_shape.setOutlineColor(UiStyle::BUTTON_OUTLINE_COLOR);
			m_topBevel.setFillColor(palette.highlightBevel);
			m_leftBevel.setFillColor(palette.highlightBevel);
			m_bottomBevel.setFillColor(palette.shadowBevel);
			m_rightBevel.setFillColor(palette.shadowBevel);
			m_label.setFillColor(UiStyle::TEXT_COLOR);
			break;

		case PS_STATE_SELECTED:
			UpdateGeometry({ 0.f, 0.f });
			m_shape.setFillColor(palette.hoverFill);
			m_shape.setOutlineColor(UiStyle::BUTTON_SELECTED_OUTLINE_COLOR);
			m_topBevel.setFillColor(palette.highlightBevel);
			m_leftBevel.setFillColor(palette.highlightBevel);
			m_bottomBevel.setFillColor(palette.shadowBevel);
			m_rightBevel.setFillColor(palette.shadowBevel);
			m_label.setFillColor(UiStyle::SELECTED_TEXT_COLOR);
			break;

		case PS_STATE_PRESSED:
			UpdateGeometry({ UiStyle::BUTTON_PRESS_OFFSET, UiStyle::BUTTON_PRESS_OFFSET });
			m_shape.setFillColor(palette.fill);
			m_shape.setOutlineColor(UiStyle::BUTTON_SELECTED_OUTLINE_COLOR);
			m_topBevel.setFillColor(palette.shadowBevel);
			m_leftBevel.setFillColor(palette.shadowBevel);
			m_bottomBevel.setFillColor(palette.highlightBevel);
			m_rightBevel.setFillColor(palette.highlightBevel);
			m_label.setFillColor(UiStyle::SELECTED_TEXT_COLOR);
			break;
	}
}