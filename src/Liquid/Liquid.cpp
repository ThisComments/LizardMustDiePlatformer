#include "LiquidTypes.h"

Liquid::Liquid(sf::Vector2f position, sf::Vector2f size, LiquidProperties properties)
{
    m_bounds = sf::FloatRect(position - sf::Vector2f{size.x / 2, size.y / 2}, size);
    m_properties.movementModifiers = properties.movementModifiers;
}

sf::FloatRect Liquid::GetBounds() const
{
    return m_bounds;
}


const MovementModifiers &Liquid::GetModifier() const
{
    return m_properties.movementModifiers;
}