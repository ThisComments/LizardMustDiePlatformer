#include "Screens/ScreenManager/ScreenManager.h"

ScreenManager::ScreenManager(const sf::Font& font, const sf::Vector2f screenSize)
	: 	m_mainMenu(font, screenSize),
		m_gameplay(font, screenSize),
		m_pause(font, screenSize),
		m_gameOver(font, screenSize),
		//m_howToPlay(font, screenSize),
		m_active(&m_mainMenu)
{
}

void ScreenManager::Show(const GameState state)
{
	switch (state)
	{
		case GameState::Menu:
			m_active = &m_mainMenu;
			break;

		case GameState::Playing:
			m_active = &m_gameplay;
			break;

		case GameState::Paused:
			m_active = &m_pause;
			break;

		case GameState::GameOver:
			m_active = &m_gameOver;
			break;

		/*case GameState::HowToPlay:
			m_active = &m_howToPlay;
			break;*/
	}

	m_active->OnEnter();
}

void ScreenManager::SetGameOverReason(const GameOverReason reason)
{
	m_gameOver.SetReason(reason);
}

void ScreenManager::RestartGameplay()
{
	m_gameplay.Restart();
}

void ScreenManager::ReleaseGameplayInput()
{
	m_gameplay.ReleaseInput();
}

ScreenCommand ScreenManager::HandleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	return m_active->HandleEvent(event, window);
}

ScreenCommand ScreenManager::Update(const float deltaTime)
{
	return m_active->Update(deltaTime);
}

void ScreenManager::Render(sf::RenderWindow& window)
{
	if (m_active->IsOverlay())
	{
		m_gameplay.Render(window);
	}

	m_active->Render(window);
}
