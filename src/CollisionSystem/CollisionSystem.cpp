#include "CollisionSystem.h"

#include <algorithm>
#include <cmath>

namespace
{
	const float MAX_STEP_HEIGHT_PERCENT = 0.25f;
	const float GROUND_EPSILON = 1.f;
}

CollisionResult CollisionSystem::Resolve(Player& player, const std::vector<Obstacle>& obstacles)
{
	CollisionResult result{false, 0.f};

	sf::Vector2f position = player.GetPosition();
	sf::Vector2f velocity = player.GetVelocity();

	sf::FloatRect playerBounds = player.GetBounds();

	for (const Obstacle& obstacle : obstacles)
	{
		sf::FloatRect obstacleBounds = obstacle.GetBounds();

		if (IsGroundBelow(playerBounds, obstacleBounds))
		{
			result.isGrounded = true;
			result.groundFriction = obstacle.GetFriction();
		}

		auto intersection = playerBounds.findIntersection(obstacleBounds);

		if (!intersection)
		{
			continue;
		}

		if (intersection->size.x < intersection->size.y)
		{
			if (TryStepUp(player, obstacle, obstacles))
			{
				position = player.GetPosition();
				playerBounds = player.GetBounds();

				continue;
			}

			const float playerCenterX = playerBounds.position.x + playerBounds.size.x / 2.f;

			const float obstacleCenterX = obstacleBounds.position.x + obstacleBounds.size.x / 2.f;

			if (playerCenterX < obstacleCenterX)
			{
				position.x -= intersection->size.x;

				if (velocity.x > 0.f)
				{
					velocity.x = 0.f;
					player.StopDash();
				}
			}
			else
			{
				position.x += intersection->size.x;

				if (velocity.x < 0.f)
				{
					velocity.x = 0.f;
					player.StopDash();
				}
			}

			player.SetPosition(position);
			player.SetVelocity(velocity);

			playerBounds = player.GetBounds();
		}
		else
		{
			const float playerCenterY = playerBounds.position.y + playerBounds.size.y / 2.f;

			const float obstacleCenterY = obstacleBounds.position.y + obstacleBounds.size.y / 2.f;

			if (playerCenterY < obstacleCenterY)
			{
				position.y -= intersection->size.y;

				result.isGrounded = true;
				result.groundFriction = obstacle.GetFriction();

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

			player.SetPosition(position);
			player.SetVelocity(velocity);

			playerBounds = player.GetBounds();
		}
	}

	return result;
}

bool CollisionSystem::TryStepUp(Player& player, const Obstacle& obstacle, const std::vector<Obstacle>& obstacles)
{
	if (!player.GetIsGrounded())
	{
		return false;
	}

	sf::FloatRect playerBounds = player.GetBounds();
	sf::FloatRect obstacleBounds = obstacle.GetBounds();

	const float playerBottom = playerBounds.position.y + playerBounds.size.y;

	const float stepHeight = playerBottom - obstacleBounds.position.y;

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

bool CollisionSystem::IsGroundBelow(const sf::FloatRect& playerBounds, const sf::FloatRect& obstacleBounds) const
{
	const float playerBottom = playerBounds.position.y + playerBounds.size.y;
	const float obstacleTop = obstacleBounds.position.y;

	const bool isVerticallyClose = std::abs(playerBottom - obstacleTop) <= GROUND_EPSILON;

	const float playerLeft = playerBounds.position.x;
	const float playerRight = playerBounds.position.x + playerBounds.size.x;

	const float obstacleLeft = obstacleBounds.position.x;
	const float obstacleRight = obstacleBounds.position.x + obstacleBounds.size.x;

	const bool hasHorizontalOverlap = playerRight > obstacleLeft && playerLeft < obstacleRight;

	return isVerticallyClose && hasHorizontalOverlap;
}