#pragma once

#include <memory>
#include <vector>

#include "Model/Map/Map.h"
#include "Systems/CollisionSystem/CollisionSystem.h"
#include "Systems/LiquidSystem/LiquidSystem.h"
#include "World/WorldEvent.h"

class Entity;

// What an entity may use while it updates. Only what entities really need is listed here;
// the whole World or Game is deliberately not reachable.
struct EntityContext
{
	const Map& map;
	const CollisionSystem& collisionSystem;
	const LiquidSystem& liquidSystem;
	float gravity;
	std::vector<WorldEvent>& events;
	// Entities created during an update; World adds them after the update loop finishes.
	std::vector<std::unique_ptr<Entity>>& spawnQueue;
};
