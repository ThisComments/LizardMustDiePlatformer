
#include "PlayerStateManager.h"

#include <algorithm>

const float DASH_DURATION = 0.15f;
const float DASH_COOLDOWN = 2.f;
const float VERTICAL_SPEED_THRESHOLD = 0.001f;

void PlayerStateManager::PushInput(const PlayerInputEvent& event)
{
    m_inputQueue.push_back(event);
}

PlayerStateActions PlayerStateManager::ProcessInputQueue(
    const bool allowsJump,
    const bool allowsDash
)
{
    PlayerStateActions actions;

    while (!m_inputQueue.empty())
    {
        const PlayerInputEvent event = m_inputQueue.front();
        m_inputQueue.pop_front();

        HandleEvent(event, allowsJump, allowsDash, actions);
    }

    return actions;
}

void PlayerStateManager::HandleEvent(
    const PlayerInputEvent& event,
    const bool allowsJump,
    const bool allowsDash,
    PlayerStateActions& actions
)
{
    switch (event.type)
    {
        case PlayerInputEventType::MoveLeftPressed:
            m_state.moveLeftHeld = true;
            UpdateFacing();
            break;

        case PlayerInputEventType::MoveLeftReleased:
            m_state.moveLeftHeld = false;
            UpdateFacing();
            break;

        case PlayerInputEventType::MoveRightPressed:
            m_state.moveRightHeld = true;
            UpdateFacing();
            break;

        case PlayerInputEventType::MoveRightReleased:
            m_state.moveRightHeld = false;
            UpdateFacing();
            break;

        case PlayerInputEventType::JumpPressed:
            m_state.jumpHeld = true;
            TryStartJump(allowsJump, actions);
            break;

        case PlayerInputEventType::JumpReleased:
            m_state.jumpHeld = false;
            break;

        case PlayerInputEventType::DashPressed:
            TryStartDash(allowsDash, actions);
            break;
    }
}

void PlayerStateManager::UpdateFacing()
{
    const float direction = m_state.MoveDirection();
    if (direction != 0.f)
    {
        m_state.facingDirection = direction;
    }
}

void PlayerStateManager::TryStartJump(
    const bool allowsJump,
    PlayerStateActions& actions
)
{
    if (actions.AnyStarted() || !allowsJump || m_state.IsDashing())
    {
        return;
    }

    if (m_state.IsGrounded())
    {
        actions.jumpStarted = true;
        m_state.verticalState = VerticalState::Rising;
        m_state.canAirJump = true;
    }
    else if (m_state.canAirJump)
    {
        actions.jumpStarted = true;
        actions.airJumpStarted = true;
        m_state.verticalState = VerticalState::Rising;
        m_state.canAirJump = false;
    }
}

void PlayerStateManager::TryStartDash(
    const bool allowsDash,
    PlayerStateActions& actions
)
{
    if (actions.AnyStarted() ||
        !allowsDash ||
        m_state.IsDashing() ||
        m_state.dashCooldown > 0.f ||
        (!m_state.IsGrounded() && !m_state.canAirDash))
    {
        return;
    }

    if (!m_state.IsGrounded())
    {
        m_state.canAirDash = false;
    }

    m_state.movementMode = MovementMode::Dashing;
    m_state.dashDirection = m_state.facingDirection;
    m_state.dashTimer = DASH_DURATION;
    m_state.dashCooldown = DASH_COOLDOWN;

    actions.dashStarted = true;
    actions.dashDirection = m_state.dashDirection;
}

void PlayerStateManager::UpdateTimers(
    const float dt,
    const bool dashStartedThisFrame
)
{
    if (dashStartedThisFrame)
    {
        return;
    }

    m_state.dashCooldown = std::max(0.f, m_state.dashCooldown - dt);

    if (m_state.IsDashing())
    {
        m_state.dashTimer = std::max(0.f, m_state.dashTimer - dt);

        if (m_state.dashTimer <= 0.f)
        {
            StopDash();
        }
    }
}

void PlayerStateManager::ApplyPhysicsResult(
    const bool isGrounded,
    const float verticalVelocity
)
{
    if (isGrounded)
    {
        m_state.verticalState = VerticalState::Grounded;
        m_state.canAirJump = true;
        m_state.canAirDash = true;
    }
    else if (verticalVelocity < -VERTICAL_SPEED_THRESHOLD)
    {
        m_state.verticalState = VerticalState::Rising;
    }
    else
    {
        m_state.verticalState = VerticalState::Falling;
    }
}

void PlayerStateManager::StopDash()
{
    m_state.movementMode = MovementMode::Normal;
    m_state.dashTimer = 0.f;
}

const PlayerState& PlayerStateManager::GetState() const
{
    return m_state;
}
