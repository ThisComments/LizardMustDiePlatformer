
#pragma once

enum class PlayerInputEventType
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
