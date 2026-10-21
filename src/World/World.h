#pragma once

#include <SFML/Graphics.hpp>

#include "World/Map/Map.h"
#include "Player/Player.h"
#include "World/CollisionSystem/CollisionSystem.h"
#include "World/LiquidSystem/LiquidSystem.h"
#include "World/Render/Render.h"

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
	Render m_render;
	EnvironmentState BuildEnvironmentState(
		const CollisionResult& collisionResult,
		const LiquidResult& liquidResult
	) const;

public:
	World();
	void PushPlayerInput(const PlayerInputEvent& event);
	void Update(float dt);
	void Draw(sf::RenderTarget& window) const;
	sf::FloatRect GetBounds() const;
	sf::Vector2f GetPositionPlayer() const;
};
