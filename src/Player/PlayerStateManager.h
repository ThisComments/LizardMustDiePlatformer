
#pragma once

#include <deque>

#include "PlayerInputEvent.h"
#include "PlayerState.h"

struct PlayerStateActions
{
    bool jumpStarted = false;
    bool airJumpStarted = false;
    bool dashStarted = false;

    float dashDirection = 0.f;

    bool AnyStarted() const
    {
        return jumpStarted || dashStarted;
    }
};

class PlayerStateManager
{
private:
    PlayerState m_state;
    std::deque<PlayerInputEvent> m_inputQueue;

    void HandleEvent(
        const PlayerInputEvent& event,
        bool allowsJump,
        bool allowsDash,
        PlayerStateActions& actions
    );
    void UpdateFacing();
    void TryStartJump(bool allowsJump, PlayerStateActions& actions);
    void TryStartDash(bool allowsDash, PlayerStateActions& actions);

public:
    void PushInput(const PlayerInputEvent& event);
    PlayerStateActions ProcessInputQueue(bool allowsJump, bool allowsDash);
    void UpdateTimers(float dt, bool dashStartedThisFrame);
    void ApplyPhysicsResult(bool isGrounded, float verticalVelocity);
    void StopDash();

    const PlayerState& GetState() const;
};
