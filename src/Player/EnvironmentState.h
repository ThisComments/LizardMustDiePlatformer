#pragma once

#include "Common/PhysicsTypes.h"

struct EnvironmentState
{
	bool isGrounded = false;
	float gravity = 0.f;
	float groundFriction = 0.f;
	MovementModifiers movementModifiers;
};