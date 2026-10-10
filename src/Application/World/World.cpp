#include "World.h"
#include "Domain/Liquid/LiquidTypes.h"

#include <algorithm>
#include <cstdint>

namespace
{
	const float WORLD_GRAVITY = 1800.f;
}

World::World()
	: m_gravity(WORLD_GRAVITY)
{
	MapInit(m_map);
	m_bounds = sf::FloatRect(
		{0.f, 0.f},
		{
			static_cast<float>(m_map.width * TILE_SIZE),
			static_cast<float>(m_map.height * TILE_SIZE)
		}
	);

	m_playerEnvironment.gravity = m_gravity;
	m_playerEnvironment.movementModifiers = DEFAULT_PROPERTIES.movementModifiers;
}

void World::Update(PlayerInput& input, const float dt)
{
	if (dt <= 0.f)
	{
		return;
	}

	m_player.Update(input, m_playerEnvironment, dt);

	const sf::Vector2f velocity = m_player.GetVelocity();
	const AxisCollisionResult horizontalResult = m_collisionSystem.ResolveHorizontal(
		m_player,
		m_map,
		velocity.x * dt
	);
	m_player.MoveBy({horizontalResult.allowedMovement, 0.f});
	if (horizontalResult.collided)
	{
		m_player.ResolveHorizontalCollision();
	}

	const float verticalMovement = m_player.GetVelocity().y * dt;
	const AxisCollisionResult verticalResult = m_collisionSystem.ResolveVertical(
		m_player,
		m_map,
		verticalMovement
	);
	m_player.MoveBy({0.f, verticalResult.allowedMovement});
	if (verticalResult.collided)
	{
		m_player.ResolveVerticalCollision(verticalMovement);
	}

	const CollisionResult collisionResult{
		verticalResult.isGrounded,
		verticalResult.groundFriction
	};
	const LiquidResult liquidResult = m_liquidSystem.Resolve(m_player, m_map);

	m_playerEnvironment = BuildEnvironmentState(collisionResult, liquidResult);
	m_player.ApplyEnvironmentState(m_playerEnvironment);
}

EnvironmentState World::BuildEnvironmentState(
	const CollisionResult& collisionResult,
	const LiquidResult& liquidResult
) const
{
	EnvironmentState environmentState;
	environmentState.gravity = m_gravity;
	environmentState.isGrounded = collisionResult.isGrounded;
	environmentState.groundFriction = collisionResult.groundFriction;
	environmentState.movementModifiers = DEFAULT_PROPERTIES.movementModifiers;

	if (liquidResult.isInLiquid)
	{
		environmentState.movementModifiers = liquidResult.modifiers;
	}

	return environmentState;
}

void World::Draw(sf::RenderTarget& window) const
{
	DrawMap(window);
	m_player.Draw(window);
}

void World::DrawMap(sf::RenderTarget& window) const
{
	const float tileSize = static_cast<float>(TILE_SIZE);

	for (std::uint32_t row = 0; row < m_map.height; ++row)
	{
		for (std::uint32_t col = 0; col < m_map.width; ++col)
		{
			const std::uint32_t index = row * m_map.width + col;
			if (index >= m_map.visualIds.size())
			{
				continue;
			}

			const std::uint32_t visualId = m_map.visualIds[index];
			if (visualId == TVI_NONE)
			{
				continue;
			}

			sf::RectangleShape tile({tileSize, tileSize});
			tile.setPosition({static_cast<float>(col) * tileSize, static_cast<float>(row) * tileSize});

			switch (visualId)
			{
			case TVI_GROUND:
				tile.setFillColor(sf::Color(158, 106, 54));
				break;
			case TVI_STONE:
				tile.setFillColor(sf::Color(111, 111, 111));
				break;
			case TVI_WATER:
				tile.setFillColor(sf::Color(0, 80, 255, 150));
				break;
			default:
				tile.setFillColor(sf::Color::Magenta);
				break;
			}

			window.draw(tile);
		}
	}
}

sf::FloatRect World::GetBounds() const
{
	return m_bounds;
}

sf::Vector2f World::GetPositionPlayer() const
{
	return m_player.GetPosition();
}
