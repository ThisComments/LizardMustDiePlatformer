#include "Obstacle.h"

Obstacle::Obstacle(
    sf::Vector2f position,
    sf::Vector2f size,
    sf::Color color
)
{
    m_rectangle.setPosition(position);
    m_rectangle.setSize(size);
    m_rectangle.setFillColor(color);
    m_rectangle.setOrigin({
        size.x / 2.f, 
        size.y / 2.f
    });
}

sf::FloatRect Obstacle::GetBounds() const
{
    return m_rectangle.getGlobalBounds();
}

void Obstacle::Draw(sf::RenderWindow& window) const
{
    window.draw(m_rectangle);
}