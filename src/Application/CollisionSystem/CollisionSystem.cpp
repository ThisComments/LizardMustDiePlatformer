#include "CollisionSystem.h"
#include "Domain/Obstacle/SurfaceTypes.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

namespace
{
	const float GROUND_PROBE_DISTANCE = 1.f;
	const float COLLISION_EPSILON = 0.001f;
}

AxisCollisionResult CollisionSystem::ResolveHorizontal(
	const Player& player,
	const Map& map,
	const float deltaX
) const
{
	AxisCollisionResult result;
	result.allowedMovement = deltaX;

	if (deltaX == 0.f || map.width == 0 || map.height == 0)
	{
		return result;
	}

	const sf::FloatRect bounds = player.GetBounds();
	const float tileSize = static_cast<float>(TILE_SIZE);
	const float currentLeft = bounds.position.x;
	const float currentRight = currentLeft + bounds.size.x;
	const float top = bounds.position.y;
	const float bottom = top + bounds.size.y;
	const float targetLeft = currentLeft + deltaX;
	const float targetRight = currentRight + deltaX;

	const float minX = std::min(currentLeft, targetLeft);
	const float maxX = std::max(currentRight, targetRight);
	const int firstCol = std::max(0, static_cast<int>(std::floor(minX / tileSize)));
	const int lastCol = std::min(
		static_cast<int>(map.width) - 1,
		static_cast<int>(std::floor((maxX - COLLISION_EPSILON) / tileSize))
	);
	const int firstRow = std::max(0, static_cast<int>(std::floor(top / tileSize)));
	const int lastRow = std::min(
		static_cast<int>(map.height) - 1,
		static_cast<int>(std::floor((bottom - COLLISION_EPSILON) / tileSize))
	);

	for (int row = firstRow; row <= lastRow; ++row)
	{
		for (int col = firstCol; col <= lastCol; ++col)
		{
			if (!IsSolidTile(map, col, row))
			{
				continue;
			}

			const float tileLeft = static_cast<float>(col) * tileSize;
			const float tileRight = tileLeft + tileSize;
			const float tileTop = static_cast<float>(row) * tileSize;
			const float tileBottom = tileTop + tileSize;
			const bool overlapsVertically = bottom > tileTop && top < tileBottom;

			if (!overlapsVertically)
			{
				continue;
			}

			if (deltaX > 0.f && currentRight <= tileLeft && targetRight > tileLeft)
			{
				result.allowedMovement = std::min(result.allowedMovement, tileLeft - currentRight);
				result.collided = true;
			}
			else if (deltaX < 0.f && currentLeft >= tileRight && targetLeft < tileRight)
			{
				result.allowedMovement = std::max(result.allowedMovement, tileRight - currentLeft);
				result.collided = true;
			}
			else if (bounds.findIntersection(sf::FloatRect(
				{tileLeft, tileTop}, {tileSize, tileSize})))
			{
				if (deltaX > 0.f && currentRight > tileLeft)
				{
					result.allowedMovement = std::min(result.allowedMovement, tileLeft - currentRight);
					result.collided = true;
				}
				else if (deltaX < 0.f && currentLeft < tileRight)
				{
					result.allowedMovement = std::max(result.allowedMovement, tileRight - currentLeft);
					result.collided = true;
				}
			}
		}
	}

	return result;
}

