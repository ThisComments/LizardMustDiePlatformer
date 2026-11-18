#pragma once

#include <array>

#include "Resources/AssetPaths.h"

namespace ParallaxConstants
{
	struct ParallaxLayerConfig
	{
		const char *texturePath;
		float parallaxFactor;
		bool isHorizontalRepeat;
	};

	constexpr std::array<ParallaxLayerConfig, 3> LAYERS =
	{{
		{AssetPaths::ParallaxLayer1, 0.8f, true},
		{AssetPaths::ParallaxLayer2, 0.5f, true},
		{AssetPaths::ParallaxLayer3, 0.2f, false}
	}};
}
