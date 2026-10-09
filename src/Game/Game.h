#pragma once
#include <SFML/Graphics.hpp>
#include "../World/World.h"
#include "../KeyboardHandler/KeyboardHandler.h"

class Game
{
private:
    sf::RenderWindow m_window;
    sf::View m_camera;
    sf::Clock m_clock;
    World m_world;
    KeyboardHandler m_keyboardHandler;
    void UpdateCamera();

public:
    Game();
    void Run();
};