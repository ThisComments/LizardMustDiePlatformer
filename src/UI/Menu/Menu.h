#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "UI/Button/Button.h"

struct MenuItemSpec
{
	std::string label;
	ButtonTheme theme = PS_BUTTON_THEME_PRIMARY;
};

class Menu
{
private:
	std::vector<Button> m_buttons;
	std::size_t m_selected;
	std::optional<std::size_t> m_pressed;

	void Select(std::size_t index);
	void ClearPressed();
	std::optional<std::size_t> FindButtonAt(sf::Vector2f point) const;
	std::optional<std::size_t> HandleKeyPressed(sf::Keyboard::Key key);
	std::optional<std::size_t> HandleMouseReleased(sf::Vector2f point);

public:
	Menu(
		const sf::Font& font,
		const std::vector<MenuItemSpec>& items,
		sf::Vector2f screenSize,
		float startY
	);

	std::optional<std::size_t> HandleEvent(const sf::Event& event, const sf::RenderWindow& window);
	void Reset();
	void Render(sf::RenderTarget& target) const;
};