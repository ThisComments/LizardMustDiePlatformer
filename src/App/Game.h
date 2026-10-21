#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include "App/CameraController.h"
#include "Input/KeyboardHandler.h"
#include "View/WorldRenderer/WorldRenderer.h"
#include "World/World.h"

class Game
{
private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	KeyboardHandler m_keyboardHandler;
	WorldRenderer m_worldRenderer;
	World m_world;
	CameraController m_cameraController;

	void ProcessEvents();
	void Render();

public:
	Game();
	void Run();
};
