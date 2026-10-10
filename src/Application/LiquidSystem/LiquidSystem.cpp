#include "LiquidSystem.h"
#include "Domain/Liquid/LiquidTypes.h"

#include <algorithm>
#include <cmath>

LiquidResult LiquidSystem::Resolve(const Player& player, const Map& map) const
{
	LiquidResult result;
	result.modifiers = DEFAULT_PROPERTIES.movementModifiers;

	const sf::FloatRect playerBounds = player.GetBounds();
	const float tileSize = static_cast<float>(TILE_SIZE);
	const int firstCol = std::max(0, static_cast<int>(std::floor(playerBounds.position.x / tileSize)));
	const int firstRow = std::max(0, static_cast<int>(std::floor(playerBounds.position.y / tileSize)));
	const int lastCol = std::min(static_cast<int>(map.width) - 1,
		static_cast<int>(std::floor((playerBounds.position.x + playerBounds.size.x) / tileSize)));
	const int lastRow = std::min(static_cast<int>(map.height) - 1,
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

			const uint32_t index = static_cast<uint32_t>(row) * map.width + static_cast<uint32_t>(col);

			if (index >= map.grid.size() || map.grid[index] != TileType::Water)
			{
				continue;
			}

			result.isInLiquid = true;
			result.modifiers = WATER_PROPERTIES.movementModifiers;
			return result;
		}
	}

	return result;
}
