#include "World/CollisionSystem/CollisionSystem.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "World/Map/TileProperties.h"

const float COLLISION_EPSILON = 0.001f;
const float GROUND_CONTACT_TOLERANCE = 0.01f;

struct TileRange
{
	int first = 0;
	int last = -1;

	bool IsEmpty() const
	{
		return last < first;
	}
};

TileRange ComputeTileRange(const float min, const float max, const int tileCount)
{
	TileRange range;
	if (tileCount == 0 || !std::isfinite(min) || !std::isfinite(max))
	{
		return range;
	}

	const float tileSize = static_cast<float>(TILE_SIZE);
	const float maxIndex = static_cast<float>(tileCount - 1);
	const float first = std::clamp(std::floor((min + COLLISION_EPSILON) / tileSize), 0.f, maxIndex);
	const float last = std::clamp(std::floor((max - COLLISION_EPSILON) / tileSize), 0.f, maxIndex);

	if (max - COLLISION_EPSILON < 0.f || min + COLLISION_EPSILON >= static_cast<float>(tileCount) * tileSize)
	{
		return range;
	}

	range.first = static_cast<int>(first);
	range.last = static_cast<int>(last);
	return range;
}

float Overlap(const float minA, const float maxA, const float minB, const float maxB)
{
	return std::min(maxA, maxB) - std::max(minA, minB);
}

AxisCollisionResult CollisionSystem::ResolveHorizontal(
	const sf::FloatRect& bounds,
	const Map& map,
	const float deltaX
) const
{
	AxisCollisionResult result;
	result.allowedMovement = deltaX;

	if (deltaX == 0.f || !std::isfinite(deltaX))
	{
		result.allowedMovement = 0.f;
		return result;
	}

	const float tileSize = static_cast<float>(TILE_SIZE);
	const float left = bounds.position.x;
	const float right = left + bounds.size.x;
	const float top = bounds.position.y;
	const float bottom = top + bounds.size.y;

	const TileRange cols = ComputeTileRange(
		std::min(left, left + deltaX),
		std::max(right, right + deltaX),
		map.GetWidth()
	);
	const TileRange rows = ComputeTileRange(top, bottom, map.GetHeight());
	if (cols.IsEmpty() || rows.IsEmpty())
	{
		return result;
	}

	const float distance = std::abs(deltaX);
	float nearest = distance;

	for (int row = rows.first; row <= rows.last; ++row)
	{
		for (int col = cols.first; col <= cols.last; ++col)
		{
			if (!GetSolidTile(map, col, row))
			{
				continue;
			}

			const float tileLeft = static_cast<float>(col) * tileSize;
			const float tileRight = tileLeft + tileSize;

			const float gap = deltaX > 0.f ? tileLeft - right : left - tileRight;
			if (gap < -COLLISION_EPSILON || gap > nearest)
			{
				continue;
			}

			nearest = std::max(0.f, gap);
			result.collided = true;
		}
	}

	if (result.collided)
	{
		result.allowedMovement = deltaX > 0.f ? nearest : -nearest;
		result.normal = deltaX > 0.f ? -1.f : 1.f;
	}

	return result;
}

AxisCollisionResult CollisionSystem::ResolveVertical(
	const sf::FloatRect& bounds,
	const Map& map,
	const float deltaY
) const
{
	AxisCollisionResult result;
	result.allowedMovement = deltaY;

	if (!std::isfinite(deltaY))
	{
		result.allowedMovement = 0.f;
		return result;
	}

	const float tileSize = static_cast<float>(TILE_SIZE);
	const float left = bounds.position.x;
	const float right = left + bounds.size.x;
	const float top = bounds.position.y;
	const float bottom = top + bounds.size.y;

	const bool isGroundProbe = deltaY == 0.f;
	const float sweepDown = isGroundProbe ? GROUND_CONTACT_TOLERANCE : std::max(0.f, deltaY);
	const float sweepUp = std::max(0.f, -deltaY);

	const TileRange rows = ComputeTileRange(top - sweepUp, bottom + sweepDown, map.GetHeight());
	const TileRange cols = ComputeTileRange(left, right, map.GetWidth());
	if (cols.IsEmpty() || rows.IsEmpty())
	{
		return result;
	}

	const bool movingDown = deltaY > 0.f || isGroundProbe;
	const float distance = isGroundProbe ? GROUND_CONTACT_TOLERANCE : std::abs(deltaY);
	float nearest = distance;
	float bestOverlap = -1.f;
	float friction = 0.f;
	bool found = false;

	for (int row = rows.first; row <= rows.last; ++row)
	{
		for (int col = cols.first; col <= cols.last; ++col)
		{
			const TileProperties* tile = GetSolidTile(map, col, row);
			if (!tile)
			{
				continue;
			}

			const float tileLeft = static_cast<float>(col) * tileSize;
			const float tileTop = static_cast<float>(row) * tileSize;
			const float tileBottom = tileTop + tileSize;

			const float gap = movingDown ? tileTop - bottom : top - tileBottom;
			if (gap < -COLLISION_EPSILON || gap > nearest + COLLISION_EPSILON)
			{
				continue;
			}

			const float overlap = Overlap(left, right, tileLeft, tileLeft + tileSize);
			if (!found || gap < nearest - COLLISION_EPSILON)
			{
				nearest = std::max(0.f, gap);
				bestOverlap = overlap;
				friction = tile->friction;
				found = true;
			}
			else if (overlap > bestOverlap)
			{
				bestOverlap = overlap;
				friction = tile->friction;
			}
		}
	}

	if (!found)
	{
		return result;
	}

	if (movingDown)
	{
		result.isGrounded = true;
		result.groundFriction = friction;
	}

	if (!isGroundProbe)
	{
		result.allowedMovement = movingDown ? nearest : -nearest;
		result.collided = true;
		result.normal = movingDown ? -1.f : 1.f;
	}

	return result;
}

const TileProperties* CollisionSystem::GetSolidTile(const Map& map, const int col, const int row) const
{
	const TileProperties& properties = GetTileProperties(map.GetTileAtCell(col, row));
	return properties.isSolid ? &properties : nullptr;
}
