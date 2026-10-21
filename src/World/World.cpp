#include "World.h"
#include "World/Map/TileProperties.h"

#include <algorithm>

const float WORLD_GRAVITY = 1800.f;

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
	m_playerEnvironment.movementModifiers = DEFAULT_MOVEMENT_MODIFIERS;
}

void World::PushPlayerInput(const PlayerInputEvent& event)
{
	m_player.PushInput(event);
}

void World::Update(const float dt)
{
	if (dt <= 0.f)
	{
		return;
	}
	m_player.Update(m_playerEnvironment, dt);

	const sf::Vector2f velocity = m_player.GetVelocity();
	const AxisCollisionResult horizontalResult = m_collisionSystem.ResolveHorizontal(
		m_player,
		m_map,
		velocity.x * dt
	);
	m_player.MoveBy({horizontalResult.allowedMovement, 0.f});
	if (horizontalResult.collided)
	{
		m_player.ResolveHorizontalCollision(horizontalResult.normal);
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
		m_player.ResolveVerticalCollision(verticalResult.normal);
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
	environmentState.movementModifiers = DEFAULT_MOVEMENT_MODIFIERS;

	if (liquidResult.isInLiquid)
	{
		environmentState.movementModifiers = liquidResult.modifiers;
	}

	return environmentState;
}

void World::Draw(sf::RenderTarget& window) const
{
	m_render.Draw(m_map, window);
	m_player.Draw(window);
}

sf::FloatRect World::GetBounds() const
{
	return m_bounds;
}

sf::Vector2f World::GetPositionPlayer() const
{
	return m_player.GetPosition();
}
