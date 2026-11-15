#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

// Display-only. It shows only indicators that have gameplay logic behind them (currently the
// score) and draws in whatever view is active: the caller selects the screen-space view.
class HUD
{
private:
	sf::Text m_scoreText;

public:
	HUD(const sf::Font& font);

	void SetScore(int score);
	void Render(sf::RenderTarget& target) const;
};
