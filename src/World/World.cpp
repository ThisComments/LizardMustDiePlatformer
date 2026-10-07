#include "World.h"
#include "SurfaceTypes.h"
#include "LiquidTypes.h"

World::World()
    :   m_bounds({0.f, 0.f}, {3000.f, 2000.f}),
        m_gravity(1800.f),
        m_obstacles({
            Obstacle({1500.f, 1975.f}, {3000.f, 50.f}, STONE),
            Obstacle({25.f, 1000.f}, {50.f, 2000.f}, STONE),
            Obstacle({2975.f, 1000.f}, {50.f, 2000.f}, STONE),

            // Левая часть
            Obstacle({300.f, 1850.f}, {300.f, 50.f}, GROUND),
            Obstacle({500.f, 1800.f}, {100.f, 50.f}, GROUND),
            Obstacle({600.f, 1750.f}, {100.f, 50.f}, GROUND),

            // Центральная часть
            Obstacle({950.f, 1850.f}, {400.f, 50.f}, GROUND),
            Obstacle({850.f, 1750.f}, {100.f, 50.f}, GROUND),
            Obstacle({750.f, 1650.f}, {100.f, 50.f}, GROUND),

            Obstacle({1400.f, 1800.f}, {300.f, 50.f}, GROUND),
            Obstacle({1600.f, 1700.f}, {100.f, 50.f}, GROUND),
            Obstacle({1750.f, 1600.f}, {100.f, 50.f}, GROUND),

            // Верхняя центральная зона
            Obstacle({1200.f, 1450.f}, {500.f, 50.f}, ICE),
            Obstacle({900.f, 1350.f}, {100.f, 50.f}, ICE),
            Obstacle({800.f, 1250.f}, {100.f, 50.f}, ICE),

            // Правая часть
            Obstacle({2100.f, 1850.f}, {500.f, 50.f}, GROUND),
            Obstacle({2400.f, 1830.f}, {100.f, 50.f}, GROUND),
            Obstacle({2500.f, 1810.f}, {100.f, 50.f}, GROUND),
            Obstacle({2600.f, 1790.f}, {100.f, 50.f}, GROUND),
            Obstacle({2700.f, 1670.f}, {100.f, 50.f}, GROUND),
            Obstacle({2000.f, 1750.f}, {100.f, 50.f}, GROUND),
            Obstacle({1900.f, 1650.f}, {100.f, 50.f}, GROUND),

            Obstacle({2400.f, 1550.f}, {300.f, 50.f}, STONE),
            Obstacle({2250.f, 1450.f}, {100.f, 50.f}, STONE),
            Obstacle({2150.f, 1350.f}, {100.f, 50.f}, STONE)
        }),
        m_liquids({
            Liquid({1500.f, 1925.f}, {2900.f, 50.f}, WATER_PROPERTIES)
        })
{
}

void World::Update(PlayerInput& input, const float dt)
{
    CollisionResult collisionResult = m_collisionSystem.Resolve(m_player, m_obstacles);
    LiquidResult liquidResult = m_liquidSystem.Resolve(m_player, m_liquids);
    LiquidProperties currentProperties = DEFAULT_PROPERTIES;
    if (liquidResult.isInLiquid)
    {
        currentProperties = liquidResult.properties;
    }
    m_player.Update(input, collisionResult.groundFriction, m_gravity, currentProperties, dt);
    m_player.SetIsGrounded(collisionResult.isGrounded);
}

void World::Draw(sf::RenderWindow& window) const
{
    DrawObstacles(window);
    m_player.Draw(window);
    DrawLiquids(window);
}

void World::DrawObstacles(sf::RenderWindow& window) const
{
    
    for (const Obstacle obstacle : m_obstacles)
    {
        sf::FloatRect bound = obstacle.GetBounds();
        sf::RectangleShape rect(bound.size);
        rect.setPosition(bound.position);

        switch (obstacle.GetVisualId())
        {
        case SURFACE_VISUAL_GROUND:
            rect.setFillColor(sf::Color(158, 106, 54));
            break;

        case SURFACE_VISUAL_ICE:
            rect.setFillColor(sf::Color(102, 127, 209, 160));
            break;

        case SURFACE_VISUAL_STONE:
            rect.setFillColor(sf::Color(111, 111, 111));
            break;
        
        default:
            rect.setFillColor(sf::Color(126, 0, 255));
            break;
        }

        window.draw(rect);
    }
}

void World::DrawLiquids(sf::RenderWindow& window) const
{
    for (const Liquid liquid : m_liquids)
    {
        sf::FloatRect bound = liquid.GetBounds();
        sf::RectangleShape rect(bound.size);
        rect.setPosition(bound.position);
        rect.setFillColor(sf::Color::Blue);

        window.draw(rect);
    }
}

const sf::FloatRect World::GetBounds() const
{
	return m_bounds;
}

const sf::Vector2f World::GetPositionPlayer() const
{
    return m_player.GetPosition();
}