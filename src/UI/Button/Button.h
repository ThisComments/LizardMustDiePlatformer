#pragma once

#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "UI/UiStyle.h"

class Button
{
private:
	enum State
	{
		PS_STATE_NORMAL,
		PS_STATE_SELECTED,
		PS_STATE_PRESSED
	};

	sf::Vector2f m_basePosition;
	sf::Vector2f m_size;
	ButtonTheme m_theme;
	sf::RectangleShape m_shape;
	sf::RectangleShape m_topBevel;
	sf::RectangleShape m_leftBevel;
	sf::RectangleShape m_bottomBevel;
	sf::RectangleShape m_rightBevel;
	sf::Text m_label;
	bool m_isSelected;
	bool m_isPressed;

	State GetState() const;
	void ApplyState();
	void UpdateGeometry(sf::Vector2f offset);

public:
	Button(
		const sf::Font& font,
		const std::string& label,
		sf::Vector2f position,
		sf::Vector2f size,
		ButtonTheme theme = PS_BUTTON_THEME_PRIMARY
	);

	void SetSelected(bool isSelected);
	void SetPressed(bool isPressed);
	bool Contains(sf::Vector2f point) const;
	void Render(sf::RenderTarget& target) const;
};