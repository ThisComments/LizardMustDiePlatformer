#include "KeyboardHandler.h"

KeyboardHandler::KeyboardHandler()
    :   m_input{},
        m_previousJump(false),
        m_previousDash(false)
{
}

void KeyboardHandler::Update()
{
    m_input.moveLeft = 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
    m_input.moveRight = 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
    bool dashHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);
    bool jumpHeld = 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);

    m_input.jump = jumpHeld && !m_previousJump;
    m_input.jumpHeld = jumpHeld;

    m_input.dash = dashHeld && !m_previousDash;

    m_previousJump = jumpHeld;
    m_previousDash = dashHeld;
}

PlayerInput KeyboardHandler::GetInput() const
{
    return m_input;
}