AxisCollisionResult CollisionSystem::ResolveVertical(
	const Player& player,
	const Map& map,
	const float deltaY
) const
{
	AxisCollisionResult result;
	result.allowedMovement = deltaY;

	if (map.width == 0 || map.height == 0)
	{
		return result;
	}

	const sf::FloatRect bounds = player.GetBounds();
	const float tileSize = static_cast<float>(TILE_SIZE);
	const float left = bounds.position.x;
	const float right = left + bounds.size.x;
	const float currentTop = bounds.position.y;
	const float currentBottom = currentTop + bounds.size.y;
	const float effectiveDeltaY = deltaY == 0.f ? GROUND_PROBE_DISTANCE : deltaY;
	const float targetTop = currentTop + effectiveDeltaY;
	const float targetBottom = currentBottom + effectiveDeltaY;

	const float minY = std::min(currentTop, targetTop);
	const float maxY = std::max(currentBottom, targetBottom);
	const int firstRow = std::max(0, static_cast<int>(std::floor(minY / tileSize)));
	const int lastRow = std::min(
		static_cast<int>(map.height) - 1,
		static_cast<int>(std::floor((maxY - COLLISION_EPSILON) / tileSize))
	);
	const int firstCol = std::max(0, static_cast<int>(std::floor(left / tileSize)));
	const int lastCol = std::min(
		static_cast<int>(map.width) - 1,
		static_cast<int>(std::floor((right - COLLISION_EPSILON) / tileSize))
	);

	for (int row = firstRow; row <= lastRow; ++row)
	{
		for (int col = firstCol; col <= lastCol; ++col)
		{
			if (!IsSolidTile(map, col, row))
			{
				continue;
			}

			const float tileLeft = static_cast<float>(col) * tileSize;
			const float tileRight = tileLeft + tileSize;
			const float tileTop = static_cast<float>(row) * tileSize;
			const float tileBottom = tileTop + tileSize;
			const bool overlapsHorizontally = right > tileLeft && left < tileRight;

			if (!overlapsHorizontally)
			{
				continue;
			}

			if (effectiveDeltaY > 0.f && currentBottom <= tileTop && targetBottom > tileTop)
			{
				const float allowed = tileTop - currentBottom;
				if (deltaY == 0.f || allowed < result.allowedMovement)
				{
					result.allowedMovement = deltaY == 0.f ? 0.f : allowed;
					result.collided = deltaY != 0.f;
					result.isGrounded = true;
					result.groundFriction = GetTileFriction(map, col, row);
				}
			}
			else if (deltaY > 0.f && bounds.findIntersection(sf::FloatRect(
				{tileLeft, tileTop}, {tileSize, tileSize})))
			{
				result.allowedMovement = std::min(result.allowedMovement, tileTop - currentBottom);
				result.collided = true;
				result.isGrounded = true;
				result.groundFriction = GetTileFriction(map, col, row);
			}
			else if (deltaY < 0.f && currentTop >= tileBottom && targetTop < tileBottom)
			{
				result.allowedMovement = std::max(result.allowedMovement, tileBottom - currentTop);
				result.collided = true;
			}
			else if (deltaY < 0.f && bounds.findIntersection(sf::FloatRect(
				{tileLeft, tileTop}, {tileSize, tileSize})))
			{
				result.allowedMovement = std::max(result.allowedMovement, tileBottom - currentTop);
				result.collided = true;
			}
		}
	}

	return result;
}

bool CollisionSystem::IsSolidTile(const Map& map, const int col, const int row) const
{
	if (col < 0 || row < 0 || col >= static_cast<int>(map.width) || row >= static_cast<int>(map.height))
	{
		return false;
	}

	const std::uint32_t index = static_cast<std::uint32_t>(row) * map.width + static_cast<std::uint32_t>(col);
	if (index >= map.grid.size())
	{
		return false;
	}

	const TileType tileType = map.grid[index];
	return tileType == TileType::Ground || tileType == TileType::Stone;
}

float CollisionSystem::GetTileFriction(const Map& map, const int col, const int row) const
{
	const std::uint32_t index = static_cast<std::uint32_t>(row) * map.width + static_cast<std::uint32_t>(col);
	if (index >= map.grid.size())
	{
		return 0.f;
	}

	if (map.grid[index] == TileType::Ground)
	{
		return GROUND.friction;
	}
	if (map.grid[index] == TileType::Stone)
	{
		return STONE.friction;
	}

	return 0.f;
}
