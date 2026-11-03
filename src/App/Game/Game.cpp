#include "App/Game/Game.h"

#include <algorithm>
#include <optional>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "View/RenderConstants.h"

namespace
{
	constexpr sf::Vector2u WINDOW_SIZE = {1500, 1200};
	constexpr const char* WINDOW_TITLE = "LIZARD MUST DIE";
	constexpr float MAX_DELTA_TIME = 1.f / 20.f;
}

Game::Game()
	: 	m_window(sf::VideoMode(WINDOW_SIZE), WINDOW_TITLE),
		m_font(RenderConstants::UiFontPath),
		m_screens(m_font, sf::Vector2f(WINDOW_SIZE)),
		m_state(GameState::Menu),
		m_gameOverReason(GameOverReason::Defeat)
{
	m_window.setVerticalSyncEnabled(true);
	m_window.setKeyRepeatEnabled(false);
	m_screens.Show(m_state);
}

void Game::Run()
{
	while (m_window.isOpen())
	{
		const float dt = std::min(
			m_clock.restart().asSeconds(),
			MAX_DELTA_TIME
		);

		ProcessEvents();
		Update(dt);
		Render();
	}
}

void Game::ProcessEvents()
{
	while (const std::optional event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
			continue;
		}

		ApplyCommand(m_screens.HandleEvent(*event, m_window));
	}
}

void Game::Update(const float deltaTime)
{
	switch (m_state)
	{
		case GameState::Menu:
			break;

		case GameState::Playing:
			ApplyCommand(m_screens.Update(deltaTime));
			break;

		case GameState::Paused:
			break;

		case GameState::GameOver:
			break;

		case GameState::HowToPlay:
			break;
	}
}

void Game::Render()
{
	m_screens.Render(m_window);
	m_window.display();
}

void Game::ApplyCommand(const ScreenCommand command)
{
	switch (command)
	{
		case ScreenCommand::None:
			break;

		case ScreenCommand::StartGame:
			if (m_state == GameState::Menu)
			{
				StartSession();
			}
			break;

		case ScreenCommand::Pause:
			if (m_state == GameState::Playing)
			{
				PauseSession();
			}
			break;

		case ScreenCommand::Resume:
			if (m_state == GameState::Paused)
			{
				SetState(GameState::Playing);
			}
			break;

		case ScreenCommand::Restart:
			if (m_state == GameState::GameOver)
			{
				StartSession();
			}
			break;

		case ScreenCommand::ReturnToMenu:
			if (m_state == GameState::Paused 
				|| m_state == GameState::GameOver 
				|| m_state == GameState::HowToPlay)
			{
				SetState(GameState::Menu);
			}
			break;

		case ScreenCommand::Exit:
			if (m_state == GameState::Menu)
			{
				m_window.close();
			}
			break;

		case ScreenCommand::Victory:
			if (m_state == GameState::Playing)
			{
				FinishSession(GameOverReason::Victory);
			}
			break;

		case ScreenCommand::Defeat:
			if (m_state == GameState::Playing)
			{
				FinishSession(GameOverReason::Defeat);
			}
			break;

		case ScreenCommand::HowToPlay:
			if (m_state == GameState::Menu)
			{
				SetState(GameState::HowToPlay);
			}
			break;
	}
}

void Game::SetState(const GameState state)
{
	m_state = state;
	m_screens.Show(m_state);
}

void Game::StartSession()
{
	m_screens.RestartGameplay();
	m_gameOverReason = GameOverReason::Defeat;
	SetState(GameState::Playing);
}

void Game::PauseSession()
{
	m_screens.ReleaseGameplayInput();
	SetState(GameState::Paused);
}

void Game::FinishSession(const GameOverReason reason)
{
	m_screens.ReleaseGameplayInput();
	m_gameOverReason = reason;
	m_screens.SetGameOverReason(m_gameOverReason);
	SetState(GameState::GameOver);
}
