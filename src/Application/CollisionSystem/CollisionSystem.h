#pragma once

#include "Domain//Map/Map.h"
#include "Domain//Player/Player.h"

struct AxisCollisionResult
{
	float allowedMovement = 0.f;
	bool collided = false;
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
	bool IsSolidTile(const Map& map, int col, int row) const;
	float GetTileFriction(const Map& map, int col, int row) const;
};
