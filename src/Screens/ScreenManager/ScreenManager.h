#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "App/Game/GameState.h"
#include "Screens/GameOverScreen/GameOverScreen.h"
#include "Screens/GameplayScreen/GameplayScreen.h"
#include "Screens/MainMenuScreen/MainMenuScreen.h"
#include "Screens/PauseScreen/PauseScreen.h"
#include "Screens/HowToPlayScreen/HowToPlayScreen.h"
#include "Screens/Screen.h"

class ScreenManager
{
private:
	MainMenuScreen m_mainMenu;
	GameplayScreen m_gameplay;
	PauseScreen m_pause;
	GameOverScreen m_gameOver;
	HowToPlayScreen m_howToPlay;
	Screen *m_active;

public:
	ScreenManager(const sf::Font& font, sf::Vector2f screenSize);

	ScreenManager(const ScreenManager&) = delete;
	ScreenManager& operator=(const ScreenManager&) = delete;

	void Show(GameState state);
	void SetGameOverReason(GameOverReason reason);
	void RestartGameplay();
	void ReleaseGameplayInput();

	ScreenCommand HandleEvent(const sf::Event& event, const sf::RenderWindow& window);
	ScreenCommand Update(float deltaTime);
	void Render(sf::RenderWindow& window);
};
