#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>

#include "Screens/Screen.h"
#include "UI/Menu/Menu.h"

class HowToPlayScreen : public Screen
{
private:
	const sf::Font& m_font;
	sf::Vector2f m_screenSize;
	Menu m_menu;

public:
	HowToPlayScreen(const sf::Font& font, sf::Vector2f screenSize);

	ScreenCommand HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	ScreenCommand Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;
	void OnEnter() override;
};
