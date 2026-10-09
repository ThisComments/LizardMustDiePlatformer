#pragma once
#include <SFML/Graphics.hpp>
#include "../Player/Player.h"

struct LiquidProperties
{
	MovementModifiers movementModifiers;
};

class Liquid
{
private:
	sf::FloatRect m_bounds;
	LiquidProperties m_properties;

public:
	Liquid(sf::Vector2f position, sf::Vector2f size, LiquidProperties properties);
	sf::FloatRect GetBounds() const;
	const MovementModifiers &GetModifier() const;

};