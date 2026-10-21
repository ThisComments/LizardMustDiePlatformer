#pragma once

#include "World/Map/Map.h"
#include "Player/Player.h"

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
	AxisCollisionResult ResolveHorizontal(const Player& player, const Map& map, float deltaX) const;
	AxisCollisionResult ResolveVertical(const Player& player, const Map& map, float deltaY) const;

private:
	const TileProperties* GetSolidTile(const Map& map, int col, int row) const;
};
