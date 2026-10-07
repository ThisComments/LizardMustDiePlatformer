#pragma once
#include <SFML/Graphics.hpp>

struct LiquidProperties
{
	float gravityScale;
	float movementSpeedScale;
	float drag;
	bool allowsJump;
	bool allowsDash;
};

class Liquid
{
private:
	sf::FloatRect m_bounds;
	LiquidProperties m_properties;

public:
	Liquid(sf::Vector2f position, sf::Vector2f size, LiquidProperties properties);
	sf::FloatRect GetBounds() const;
	const LiquidProperties &GetProperties() const;

};