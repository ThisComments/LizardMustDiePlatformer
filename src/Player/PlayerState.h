
#pragma once

enum class VerticalState
{
    Grounded,
    Rising,
    Falling
};

enum class MovementMode
{
    Normal,
    Dashing
};

struct PlayerState
{
    VerticalState verticalState = VerticalState::Falling;
    MovementMode movementMode = MovementMode::Normal;

    bool moveLeftHeld = false;
    bool moveRightHeld = false;
    float facingDirection = 1.f;

    bool jumpHeld = false;
    bool canAirJump = false;
    bool canAirDash = false;

    float dashDirection = 1.f;
    float dashTimer = 0.f;
    float dashCooldown = 0.f;

    bool IsGrounded() const
    {
        return verticalState == VerticalState::Grounded;
    }

    bool IsDashing() const
    {
        return movementMode == MovementMode::Dashing;
    }

    float MoveDirection() const
    {
        return (moveRightHeld ? 1.f : 0.f) - (moveLeftHeld ? 1.f : 0.f);
    }
};
