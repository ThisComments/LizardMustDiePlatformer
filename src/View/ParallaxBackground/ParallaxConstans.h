#pragma once

#include <vector>

#include "View/RenderConstants.h"

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
		{RenderConstants::ParallaxLayer1, 0.8f, true},
		{RenderConstants::ParallaxLayer2, 0.5f, true},
		{RenderConstants::ParallaxLayer3, 0.2f, false}
	}};
}