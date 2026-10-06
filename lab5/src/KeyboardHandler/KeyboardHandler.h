#pragma once

#include <SFML/Window/Keyboard.hpp>
#include "PlayerInput.h"

class KeyboardHandler
{
private:
    PlayerInput m_input;
    bool m_previousJump;
    bool m_previousDash;

public:
    KeyboardHandler();
    void Update();
    PlayerInput GetInput() const;
    
};