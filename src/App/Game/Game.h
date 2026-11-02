#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include "App/CameraController/CameraController.h"
#include "Infrastructure/Input/KeyboardHandler.h"
#include "Systems/RenderDataBuilder/RenderDataBuilder.h"
#include "View/WorldRenderer/WorldRenderer.h"
#include "World/World.h"

class Game
{
private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	World m_world;
	KeyboardHandler m_keyboardHandler;
	RenderDataBuilder m_renderDataBuilder;
	CameraController m_cameraController;
	WorldRenderer m_worldRenderer;

	void ProcessEvents();
	void Render(const RenderSnapshot& snapshot);

public:
	Game();
	void Run();
};
