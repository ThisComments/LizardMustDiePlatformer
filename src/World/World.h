#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

#include "Infrastructure/Input/PlayerInputEvent.h"
#include "Infrastructure/LevelRepository/LevelData.h"
#include "Model/Collectible/Collectible.h"
#include "Model/Entity/Entity.h"
#include "Model/Map/Map.h"
#include "Model/Player/Player.h"
#include "Systems/CollisionSystem/CollisionSystem.h"
#include "Systems/LiquidSystem/LiquidSystem.h"
#include "Systems/TriggerSystem/TriggerSystem.h"
#include "World/WorldEvent.h"
#include "World/WorldStats.h"

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

	void CreateEntities(const LevelData& level);
	std::unique_ptr<Collectible> CreateCollectible(
		CollectibleType type,
		sf::Vector2f position
	) const;
	void ProcessCollectibles();
	void ActivateCollectible(Collectible& collectible);
	void ApplyCollectibleEffect(const CollectibleEffect& effect);
	void SyncPlayerStats();
	void ApplyPendingSpawns();
	void RemoveFinishedEntities();
	void ReportResult();

public:
	explicit World(LevelData level);

	World(const World&) = delete;
	World& operator=(const World&) = delete;

	void PushPlayerInput(const PlayerInputEvent& event);
	void Update(float dt);

	std::vector<WorldEvent> DrainEvents();

	const Map& GetMap() const;
	const std::vector<std::unique_ptr<Entity>>& GetEntities() const;
	const Player& GetPlayer() const;
	const WorldStats& GetStats() const;
	sf::FloatRect GetBounds() const;
};
