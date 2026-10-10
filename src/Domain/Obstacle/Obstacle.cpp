#include "Obstacle.h"

Obstacle::Obstacle(sf::Vector2f position, sf::Vector2f size, Surface surface)
{
    m_bounds = sf::FloatRect(position - sf::Vector2f{size.x / 2, size.y / 2}, size);
    m_surface = surface;
}

float Obstacle::GetFriction() const
{
    return m_surface.friction;
}

SurfaceVisualId Obstacle::GetVisualId() const
{
    return m_surface.visualId;
}

sf::FloatRect Obstacle::GetBounds() const
{
    return m_bounds;
}