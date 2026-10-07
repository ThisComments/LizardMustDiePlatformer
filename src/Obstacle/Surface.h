#pragma once

enum SurfaceVisualId
{
    SURFACE_VISUAL_GROUND,
	SURFACE_VISUAL_ICE,
	SURFACE_VISUAL_STONE
};

struct Surface
{
	float friction;
	SurfaceVisualId visualId;
};