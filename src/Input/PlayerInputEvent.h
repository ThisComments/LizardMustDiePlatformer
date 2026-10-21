
#pragma once

enum class PlayerInputEventType: int
{
    MoveLeftPressed,
    MoveLeftReleased,
    MoveRightPressed,
    MoveRightReleased,
    JumpPressed,
    JumpReleased,
    DashPressed
};

struct PlayerInputEvent
{
    PlayerInputEventType type;
};
