#pragma once

enum VerticalState
{
	VERTICAL_GROUNDED,
	VERTICAL_RISING,
	VERTICAL_FALLING,
};

enum MovementMode
{
	MOVEMENT_NORMAL,
	MOVEMENT_DASHING,
};

struct PlayerState
{
	VerticalState verticalState = VERTICAL_FALLING;
	MovementMode movementMode = MOVEMENT_NORMAL;

	bool moveLeftHeld = false;
	bool moveRightHeld = false;
	float facingDirection = 1.f;

	bool jumpHeld = false;
	bool canAirJump = false;
	bool canAirDash = false;

	float dashDirection = 1.f;
	float dashTimer = 0.f;
	float dashCooldown = 0.f;
	float healCooldown = 0.f;

	bool IsGrounded() const
	{
		return verticalState == VERTICAL_GROUNDED;
	}

	bool IsDashing() const
	{
		return movementMode == MOVEMENT_DASHING;
	}

	float MoveDirection() const
	{
		return (moveRightHeld ? 1.f : 0.f) - (moveLeftHeld ? 1.f : 0.f);
	}
};
