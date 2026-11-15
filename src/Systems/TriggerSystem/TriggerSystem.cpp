#include "Systems/TriggerSystem/TriggerSystem.h"

#include "Model/Entity/Triggerable.h"
#include "Model/Player/Player.h"

void TriggerSystem::Update(
	const Player& player,
	const std::vector<std::unique_ptr<Entity>>& entities,
	EntityContext& context
) const
{
	const sf::FloatRect playerBounds = player.GetBounds();

	for (const std::unique_ptr<Entity>& entity : entities)
	{
		auto* trigger = dynamic_cast<Triggerable*>(entity.get());
		if (trigger == nullptr)
		{
			continue;
		}

		if (playerBounds.findIntersection(trigger->GetTriggerBounds()))
		{
			trigger->OnPlayerEnter(context);
		}
	}
}
