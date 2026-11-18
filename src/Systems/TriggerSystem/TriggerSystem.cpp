#include "Systems/TriggerSystem/TriggerSystem.h"

#include "Model/Collectible/Collectible.h"
#include "Model/Player/Player.h"

std::vector<Collectible*> TriggerSystem::FindTriggeredCollectibles(
	const Player& player,
	const std::vector<std::unique_ptr<Entity>>& entities
) const
{
	std::vector<Collectible*> triggered;
	const sf::FloatRect playerBounds = player.GetBounds();

	for (const std::unique_ptr<Entity>& entity : entities)
	{
		if (entity->IsPendingRemoval())
		{
			continue;
		}

		auto* collectible = dynamic_cast<Collectible*>(entity.get());
		if (collectible == nullptr || collectible->IsCollected())
		{
			continue;
		}

		if (playerBounds.findIntersection(collectible->GetTriggerBounds()))
		{
			triggered.push_back(collectible);
		}
	}

	return triggered;
}
