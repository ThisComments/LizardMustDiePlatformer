#pragma once

#include <memory>
#include <vector>

#include "Model/Entity/Entity.h"

class Player;
struct EntityContext;

// Detects overlaps between the player and triggerable entities and notifies them.
// Unlike CollisionSystem it never moves anything.
class TriggerSystem
{
public:
	void Update(
		const Player& player,
		const std::vector<std::unique_ptr<Entity>>& entities,
		EntityContext& context
	) const;
};
