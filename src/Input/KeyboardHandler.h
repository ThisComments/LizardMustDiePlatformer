#pragma once

#include <array>
#include <cstddef>
#include <vector>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Input/PlayerInputEvent.h"

class KeyboardHandler
{
private:
    enum class Action: int
    {
        MoveLeft,
        MoveRight,
        Jump,
        Dash
    };

    struct Binding
    {
        sf::Keyboard::Key key;
        Action action;
    };

    static constexpr std::size_t BINDING_COUNT = 8;
    static const std::array<Binding, BINDING_COUNT> BINDINGS;

    std::array<bool, BINDING_COUNT> m_keyDown;
    std::vector<PlayerInputEvent> m_events;

    bool IsActionHeld(Action action) const;
    void OnKeyPressed(sf::Keyboard::Key key);
    void OnKeyReleased(sf::Keyboard::Key key);
    void ReleaseAll();
    void EmitPressed(Action action);
    void EmitReleased(Action action);

public:
    KeyboardHandler();
    void HandleEvent(const sf::Event& event);
    std::vector<PlayerInputEvent> TakeEvents();
};
