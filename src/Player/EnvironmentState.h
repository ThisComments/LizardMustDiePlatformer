#pragma once

struct MovementModifiers
{
	float gravityScale = 1.f;
	float movementSpeedScale = 1.f;
	bool allowsJump = true;
	bool allowsDash = true;
};

struct EnvironmentState
{
	bool isGrounded = false;
	float gravity = 0.f;
	float groundFriction = 0.f;
	MovementModifiers movementModifiers;
};