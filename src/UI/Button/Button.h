#pragma once

#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

class Button
{
private:
	enum class State
	{
		Normal,
		Selected,
		Pressed
	};

	sf::RectangleShape m_shape;
	sf::Text m_label;
	bool m_isSelected;
	bool m_isPressed;

	State GetState() const;
	void ApplyState();

public:
	Button(
		const sf::Font& font,
		const std::string& label,
		sf::Vector2f position,
		sf::Vector2f size
	);

	void SetSelected(bool isSelected);
	void SetPressed(bool isPressed);
	bool Contains(sf::Vector2f point) const;
	void Render(sf::RenderTarget& target) const;
};
