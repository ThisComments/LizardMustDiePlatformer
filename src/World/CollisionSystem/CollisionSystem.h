#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "World/Map/Map.h"

struct TileProperties;

struct AxisCollisionResult
{
	float allowedMovement = 0.f;
	bool collided = false;
	float normal = 0.f;
	bool isGrounded = false;
	float groundFriction = 0.f;
};

struct CollisionResult
{
	bool isGrounded = false;
	float groundFriction = 0.f;
};

class CollisionSystem
{
public:
	AxisCollisionResult ResolveHorizontal(const sf::FloatRect& bounds, const Map& map, float deltaX) const;
	AxisCollisionResult ResolveVertical(const sf::FloatRect& bounds, const Map& map, float deltaY) const;

private:
	const TileProperties* GetSolidTile(const Map& map, int col, int row) const;
};
