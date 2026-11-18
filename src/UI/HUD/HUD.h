#pragma once

#include <string>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "World/WorldStats.h"

class HUD
{
private:
	struct AbilitySlotWidget
	{
		sf::RectangleShape box;
		sf::RectangleShape keySlot;
		sf::Text keyText;
		sf::Text titleText;
		sf::Text statusText;

		AbilitySlotWidget(
			const sf::Font& font,
			const std::string& keyLabel,
			const std::string& titleLabel,
			sf::Vector2f position
		);

		void UpdateCooldown(float cooldown);
		void Render(sf::RenderTarget& target) const;
	};

	sf::Vector2f m_panelPosition;
	sf::RectangleShape m_healthBarBackground;
	sf::RectangleShape m_healthBarFill;
	sf::RectangleShape m_healthBarHighlight;
	sf::RectangleShape m_goldBox;
	sf::CircleShape m_coinOuter;
	sf::CircleShape m_coinInner;
	sf::Text m_healthText;
	sf::Text m_goldValueText;
	AbilitySlotWidget m_healWidget;
	AbilitySlotWidget m_dashWidget;

public:
	explicit HUD(const sf::Font& font);

	void Update(const WorldStats& stats);
	void Render(sf::RenderTarget& target) const;
};
