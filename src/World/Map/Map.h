#pragma once
#include <cstdint>
#include <filesystem>
#include <vector>
#include "TileType.h"

constexpr uint32_t TILE_SIZE = 70;

struct Map
{
	std::vector<TileType> grid;
	std::vector<uint32_t> visualIds;
	uint32_t width = 0;
	uint32_t height = 0;
};

void MapInit(Map& map);
void MapLoad(Map& map, const std::filesystem::path& levelFile);
TileType MapGetTile(const Map& map, float worldX, float worldY);
uint32_t MapGetVisualId(const Map& map, float worldX, float worldY);
bool MapIsSolid(const Map& map, float worldX, float worldY);
