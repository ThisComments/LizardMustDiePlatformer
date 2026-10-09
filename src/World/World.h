#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Obstacle/Obstacle.h"
#include "../Liquid/Liquid.h"
#include "../Player/Player.h"
#include "../CollisionSystem/CollisionSystem.h"
#include "../LiquidSystem/LiquidSystem.h"

class World
{
private:
    sf::FloatRect m_bounds;
    float m_gravity;
    std::vector<Obstacle> m_obstacles;
    std::vector<Liquid> m_liquids;
    Player m_player;
    EnvironmentState m_playerEnvironment;
    CollisionSystem m_collisionSystem;
    LiquidSystem m_liquidSystem;
    void DrawObstacles(sf::RenderTarget& window) const;
    void DrawLiquids(sf::RenderTarget& window) const;
    EnvironmentState BuildEnvironmentState(
        const CollisionResult& collisionResult,
        const LiquidResult& liquidResult
    ) const;

public:
    World();
    void Update(PlayerInput& input, const float dt);
    void Draw(sf::RenderTarget& window) const;
    const sf::FloatRect GetBounds() const;
    const sf::Vector2f GetPositionPlayer() const;
};