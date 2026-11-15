#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include "Audio/AudioManager.h"
#include "Resources/ProjectPaths.h"
#include "Resources/ResourceManager.h"
#include "Screens/ScreenManager/ScreenManager.h"

class Game
{
private:
	ProjectPaths m_paths;
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	ResourceManager m_resourceManager;
	AudioManager m_audioManager;
	ScreenManager m_screenManager;

	void ProcessEvents();
	void Update(float deltaTime);
	void Render();

public:
	Game(ProjectPaths paths);

	void Run();
};
