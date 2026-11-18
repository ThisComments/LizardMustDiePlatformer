#include "Model/Player/PlayerStateManager/PlayerStateManager.h"

#include <algorithm>

namespace
{
	constexpr float DASH_DURATION = 0.15f;
	constexpr float DASH_COOLDOWN = 2.f;
	constexpr float HEAL_COOLDOWN = 8.f;
	constexpr float VERTICAL_SPEED_THRESHOLD = 0.001f;
}

void PlayerStateManager::PushInput(const PlayerInputEvent& event)
{
	m_inputQueue.push_back(event);
}

PlayerStateActions PlayerStateManager::ProcessInputQueue(
	const bool allowsJump,
	const bool allowsDash,
	const bool canHeal
)
{
	PlayerStateActions actions;

	while (!m_inputQueue.empty())
	{
		const PlayerInputEvent event = m_inputQueue.front();
		m_inputQueue.pop_front();

		HandleEvent(event, allowsJump, allowsDash, canHeal, actions);
	}

	return actions;
}

void PlayerStateManager::HandleEvent(
	const PlayerInputEvent& event,
	const bool allowsJump,
	const bool allowsDash,
	const bool canHeal,
	PlayerStateActions& actions
)
{
	switch (event.type)
	{
		case INPUT_MOVE_LEFT_PRESSED:
			m_state.moveLeftHeld = true;
			UpdateFacing();
			break;

		case INPUT_MOVE_LEFT_RELEASED:
			m_state.moveLeftHeld = false;
			UpdateFacing();
			break;

		case INPUT_MOVE_RIGHT_PRESSED:
			m_state.moveRightHeld = true;
			UpdateFacing();
			break;

		case INPUT_MOVE_RIGHT_RELEASED:
			m_state.moveRightHeld = false;
			UpdateFacing();
			break;

		case INPUT_JUMP_PRESSED:
			m_state.jumpHeld = true;
			TryStartJump(allowsJump, actions);
			break;

		case INPUT_JUMP_RELEASED:
			m_state.jumpHeld = false;
			break;

		case INPUT_DASH_PRESSED:
			TryStartDash(allowsDash, actions);
			break;

		case INPUT_HEAL_PRESSED:
			TryStartHeal(canHeal, actions);
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
		m_state.verticalState = VERTICAL_RISING;
		m_state.canAirJump = true;
	}
	else if (m_state.canAirJump)
	{
		actions.jumpStarted = true;
		actions.airJumpStarted = true;
		m_state.verticalState = VERTICAL_RISING;
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

	m_state.movementMode = MOVEMENT_DASHING;
	m_state.dashDirection = m_state.facingDirection;
	m_state.dashTimer = DASH_DURATION;
	m_state.dashCooldown = DASH_COOLDOWN;

	actions.dashStarted = true;
	actions.dashDirection = m_state.dashDirection;
}

void PlayerStateManager::TryStartHeal(
	const bool canHeal,
	PlayerStateActions& actions
)
{
	if (!canHeal || actions.healStarted || m_state.healCooldown > 0.f)
	{
		return;
	}

	m_state.healCooldown = HEAL_COOLDOWN;
	actions.healStarted = true;
}

void PlayerStateManager::UpdateTimers(
	const float dt,
	const bool dashStartedThisFrame
)
{
	m_state.healCooldown = std::max(0.f, m_state.healCooldown - dt);

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
		m_state.verticalState = VERTICAL_GROUNDED;
		m_state.canAirJump = true;
		m_state.canAirDash = true;
	}
	else if (verticalVelocity < -VERTICAL_SPEED_THRESHOLD)
	{
		m_state.verticalState = VERTICAL_RISING;
	}
	else
	{
		m_state.verticalState = VERTICAL_FALLING;
	}
}

void PlayerStateManager::StopDash()
{
	m_state.movementMode = MOVEMENT_NORMAL;
	m_state.dashTimer = 0.f;
}

const PlayerState& PlayerStateManager::GetState() const
{
	return m_state;
}
