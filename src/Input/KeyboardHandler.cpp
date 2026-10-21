#include "KeyboardHandler.h"

#include <utility>

const std::array<KeyboardHandler::Binding, KeyboardHandler::BINDING_COUNT>
    KeyboardHandler::BINDINGS = {{
        {sf::Keyboard::Key::A, Action::MoveLeft},
        {sf::Keyboard::Key::Left, Action::MoveLeft},
        {sf::Keyboard::Key::D, Action::MoveRight},
        {sf::Keyboard::Key::Right, Action::MoveRight},
        {sf::Keyboard::Key::Space, Action::Jump},
        {sf::Keyboard::Key::W, Action::Jump},
        {sf::Keyboard::Key::Up, Action::Jump},
        {sf::Keyboard::Key::LShift, Action::Dash}
    }};

KeyboardHandler::KeyboardHandler()
    :   m_keyDown{}
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
        Action::MoveLeft, Action::MoveRight, Action::Jump, Action::Dash
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
        case Action::MoveLeft:
            m_events.push_back({PlayerInputEventType::MoveLeftPressed});
            break;
        case Action::MoveRight:
            m_events.push_back({PlayerInputEventType::MoveRightPressed});
            break;
        case Action::Jump:
            m_events.push_back({PlayerInputEventType::JumpPressed});
            break;
        case Action::Dash:
            m_events.push_back({PlayerInputEventType::DashPressed});
            break;
    }
}

void KeyboardHandler::EmitReleased(const Action action)
{
    switch (action)
    {
        case Action::MoveLeft:
            m_events.push_back({PlayerInputEventType::MoveLeftReleased});
            break;
        case Action::MoveRight:
            m_events.push_back({PlayerInputEventType::MoveRightReleased});
            break;
        case Action::Jump:
            m_events.push_back({PlayerInputEventType::JumpReleased});
            break;
        case Action::Dash:
            break;
    }
}
