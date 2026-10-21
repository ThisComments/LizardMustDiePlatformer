#include "World/LiquidSystem/LiquidSystem.h"

#include <algorithm>
#include <cmath>

#include "World/Map/TileProperties.h"

LiquidResult LiquidSystem::Resolve(const sf::FloatRect& playerBounds, const Map& map) const
{
	LiquidResult result;

	const float tileSize = static_cast<float>(TILE_SIZE);
	const int firstCol = std::max(0, static_cast<int>(std::floor(playerBounds.position.x / tileSize)));
	const int firstRow = std::max(0, static_cast<int>(std::floor(playerBounds.position.y / tileSize)));
	const int lastCol = std::min(static_cast<int>(map.GetWidth()) - 1,
		static_cast<int>(std::floor((playerBounds.position.x + playerBounds.size.x) / tileSize)));
	const int lastRow = std::min(static_cast<int>(map.GetHeight()) - 1,
		static_cast<int>(std::floor((playerBounds.position.y + playerBounds.size.y) / tileSize)));

	for (int row = firstRow; row <= lastRow; ++row)
	{
		for (int col = firstCol; col <= lastCol; ++col)
		{
			const sf::FloatRect tileBounds(
				{static_cast<float>(col) * tileSize, static_cast<float>(row) * tileSize},
				{tileSize, tileSize}
			);

			if (!playerBounds.findIntersection(tileBounds))
			{
				continue;
			}

			const TileProperties& properties = GetTileProperties(map.GetTileAtCell(col, row));
			if (!properties.isLiquid)
			{
				continue;
			}

			result.isInLiquid = true;
			result.modifiers = properties.movementModifiers;
			return result;
		}
	}

	return result;
}
