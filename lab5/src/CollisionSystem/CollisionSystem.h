#pragma once

#include <vector>
#include "Player.h"
#include "Obstacle.h"

class CollisionSystem
{
public:
    void Resolve(Player& player, const std::vector<Obstacle>& obstacles);
    bool IsGrounded(const Player& player, const std::vector<Obstacle>& obstacles) const;

private:
    bool TryStepUp(Player& player, const Obstacle& obstacle, const std::vector<Obstacle>& obstacles);
};