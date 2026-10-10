#pragma once
#include "Domain/Map/Map.h"
#include "Domain/Player/Player.h"

struct LiquidResult
{
	bool isInLiquid = false;
	MovementModifiers modifiers;
};

class LiquidSystem
{
public:
	LiquidResult Resolve(const Player& player, const Map& map) const;
};
