#pragma once
#include <vector>
#include "../Obstacle/Obstacle.h"
#include "../Player/Player.h"

struct CollisionResult
{
	bool isGrounded;
	float groundFriction;
};

class CollisionSystem
{
public:
	CollisionResult Resolve(Player& player, const std::vector<Obstacle>& obstacles);

private:
	bool TryStepUp(Player& player, const Obstacle& obstacle, const std::vector<Obstacle>& obstacles);
	bool IsGroundBelow(const sf::FloatRect& playerBounds, const sf::FloatRect& obstacleBounds) const;
};