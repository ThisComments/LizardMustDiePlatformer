#include "World/World.h"

#include <utility>

const float WORLD_GRAVITY = 1800.f;
const sf::Vector2f PLAYER_START_POSITION = {300.f, 250.f};

World::World(Map map, const sf::Vector2f playerCollisionSize)
	: m_map(std::move(map)),
	  m_bounds(
		{0.f, 0.f},
		{
			static_cast<float>(m_map.GetWidth() * TILE_SIZE),
			static_cast<float>(m_map.GetHeight() * TILE_SIZE)
		}
	  ),
	  m_gravity(WORLD_GRAVITY),
	  m_player(PLAYER_START_POSITION, playerCollisionSize)
{
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

	MovePlayerHorizontally(dt);
	const AxisCollisionResult verticalResult = MovePlayerVertically(dt);

	const CollisionResult collisionResult{
		verticalResult.isGrounded,
		verticalResult.groundFriction
	};
	const LiquidResult liquidResult = m_liquidSystem.Resolve(m_player.GetBounds(), m_map);

	m_playerEnvironment = BuildEnvironmentState(collisionResult, liquidResult);
	m_player.ApplyEnvironmentState(m_playerEnvironment);
}

void World::MovePlayerHorizontally(const float dt)
{
	const sf::Vector2f velocity = m_player.GetVelocity();
	const AxisCollisionResult result = m_collisionSystem.ResolveHorizontal(
		m_player.GetBounds(),
		m_map,
		velocity.x * dt
	);
	m_player.MoveBy({result.allowedMovement, 0.f});
	if (result.collided)
	{
		m_player.ResolveHorizontalCollision(result.normal);
	}
}

AxisCollisionResult World::MovePlayerVertically(const float dt)
{
	const float verticalMovement = m_player.GetVelocity().y * dt;
	const AxisCollisionResult result = m_collisionSystem.ResolveVertical(
		m_player.GetBounds(),
		m_map,
		verticalMovement
	);
	m_player.MoveBy({0.f, result.allowedMovement});
	if (result.collided)
	{
		m_player.ResolveVerticalCollision(result.normal);
	}
	return result;
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

const Map& World::GetMap() const
{
	return m_map;
}

const Player& World::GetPlayer() const
{
	return m_player;
}

sf::FloatRect World::GetBounds() const
{
	return m_bounds;
}

sf::Vector2f World::GetPositionPlayer() const
{
	return m_player.GetPosition();
}
