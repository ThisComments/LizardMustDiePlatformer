#include "CollisionSystem.h"

const float GROUND_EPSILON = 1.f;
const float MAX_STEP_HEIGHT_PERCENT = 0.25f;

void CollisionSystem::Resolve(Player& player, const std::vector<Obstacle>& obstacles)
{
    for (const Obstacle& obstacle : obstacles)
    {
        sf::FloatRect playerBounds = player.GetBounds();
        sf::FloatRect obstacleBounds = obstacle.GetBounds();

        auto intersection = playerBounds.findIntersection(obstacleBounds);

        if (!intersection)
        {
            continue;
        }

        sf::Vector2f position = player.GetPosition();
        sf::Vector2f velocity = player.GetVelocity();
        
        if (intersection->size.x < intersection->size.y)
        {
            if (TryStepUp(player, obstacle, obstacles))
            {
                continue;
            }
            player.StopDash();
            if (playerBounds.position.x < obstacleBounds.position.x)
            {
                position.x -= intersection->size.x;
                if (velocity.x > 0.f)
                {
                    velocity.x = 0.f;
                }
            }
            else
            {
                position.x += intersection->size.x;
                if (velocity.x < 0.f)
                {
                    velocity.x = 0.f;
                }
            }
        }
        else
        {
            if (playerBounds.position.y < obstacleBounds.position.y)
            {
                position.y -= intersection->size.y;
                if (velocity.y > 0.f)
                {
                    velocity.y = 0.f;
                }
            }
            else
            {
                position.y += intersection->size.y;
                if (velocity.y < 0.f)
                {
                    velocity.y = 0.f;
                }
            }
        }

        player.SetPosition(position);
        player.SetVelocity(velocity);
    }
}

bool CollisionSystem::TryStepUp(
    Player& player,
    const Obstacle& obstacle,
    const std::vector<Obstacle>& obstacles)
{
    if (!player.GetIsGrounded())
    {
        return false;
    }

    sf::FloatRect playerBounds = player.GetBounds();
    sf::FloatRect obstacleBounds = obstacle.GetBounds();

    const float stepHeight = playerBounds.position.y + playerBounds.size.y - obstacleBounds.position.y;

    const float maxStepHeight = playerBounds.size.y * MAX_STEP_HEIGHT_PERCENT;

    if (stepHeight < 0.f || stepHeight > maxStepHeight)
    {
        return false;
    }

    sf::FloatRect stepBounds = playerBounds;
    stepBounds.position.y -= stepHeight;

    for (const Obstacle& otherObstacle : obstacles)
    {
        if (&otherObstacle == &obstacle)
        {
            continue;
        }

        if (stepBounds.findIntersection(otherObstacle.GetBounds()))
        {
            return false;
        }
    }

    sf::Vector2f position = player.GetPosition();
    position.y -= stepHeight;

    player.SetPosition(position);

    return true;
}

bool CollisionSystem::IsGrounded(const Player& player, const std::vector<Obstacle>& obstacles) const
{
    const sf::FloatRect playerBounds = player.GetBounds();

    const float playerBottom = playerBounds.position.y + playerBounds.size.y;

    for (const Obstacle& obstacle : obstacles)
    {
        const sf::FloatRect obstacleBounds = obstacle.GetBounds();

        const float obstacleTop = obstacleBounds.position.y;

        const bool overlapsX =
            playerBounds.position.x <
                obstacleBounds.position.x + obstacleBounds.size.x &&
            playerBounds.position.x + playerBounds.size.x >
                obstacleBounds.position.x;

        if (overlapsX && std::abs(playerBottom - obstacleTop) <= GROUND_EPSILON)
        {
            return true;
        }
    }

    return false;
}