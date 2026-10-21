#include "World/Map/Map.h"

#include <cmath>
#include <stdexcept>
#include <utility>

#include "World/Map/TileProperties.h"

Map::Map(const int width, const int height, std::vector<TileType> tiles)
	: m_tiles(std::move(tiles)),
	  m_width(width),
	  m_height(height)
{
	if (width == 0 || height == 0)
	{
		throw std::invalid_argument("Map dimensions must be positive");
	}

	if (m_tiles.size() != static_cast<std::size_t>(width) * height)
	{
		throw std::invalid_argument("Map tile count does not match its dimensions");
	}
}

int Map::GetWidth() const
{
	return m_width;
}

int Map::GetHeight() const
{
	return m_height;
}

std::size_t Map::GetIndex(const int col, const int row) const
{
	return static_cast<std::size_t>(row) * m_width + col;
}

std::optional<CellCoordinates> Map::WorldToCell(const float worldX, const float worldY) const
{
	if (!std::isfinite(worldX) || !std::isfinite(worldY) || worldX < 0.f || worldY < 0.f)
	{
		return std::nullopt;
	}

	const float mapWidth = static_cast<float>(m_width) * TILE_SIZE;
	const float mapHeight = static_cast<float>(m_height) * TILE_SIZE;
	if (worldX >= mapWidth || worldY >= mapHeight)
	{
		return std::nullopt;
	}

	const int col = static_cast<int>(worldX / TILE_SIZE);
	const int row = static_cast<int>(worldY / TILE_SIZE);
	if (col >= m_width || row >= m_height)
	{
		return std::nullopt;
	}

	return CellCoordinates{col, row};
}

TileType Map::GetTileAtCell(const int col, const int row) const
{
	if (col < 0 || row < 0 || col >= static_cast<int>(m_width) || row >= static_cast<int>(m_height))
	{
		return TileType::Empty;
	}

	return m_tiles[GetIndex(static_cast<int>(col), static_cast<int>(row))];
}

TileType Map::GetTile(const float worldX, const float worldY) const
{
	const std::optional<CellCoordinates> cell = WorldToCell(worldX, worldY);
	if (!cell)
	{
		return TileType::Empty;
	}

	return m_tiles[GetIndex(cell->col, cell->row)];
}

bool Map::IsSolid(const float worldX, const float worldY) const
{
	return GetTileProperties(GetTile(worldX, worldY)).isSolid;
}
