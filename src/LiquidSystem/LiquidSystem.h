#pragma once
#include <vector>
#include "Liquid.h"
#include "../Player/Player.h"

struct LiquidResult
{
	bool isInLiquid = false;
	MovementModifiers modifiers;
};

class LiquidSystem
{
public:
	LiquidResult Resolve(const Player& player, const std::vector<Liquid>& liquids) const;
};