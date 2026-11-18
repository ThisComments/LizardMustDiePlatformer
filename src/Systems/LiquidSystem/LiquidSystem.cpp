#include "Systems/LiquidSystem/LiquidSystem.h"

#include <algorithm>
#include <cmath>

#include "Model/Map/TileProperties.h"

LiquidResult LiquidSystem::Resolve(const sf::FloatRect& bounds, const Map& map) const
{
	LiquidResult result;

	const float tileSize = static_cast<float>(TILE_SIZE);
	const int firstCol = std::max(0, static_cast<int>(std::floor(bounds.position.x / tileSize)));
	const int firstRow = std::max(0, static_cast<int>(std::floor(bounds.position.y / tileSize)));
	const int lastCol = std::min(
		map.GetWidth() - 1,
		static_cast<int>(std::floor((bounds.position.x + bounds.size.x) / tileSize))
	);
	const int lastRow = std::min(
		map.GetHeight() - 1,
		static_cast<int>(std::floor((bounds.position.y + bounds.size.y) / tileSize))
	);

	for (int row = firstRow; row <= lastRow; ++row)
	{
		for (int col = firstCol; col <= lastCol; ++col)
		{
			const sf::FloatRect tileBounds(
				{ static_cast<float>(col) * tileSize, static_cast<float>(row) * tileSize },
				{ tileSize, tileSize }
			);

			if (!bounds.findIntersection(tileBounds))
			{
				continue;
			}

			const TileProperties& properties = GetTileProperties(map.GetTileAtCell(col, row));
			if (!properties.isLiquid)
			{
				continue;
			}

			if (!result.isInLiquid || properties.contactDamage > result.contactDamage)
			{
				result.isInLiquid = true;
				result.modifiers = properties.movementModifiers;
				result.contactDamage = properties.contactDamage;
			}
		}
	}

	return result;
}
