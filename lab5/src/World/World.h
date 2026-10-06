#pragma once

#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "Player.h"
#include "CollisionSystem.h"

class World
{
private:
    sf::FloatRect m_bounds;
    float m_gravity;
    float m_friction;
    std::vector<Obstacle> m_obstacles;
    Player m_player;
    CollisionSystem m_collisionSystem;
    void DrawObstacles(sf::RenderWindow& window);

public:
    World();
    void Update(PlayerInput& input, const float dt);
    void Draw(sf::RenderWindow& window);
    const sf::FloatRect GetBounds() const;
    const sf::Vector2f GetPositionPlayer() const;
};