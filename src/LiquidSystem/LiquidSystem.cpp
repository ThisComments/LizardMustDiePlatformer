#include "LiquidSystem.h"

LiquidResult LiquidSystem::Resolve(const Player& player, const std::vector<Liquid>& liquids) const
{
	LiquidResult result;

	const sf::FloatRect playerBounds = player.GetBounds();

	for (const Liquid& liquid : liquids)
	{
		if (!playerBounds.findIntersection(liquid.GetBounds()))
		{
			continue;
		}

		result.isInLiquid = true;
		result.modifiers = liquid.GetModifier();

		break;
	}

	return result;
}