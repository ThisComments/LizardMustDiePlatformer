#pragma once

enum CollectibleType
{
	COLLECTIBLE_COIN,
	COLLECTIBLE_GEM,
	COLLECTIBLE_SPEED_POTION
};

enum CollectibleEffectType
{
	EFFECT_NONE,
	EFFECT_SCORE,
	EFFECT_HEAL,
	EFFECT_SPEED_BOOST,
};

struct CollectibleEffect
{
	CollectibleEffectType type = EFFECT_NONE;
	float value = 0.f;
	float duration = 0.f;
};
