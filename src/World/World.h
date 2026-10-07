#pragma once
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "Liquid.h"
#include "Player.h"
#include "CollisionSystem.h"
#include "LiquidSystem.h"

class World
{
private:
    sf::FloatRect m_bounds;
    float m_gravity;
    std::vector<Obstacle> m_obstacles;
    std::vector<Liquid> m_liquids;
    Player m_player;
    CollisionSystem m_collisionSystem;
    LiquidSystem m_liquidSystem;
    void DrawObstacles(sf::RenderWindow& window) const;
    void DrawLiquids(sf::RenderWindow& window) const;

public:
    World();
    void Update(PlayerInput& input, const float dt);
    void Draw(sf::RenderWindow& window) const;
    const sf::FloatRect GetBounds() const;
    const sf::Vector2f GetPositionPlayer() const;
};