#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Infrastructure/Input/PlayerInputEvent.h"
#include "Model/Player/EnvironmentState.h"
#include "Model/Player/Player.h"
#include "Systems/CollisionSystem/CollisionSystem.h"
#include "Systems/LiquidSystem/LiquidSystem.h"
#include "Model/Map/Map.h"

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

	EnvironmentState BuildEnvironmentState(
		const CollisionResult& collisionResult,
		const LiquidResult& liquidResult
	) const;
	void MovePlayerHorizontally(float dt);
	AxisCollisionResult MovePlayerVertically(float dt);

public:
	World(Map map);

	void PushPlayerInput(const PlayerInputEvent& event);
	void Update(float dt);

	const Map& GetMap() const;
	const Player& GetPlayer() const;
	sf::FloatRect GetBounds() const;
};
