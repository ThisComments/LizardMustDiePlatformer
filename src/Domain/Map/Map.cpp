#include "Map.h"

#include <cmath>

namespace
{
	const uint32_t TEST_MAP_WIDTH = 75;
	const uint32_t TEST_MAP_HEIGHT = 60;

	uint32_t GetIndex(const Map& map, uint32_t col, uint32_t row)
	{
		return row * map.width + col;
	}

	void SetTile(Map& map, uint32_t col, uint32_t row, TileType tileType, uint32_t visualId)
	{
		const uint32_t index = GetIndex(map, col, row);
		map.grid[index] = tileType;
		map.visualIds[index] = visualId;
	}
}

void MapInit(Map& map)
{
	map.width = TEST_MAP_WIDTH;
	map.height = TEST_MAP_HEIGHT;
	map.grid.assign(map.width * map.height, TileType::Empty);
	map.visualIds.assign(map.width * map.height, TVI_NONE);

	for (uint32_t col = 0; col < map.width; ++col)
	{
		SetTile(map, col, 0, TileType::Stone, TVI_STONE);
		SetTile(map, col, map.height - 1, TileType::Stone, TVI_STONE);
	}

	for (uint32_t row = 0; row < map.height; ++row)
	{
		SetTile(map, 0, row, TileType::Stone, TVI_STONE);
		SetTile(map, map.width - 1, row, TileType::Stone, TVI_STONE);
	}

	for (uint32_t col = 4; col <= 8; ++col)
	{
		SetTile(map, col, 16, TileType::Ground, TVI_GROUND);
	}

	for (uint32_t col = 8; col <= 11; ++col)
	{
		SetTile(map, col, 14, TileType::Ground, TVI_GROUND);
	}

	for (uint32_t col = 14; col <= 18; ++col)
	{
		SetTile(map, col, 16, TileType::Stone, TVI_STONE);
	}

	for (uint32_t col = 21; col <= 25; ++col)
	{
		SetTile(map, col, 13, TileType::Ground, TVI_GROUND);
	}

	for (uint32_t col = 11; col <= 16; ++col)
	{
		SetTile(map, col, 18, TileType::Water, TVI_WATER);
	}
}

TileType MapGetTile(const Map& map, float worldX, float worldY)
{
	if (!std::isfinite(worldX) || !std::isfinite(worldY) ||
		worldX < 0.f || worldY < 0.f || map.width == 0 || map.height == 0)
	{
		return TileType::Empty;
	}

	const float mapWidth = static_cast<float>(map.width * TILE_SIZE);
	const float mapHeight = static_cast<float>(map.height * TILE_SIZE);

	if (worldX >= mapWidth || worldY >= mapHeight)
	{
		return TileType::Empty;
	}

	const uint32_t col = static_cast<uint32_t>(worldX / TILE_SIZE);
	const uint32_t row = static_cast<uint32_t>(worldY / TILE_SIZE);
	const uint32_t index = GetIndex(map, col, row);

	if (index >= map.grid.size())
	{
		return TileType::Empty;
	}

	return map.grid[index];
}

uint32_t MapGetVisualId(const Map& map, float worldX, float worldY)
{
	if (!std::isfinite(worldX) || !std::isfinite(worldY) ||
		worldX < 0.f || worldY < 0.f || map.width == 0 || map.height == 0)
	{
		return TVI_NONE;
	}

	const float mapWidth = static_cast<float>(map.width * TILE_SIZE);
	const float mapHeight = static_cast<float>(map.height * TILE_SIZE);

	if (worldX >= mapWidth || worldY >= mapHeight)
	{
		return TVI_NONE;
	}

	const uint32_t col = static_cast<uint32_t>(worldX / TILE_SIZE);
	const uint32_t row = static_cast<uint32_t>(worldY / TILE_SIZE);
	const uint32_t index = GetIndex(map, col, row);

	if (index >= map.visualIds.size())
	{
		return TVI_NONE;
	}

	return map.visualIds[index];
}

bool MapIsSolid(const Map& map, float worldX, float worldY)
{
	const TileType tileType = MapGetTile(map, worldX, worldY);
	return tileType == TileType::Ground || tileType == TileType::Stone;
}
