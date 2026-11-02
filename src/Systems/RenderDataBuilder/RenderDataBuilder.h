#pragma once

#include "Systems/RenderDataBuilder/RenderSnapshot.h"

class World;

class RenderDataBuilder
{
public:
	RenderSnapshot Build(const World& world) const;
};
