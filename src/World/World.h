#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

#include "Infrastructure/Input/PlayerInputEvent.h"
#include "Infrastructure/LevelRepository/LevelData.h"
#include "Model/Entity/Entity.h"
#include "Model/Map/Map.h"
#include "Model/Player/Player.h"
#include "Resources/ResourceManager.h"
#include "Systems/CollisionSystem/CollisionSystem.h"
#include "Systems/LiquidSystem/LiquidSystem.h"
#include "Systems/TriggerSystem/TriggerSystem.h"
#include "World/WorldEvent.h"
#include "World/WorldStats.h"

// State of one level: the map, every dynamic entity, statistics and the queue of events.
// Not copyable: it hands out a pointer to its player and entities reference its systems.
class World
{
private:
	Map m_map;
	sf::FloatRect m_bounds;
	sf::FloatRect m_finishZone;
	CollisionSystem m_collisionSystem;
	LiquidSystem m_liquidSystem;
	TriggerSystem m_triggerSystem;

	std::vector<std::unique_ptr<Entity>> m_entities;
	std::vector<std::unique_ptr<Entity>> m_pendingSpawns;
	Player* m_player;

	WorldStats m_stats;
	std::vector<WorldEvent> m_events;
	bool m_resultReported;

	void CreateEntities(const LevelData& level, ResourceManager& resources);
	void ApplyPendingSpawns();
	void RemoveFinishedEntities();
	void UpdateStats(std::size_t firstNewEvent);
	void ReportResult();

public:
	World(LevelData level, ResourceManager& resources);

	World(const World&) = delete;
	World& operator=(const World&) = delete;

	void PushPlayerInput(const PlayerInputEvent& event);
	void Update(float dt);

	// Returns the events produced since the last call and clears the queue.
	std::vector<WorldEvent> DrainEvents();

	const Map& GetMap() const;
	const std::vector<std::unique_ptr<Entity>>& GetEntities() const;
	const Player& GetPlayer() const;
	const WorldStats& GetStats() const;
	sf::FloatRect GetBounds() const;
};
