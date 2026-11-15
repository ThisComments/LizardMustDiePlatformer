#pragma once

#include <vector>

#include "Model/Collectible/CollectibleType.h"
#include "Model/Map/Map.h"

struct CollectibleSpawn
{
	CollectibleType type;
	int col;
	int row;
};

// Everything a level file describes: tile geometry + interactive objects.
struct LevelData
{
	Map map;
	std::vector<CollectibleSpawn> collectibles;
};
