#pragma once
#include <SFML/Graphics/Rect.hpp>

#include "Common/PhysicsTypes.h"
#include "World/Map/Map.h"

struct LiquidResult
{
	bool isInLiquid = false;
	MovementModifiers modifiers;
};

class LiquidSystem
{
public:
	LiquidResult Resolve(const sf::FloatRect& bounds, const Map& map) const;
};
