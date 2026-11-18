#include "World/World.h"

#include <algorithm>
#include <utility>

#include "Model/Collectible/CollectibleConstants.h"
#include "World/EntityContext.h"

namespace
{
	constexpr float WORLD_GRAVITY = 1800.f;
	constexpr float CELL_CENTER_OFFSET = 0.5f;
	constexpr float PIT_DEFEAT_MARGIN = 100.f;

	sf::Vector2f CellToWorldCenter(const CellCoordinates cell)
	{
		const float tileSize = static_cast<float>(TILE_SIZE);
		return {
			(static_cast<float>(cell.col) + CELL_CENTER_OFFSET) * tileSize,
			(static_cast<float>(cell.row) + CELL_CENTER_OFFSET) * tileSize
		};
	}

	sf::FloatRect ZoneToWorldRect(const TileZone& zone)
	{
		const float tileSize = static_cast<float>(TILE_SIZE);
		return sf::FloatRect(
			{
				static_cast<float>(zone.col) * tileSize,
				static_cast<float>(zone.row) * tileSize
			},
			{
				static_cast<float>(zone.width) * tileSize,
				static_cast<float>(zone.height) * tileSize
			}
		);
	}
}

World::World(LevelData level)
	: 	m_map(std::move(level.map)),
		m_bounds(
			{ 0.f, 0.f },
			{
				static_cast<float>(m_map.GetWidth() * TILE_SIZE),
				static_cast<float>(m_map.GetHeight() * TILE_SIZE)
			}
		),
		m_finishZone(ZoneToWorldRect(level.finishZone)),
		m_player(nullptr),
		m_resultReported(false)
{
	CreateEntities(level);
	ApplyPendingSpawns();
	SyncPlayerStats();
}

void World::CreateEntities(const LevelData& level)
{
	auto playerEntity = std::make_unique<Player>(CellToWorldCenter(level.playerSpawn));
	m_player = playerEntity.get();
	m_pendingSpawns.push_back(std::move(playerEntity));

	for (const CollectibleSpawn& spawn : level.collectibles)
	{
		const sf::Vector2f center = CellToWorldCenter({ spawn.col, spawn.row });
		std::unique_ptr<Collectible> collectible = CreateCollectible(spawn.type, center);
		if (collectible != nullptr)
		{
			m_pendingSpawns.push_back(std::move(collectible));
		}
	}
}

std::unique_ptr<Collectible> World::CreateCollectible(
	const CollectibleType type,
	const sf::Vector2f position
) const
{
	switch (type)
	{
		case COLLECTIBLE_COIN:
			return std::make_unique<Collectible>(
				position,
				CollectibleConstants::HITBOX_SIZE,
				VISUAL_COIN,
				CollectibleEffect{ EFFECT_SCORE, CollectibleConstants::COIN_SCORE_VALUE, 0.f }
			);

		case COLLECTIBLE_GEM:
			return std::make_unique<Collectible>(
				position,
				CollectibleConstants::HITBOX_SIZE,
				VISUAL_GEM,
				CollectibleEffect{ EFFECT_SCORE, CollectibleConstants::GEM_SCORE_VALUE, 0.f }
			);

		case COLLECTIBLE_SPEED_POTION:
			return std::make_unique<Collectible>(
				position,
				CollectibleConstants::HITBOX_SIZE,
				VISUAL_SPEED_POTION,
				CollectibleEffect{ EFFECT_SPEED_BOOST, CollectibleConstants::DEFAULT_SPEED_BOOST_MULTIPLIER, CollectibleConstants::DEFAULT_SPEED_BOOST_DURATION }
			);
	}

	return nullptr;
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

	ProcessCollectibles();
	SyncPlayerStats();
	ReportResult();

	ApplyPendingSpawns();
	RemoveFinishedEntities();
}

void World::ProcessCollectibles()
{
	const std::vector<Collectible*> triggered =
		m_triggerSystem.FindTriggeredCollectibles(*m_player, m_entities);

	for (Collectible* collectible : triggered)
	{
		ActivateCollectible(*collectible);
	}
}

void World::ActivateCollectible(Collectible& collectible)
{
	if (collectible.IsCollected())
	{
		return;
	}

	for (const CollectibleEffect& effect : collectible.GetEffects())
	{
		ApplyCollectibleEffect(effect);
	}

	collectible.MarkCollected();
}

void World::ApplyCollectibleEffect(const CollectibleEffect& effect)
{
	switch (effect.type)
	{
		case EFFECT_NONE:
			break;

		case EFFECT_SCORE:
		{
			const int points = static_cast<int>(effect.value);
			m_stats.score += points;
			m_events.push_back({ EVENT_COLLECTIBLE_COLLECTED, points });
			break;
		}

		case EFFECT_HEAL:
		{
			const int healAmount = static_cast<int>(effect.value);
			if (m_player->Heal(healAmount))
			{
				m_events.push_back({ EVENT_PLAYER_HEALED, healAmount });
			}
			break;
		}

		case EFFECT_SPEED_BOOST:
			m_player->ApplySpeedBoost(effect.value, effect.duration);
			m_events.push_back({ EVENT_COLLECTIBLE_COLLECTED, 0 });
			break;
	}
}

void World::SyncPlayerStats()
{
	m_stats.playerHealth = m_player->GetHealth();
	m_stats.playerMaxHealth = m_player->GetMaxHealth();
	m_stats.healCooldown = m_player->GetHealCooldown();
	m_stats.dashCooldown = m_player->GetDashCooldown();
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

void World::ReportResult()
{
	if (m_resultReported)
	{
		return;
	}

	const float lowerBoundary = m_bounds.position.y + m_bounds.size.y;
	if (!m_player->IsAlive() || m_player->GetPosition().y > lowerBoundary + PIT_DEFEAT_MARGIN)
	{
		m_resultReported = true;
		m_events.push_back({ EVENT_PLAYER_DEFEATED, 0 });
	}
	else if (m_player->GetBounds().findIntersection(m_finishZone))
	{
		m_resultReported = true;
		m_events.push_back({ EVENT_VICTORY_REACHED, 0 });
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
