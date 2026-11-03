#include "UI/Button/Button.h"

#include "UI/TextUtils/TextUtils.h"
#include "UI/UiStyle.h"

Button::Button(
	const sf::Font& font,
	const std::string& label,
	const sf::Vector2f position,
	const sf::Vector2f size
)
	: 	m_shape(size),
		m_label(font, ToSfString(label), UiStyle::BUTTON_FONT_SIZE),
		m_isSelected(false),
		m_isPressed(false)
{
	m_shape.setPosition(position);
	m_shape.setOutlineThickness(-UiStyle::BUTTON_OUTLINE_THICKNESS);
	CenterTextAt(m_label, position + size / 2.f);
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
	target.draw(m_label);
}

Button::State Button::GetState() const
{
	if (m_isPressed)
	{
		return State::Pressed;
	}

	if (m_isSelected)
	{
		return State::Selected;
	}

	return State::Normal;
}

void Button::ApplyState()
{
	switch (GetState())
	{
		case State::Normal:
			m_shape.setFillColor(UiStyle::BUTTON_FILL_COLOR);
			m_shape.setOutlineColor(UiStyle::BUTTON_OUTLINE_COLOR);
			m_label.setFillColor(UiStyle::TEXT_COLOR);
			break;

		case State::Selected:
			m_shape.setFillColor(UiStyle::BUTTON_SELECTED_FILL_COLOR);
			m_shape.setOutlineColor(UiStyle::BUTTON_SELECTED_OUTLINE_COLOR);
			m_label.setFillColor(UiStyle::SELECTED_TEXT_COLOR);
			break;

		case State::Pressed:
			m_shape.setFillColor(UiStyle::BUTTON_PRESSED_FILL_COLOR);
			m_shape.setOutlineColor(UiStyle::BUTTON_SELECTED_OUTLINE_COLOR);
			m_label.setFillColor(UiStyle::SELECTED_TEXT_COLOR);
			break;
	}
}
