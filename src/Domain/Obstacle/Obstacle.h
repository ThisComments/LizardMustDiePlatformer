#pragma once
#include <SFML/Graphics.hpp>
#include "Surface.h"

class Obstacle
{
private:
    sf::FloatRect m_bounds;
    Surface m_surface;

public:
    Obstacle(sf::Vector2f position, sf::Vector2f size, Surface surface);
    sf::FloatRect GetBounds() const;
    float GetFriction() const;
    SurfaceVisualId GetVisualId() const;
};