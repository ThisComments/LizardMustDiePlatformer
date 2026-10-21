#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "World/Map/TileType.h"

constexpr int TILE_SIZE = 50;

struct CellCoordinates
{
	int col;
	int row;
};

class Map
{
private:
	std::vector<TileType> m_tiles;
	int m_width;
	int m_height;

	std::size_t GetIndex(int col, int row) const;

public:
	Map(int width, int height, std::vector<TileType> tiles);

	int GetWidth() const;
	int GetHeight() const;

	std::optional<CellCoordinates> WorldToCell(float worldX, float worldY) const;
	TileType GetTileAtCell(int col, int row) const;
	TileType GetTile(float worldX, float worldY) const;
	bool IsSolid(float worldX, float worldY) const;
};
