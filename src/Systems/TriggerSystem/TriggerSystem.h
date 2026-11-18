#pragma once

#include <memory>
#include <vector>

#include "Model/Entity/Entity.h"

class Collectible;
class Player;

class TriggerSystem
{
public:
	std::vector<Collectible*> FindTriggeredCollectibles(
		const Player& player,
		const std::vector<std::unique_ptr<Entity>>& entities
	) const;
};
