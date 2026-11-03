#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>

#include "Screens/Screen.h"
#include "UI/Menu/Menu.h"
#include "UI/Overlay/ScreenOverlay.h"

class PauseScreen : public Screen
{
private:
	const sf::Font& m_font;
	sf::Vector2f m_screenSize;
	ScreenOverlay m_overlay;
	Menu m_menu;

public:
	PauseScreen(const sf::Font& font, sf::Vector2f screenSize);

	ScreenCommand HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	ScreenCommand Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;
	void OnEnter() override;
	bool IsOverlay() const override;
};
