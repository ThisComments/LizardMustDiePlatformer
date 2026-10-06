#include "World.h"

World::World()
    :   m_bounds({0.f, 0.f}, {3000.f, 2000.f}),
        m_gravity(1800.f),
        m_friction(1800.f),
        m_obstacles({
            Obstacle({1500.f, 1975.f}, {3000.f, 50.f}, sf::Color::Red),
            Obstacle({25.f, 1000.f}, {50.f, 2000.f}, sf::Color::Red),
            Obstacle({2975.f, 1000.f}, {50.f, 2000.f}, sf::Color::Red),

            // Левая часть
            Obstacle({300.f, 1850.f}, {300.f, 50.f}, sf::Color::Blue),
            Obstacle({500.f, 1800.f}, {100.f, 50.f}, sf::Color::Green),
            Obstacle({600.f, 1750.f}, {100.f, 50.f}, sf::Color::Yellow),

            // Центральная часть
            Obstacle({950.f, 1850.f}, {400.f, 50.f}, sf::Color::Blue),
            Obstacle({850.f, 1750.f}, {100.f, 50.f}, sf::Color::Green),
            Obstacle({750.f, 1650.f}, {100.f, 50.f}, sf::Color::Yellow),

            Obstacle({1400.f, 1800.f}, {300.f, 50.f}, sf::Color::White),
            Obstacle({1600.f, 1700.f}, {100.f, 50.f}, sf::Color::Blue),
            Obstacle({1750.f, 1600.f}, {100.f, 50.f}, sf::Color::Green),

            // Верхняя центральная зона
            Obstacle({1300.f, 1450.f}, {300.f, 50.f}, sf::Color::Yellow),
            Obstacle({1100.f, 1350.f}, {100.f, 50.f}, sf::Color::White),
            Obstacle({1000.f, 1250.f}, {100.f, 50.f}, sf::Color::Blue),

            // Правая часть
            Obstacle({2100.f, 1850.f}, {500.f, 50.f}, sf::Color::Green),
            Obstacle({2400.f, 1830.f}, {100.f, 50.f}, sf::Color::Blue),
            Obstacle({2500.f, 1810.f}, {100.f, 50.f}, sf::Color::Yellow),
            Obstacle({2600.f, 1790.f}, {100.f, 50.f}, sf::Color::Red),
            Obstacle({2700.f, 1670.f}, {100.f, 50.f}, sf::Color::White),
            Obstacle({2000.f, 1750.f}, {100.f, 50.f}, sf::Color::Yellow),
            Obstacle({1900.f, 1650.f}, {100.f, 50.f}, sf::Color::White),

            Obstacle({2400.f, 1550.f}, {300.f, 50.f}, sf::Color::Blue),
            Obstacle({2250.f, 1450.f}, {100.f, 50.f}, sf::Color::Green),
            Obstacle({2150.f, 1350.f}, {100.f, 50.f}, sf::Color::Yellow)
        })
{
}

void World::Update(PlayerInput& input, const float dt)
{
    m_player.Update(input, m_friction, m_gravity, dt);
    m_collisionSystem.Resolve(m_player, m_obstacles);
    m_player.SetIsGrounded(m_collisionSystem.IsGrounded(m_player, m_obstacles));
}

void World::Draw(sf::RenderWindow& window)
{
    DrawObstacles(window);
    m_player.Draw(window);
}

void World::DrawObstacles(sf::RenderWindow& window)
{
    for (size_t i = 0; i < m_obstacles.size(); i++)
    {
        m_obstacles[i].Draw(window);
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