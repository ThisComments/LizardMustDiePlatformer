#pragma once

#include <SFML/Graphics.hpp>

#include "Domain/Map/Map.h"
#include "Domain/Player/Player.h"
#include "Application/CollisionSystem/CollisionSystem.h"
#include "Application/LiquidSystem/LiquidSystem.h"

class World
{
private:
	Map m_map;
	sf::FloatRect m_bounds;
	float m_gravity;
	Player m_player;
	EnvironmentState m_playerEnvironment;
	CollisionSystem m_collisionSystem;
	LiquidSystem m_liquidSystem;
	void DrawMap(sf::RenderTarget& window) const;
	EnvironmentState BuildEnvironmentState(
		const CollisionResult& collisionResult,
		const LiquidResult& liquidResult
	) const;

public:
	World();
	void Update(PlayerInput& input, float dt);
	void Draw(sf::RenderTarget& window) const;
	sf::FloatRect GetBounds() const;
	sf::Vector2f GetPositionPlayer() const;
};
