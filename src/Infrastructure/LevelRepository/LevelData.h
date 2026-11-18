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

struct TileZone
{
	int col = 0;
	int row = 0;
	int width = 1;
	int height = 1;
};

struct LevelData
{
	Map map;
	CellCoordinates playerSpawn = { 0, 0 };
	TileZone finishZone;
	std::vector<CollectibleSpawn> collectibles;
};
