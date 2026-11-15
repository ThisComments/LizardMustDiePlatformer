#include "World/World.h"

#include <algorithm>
#include <cmath>
#include <utility>

#include "Model/Collectible/Collectible.h"
#include "Model/Collectible/CollectibleDefs.h"
#include "Resources/AssetPaths.h"
#include "World/EntityContext.h"

namespace
{
	constexpr float WORLD_GRAVITY = 1800.f;
	constexpr sf::Vector2f PLAYER_START_POSITION = {300.f, 250.f};
	constexpr sf::Vector2f PLAYER_COLLISION_SIZE = {40.f, 80.f};
	constexpr int FINISH_ZONE_WIDTH_TILES = 2;
	constexpr float PIT_DEFEAT_MARGIN = 100.f;

	// Spreads the idle animations of consecutive collectibles over the clip's duration.
	constexpr float COLLECTIBLE_PHASE_STEP = 0.173f;
}

World::World(LevelData level, ResourceManager& resources)
	: m_map(std::move(level.map)),
	  m_bounds(
		  {0.f, 0.f},
		  {
			  static_cast<float>(m_map.GetWidth() * TILE_SIZE),
			  static_cast<float>(m_map.GetHeight() * TILE_SIZE)
		  }
	  ),
	  m_finishZone(
		  {
			  m_bounds.size.x - static_cast<float>(FINISH_ZONE_WIDTH_TILES * TILE_SIZE),
			  m_bounds.position.y
		  },
		  {
			  static_cast<float>(FINISH_ZONE_WIDTH_TILES * TILE_SIZE),
			  m_bounds.size.y
		  }
	  ),
	  m_player(nullptr),
	  m_resultReported(false)
{
	CreateEntities(level, resources);
	ApplyPendingSpawns();
}

void World::CreateEntities(const LevelData& level, ResourceManager& resources)
{
	auto player = std::make_unique<Player>(
		PLAYER_START_POSITION,
		PLAYER_COLLISION_SIZE,
		resources.GetSpriteSheet(AssetPaths::HeroSheet)
	);
	// The pointer is taken before the move; the object itself never changes its address.
	m_player = player.get();
	m_pendingSpawns.push_back(std::move(player));

	float phase = 0.f;
	for (const CollectibleSpawn& spawn : level.collectibles)
	{
		const sf::Vector2f center(
			(static_cast<float>(spawn.col) + 0.5f) * static_cast<float>(TILE_SIZE),
			(static_cast<float>(spawn.row) + 0.5f) * static_cast<float>(TILE_SIZE)
		);
		const SpriteSheetAsset& sheet = resources.GetSpriteSheet(GetCollectibleDef(spawn.type).spriteSheetPath);

		m_pendingSpawns.push_back(std::make_unique<Collectible>(spawn.type, center, sheet, phase));
		phase = std::fmod(phase + COLLECTIBLE_PHASE_STEP, 1.f);
	}
}

void World::PushPlayerInput(const PlayerInputEvent& event)
{
	m_player->PushInput(event);
}

void World::Update(const float dt)
{
	if (dt <= 0.f)
	{
		return;
	}

	const std::size_t firstNewEvent = m_events.size();
	EntityContext context{
		m_map,
		m_collisionSystem,
		m_liquidSystem,
		WORLD_GRAVITY,
		m_events,
		m_pendingSpawns
	};

	for (const std::unique_ptr<Entity>& entity : m_entities)
	{
		entity->Update(dt, context);
	}

	// Triggers run after movement so pickups use the player's up-to-date position.
	m_triggerSystem.Update(*m_player, m_entities, context);

	UpdateStats(firstNewEvent);
	ReportResult();

	ApplyPendingSpawns();
	RemoveFinishedEntities();
}

std::vector<WorldEvent> World::DrainEvents()
{
	std::vector<WorldEvent> events;
	events.swap(m_events);
	return events;
}

void World::ApplyPendingSpawns()
{
	for (std::unique_ptr<Entity>& entity : m_pendingSpawns)
	{
		m_entities.push_back(std::move(entity));
	}
	m_pendingSpawns.clear();
}

void World::RemoveFinishedEntities()
{
	m_entities.erase(
		std::remove_if(
			m_entities.begin(),
			m_entities.end(),
			[](const std::unique_ptr<Entity>& entity)
			{
				return entity->IsPendingRemoval();
			}
		),
		m_entities.end()
	);
}

void World::UpdateStats(const std::size_t firstNewEvent)
{
	for (std::size_t i = firstNewEvent; i < m_events.size(); ++i)
	{
		if (m_events[i].type == WorldEventType::CollectibleCollected)
		{
			m_stats.score += m_events[i].value;
		}
	}
}

void World::ReportResult()
{
	if (m_resultReported)
	{
		return;
	}

	const float lowerBoundary = m_bounds.position.y + m_bounds.size.y;
	if (m_player->GetPosition().y > lowerBoundary + PIT_DEFEAT_MARGIN)
	{
		m_resultReported = true;
		m_events.push_back({WorldEventType::PlayerDefeated});
	}
	else if (m_player->GetBounds().findIntersection(m_finishZone))
	{
		m_resultReported = true;
		m_events.push_back({WorldEventType::VictoryReached});
	}
}

const Map& World::GetMap() const
{
	return m_map;
}

const std::vector<std::unique_ptr<Entity>>& World::GetEntities() const
{
	return m_entities;
}

const Player& World::GetPlayer() const
{
	return *m_player;
}

const WorldStats& World::GetStats() const
{
	return m_stats;
}

sf::FloatRect World::GetBounds() const
{
	return m_bounds;
}
