#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

struct HudData
{
	int health = 0;
	int maxHealth = 0;
	int potions = 0;
	int level = 0;
};

class HUD
{
private:
	sf::Text m_healthText;
	sf::Text m_potionsText;
	sf::Text m_levelText;

public:
	explicit HUD(const sf::Font& font);

	void SetData(const HudData& data);
	void Render(sf::RenderTarget& target) const;
};
