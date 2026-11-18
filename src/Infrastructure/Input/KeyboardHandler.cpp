#include "Infrastructure/Input/KeyboardHandler.h"

#include <utility>

const std::array<KeyboardHandler::Binding, KeyboardHandler::BINDING_COUNT>
	KeyboardHandler::BINDINGS = {{
		{ sf::Keyboard::Key::A, ACTION_MOVE_LEFT },
		{ sf::Keyboard::Key::Left, ACTION_MOVE_LEFT },
		{ sf::Keyboard::Key::D, ACTION_MOVE_RIGHT },
		{ sf::Keyboard::Key::Right, ACTION_MOVE_RIGHT },
		{ sf::Keyboard::Key::Space, ACTION_JUMP },
		{ sf::Keyboard::Key::W, ACTION_JUMP },
		{ sf::Keyboard::Key::Up, ACTION_JUMP },
		{ sf::Keyboard::Key::LShift, ACTION_DASH },
		{ sf::Keyboard::Key::Q, ACTION_HEAL }
	}};

KeyboardHandler::KeyboardHandler()
	: 	m_keyDown{}
{
}

void KeyboardHandler::HandleEvent(const sf::Event& event)
{
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		OnKeyPressed(keyPressed->code);
	}
	else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
	{
		OnKeyReleased(keyReleased->code);
	}
	else if (event.is<sf::Event::FocusLost>())
	{
		ReleaseAll();
	}
}

std::vector<PlayerInputEvent> KeyboardHandler::TakeEvents()
{
	return std::exchange(m_events, {});
}

void KeyboardHandler::Reset()
{
	m_keyDown.fill(false);
	m_events.clear();
}

bool KeyboardHandler::IsActionHeld(const Action action) const
{
	for (std::size_t i = 0; i < BINDING_COUNT; ++i)
	{
		if (BINDINGS[i].action == action && m_keyDown[i])
		{
			return true;
		}
	}
	return false;
}

void KeyboardHandler::OnKeyPressed(const sf::Keyboard::Key key)
{
	for (std::size_t i = 0; i < BINDING_COUNT; ++i)
	{
		if (BINDINGS[i].key != key)
		{
			continue;
		}

		if (m_keyDown[i])
		{
			return;
		}

		const Action action = BINDINGS[i].action;
		const bool wasHeld = IsActionHeld(action);
		m_keyDown[i] = true;
		if (!wasHeld)
		{
			EmitPressed(action);
		}
		return;
	}
}

void KeyboardHandler::OnKeyReleased(const sf::Keyboard::Key key)
{
	for (std::size_t i = 0; i < BINDING_COUNT; ++i)
	{
		if (BINDINGS[i].key != key)
		{
			continue;
		}

		if (!m_keyDown[i])
		{
			return;
		}

		m_keyDown[i] = false;
		const Action action = BINDINGS[i].action;
		if (!IsActionHeld(action))
		{
			EmitReleased(action);
		}
		return;
	}
}

void KeyboardHandler::ReleaseAll()
{
	const Action actions[] = {
		ACTION_MOVE_LEFT,
		ACTION_MOVE_RIGHT,
		ACTION_JUMP,
		ACTION_DASH,
		ACTION_HEAL
	};

	for (const Action action : actions)
	{
		if (IsActionHeld(action))
		{
			EmitReleased(action);
		}
	}

	m_keyDown.fill(false);
}

void KeyboardHandler::EmitPressed(const Action action)
{
	switch (action)
	{
		case ACTION_MOVE_LEFT:
			m_events.push_back({ INPUT_MOVE_LEFT_PRESSED });
			break;
		case ACTION_MOVE_RIGHT:
			m_events.push_back({ INPUT_MOVE_RIGHT_PRESSED });
			break;
		case ACTION_JUMP:
			m_events.push_back({ INPUT_JUMP_PRESSED });
			break;
		case ACTION_DASH:
			m_events.push_back({ INPUT_DASH_PRESSED });
			break;
		case ACTION_HEAL:
			m_events.push_back({ INPUT_HEAL_PRESSED });
			break;
	}
}

void KeyboardHandler::EmitReleased(const Action action)
{
	switch (action)
	{
		case ACTION_MOVE_LEFT:
			m_events.push_back({ INPUT_MOVE_LEFT_RELEASED });
			break;
		case ACTION_MOVE_RIGHT:
			m_events.push_back({ INPUT_MOVE_RIGHT_RELEASED });
			break;
		case ACTION_JUMP:
			m_events.push_back({ INPUT_JUMP_RELEASED });
			break;
		case ACTION_DASH:
		case ACTION_HEAL:
			break;
	}
}