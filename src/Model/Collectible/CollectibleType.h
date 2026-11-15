#pragma once

#include <cstdint>

enum class CollectibleType : std::uint8_t
{
	Coin,
	Gem
};

enum class CollectibleState : std::uint8_t
{
	Available,
	Collecting,
	Finished
};
