#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include "App/Game/GameState.h"
#include "Screens/ScreenCommand.h"
#include "Screens/ScreenManager/ScreenManager.h"

class Game
{
private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	sf::Font m_font;
	ScreenManager m_screens;
	GameState m_state;
	GameOverReason m_gameOverReason;

	void ProcessEvents();
	void Update(float deltaTime);
	void Render();

	void ApplyCommand(ScreenCommand command);
	void SetState(GameState state);
	void StartSession();
	void PauseSession();
	void FinishSession(GameOverReason reason);

public:
	Game();
	void Run();
};
