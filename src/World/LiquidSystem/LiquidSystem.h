#pragma once
#include "World/Map/Map.h"
#include "Player/Player.h"

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
