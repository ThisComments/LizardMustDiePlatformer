#pragma once

#include <SFML/Graphics.hpp>

class Obstacle
{
private:
    sf::RectangleShape m_rectangle;

public:
    Obstacle(
        sf::Vector2f position,
        sf::Vector2f size,
        sf::Color color
    );
    sf::FloatRect GetBounds() const;
    void Draw(sf::RenderWindow& window) const;
};