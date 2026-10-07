#pragma once
#include <vector>
#include "Liquid.h"
#include "Player.h"

struct LiquidResult
{
	bool isInLiquid;
	LiquidProperties properties;
};

class LiquidSystem
{
public:
	LiquidResult Resolve(const Player& player, const std::vector<Liquid>& liquids) const;
};