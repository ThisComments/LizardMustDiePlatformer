#pragma once

#include <memory>
#include <vector>

#include "Model/Map/Map.h"
#include "Systems/CollisionSystem/CollisionSystem.h"
#include "Systems/LiquidSystem/LiquidSystem.h"
#include "World/WorldEvent.h"

class Entity;

struct EntityContext
{
	const Map& map;
	const CollisionSystem& collisionSystem;
	const LiquidSystem& liquidSystem;
	float gravity;
	std::vector<WorldEvent>& events;
	std::vector<std::unique_ptr<Entity>>& spawnQueue;
};
