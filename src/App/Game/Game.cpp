#include "App/Game/Game.h"

#include <algorithm>
#include <optional>
#include <utility>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

namespace
{
	constexpr sf::Vector2u WINDOW_SIZE = {1500, 1200};
	constexpr const char* WINDOW_TITLE = "LIZARD MUST DIE";
	constexpr float MAX_DELTA_TIME = 1.f / 20.f;
}

Game::Game(ProjectPaths paths)
	: 	m_paths(std::move(paths)),
		m_window(sf::VideoMode(WINDOW_SIZE), WINDOW_TITLE),
		m_resourceManager(m_paths),
		m_audioManager(m_paths),
		m_screenManager(m_resourceManager, m_audioManager, m_paths, sf::Vector2f(WINDOW_SIZE))
{
	m_window.setVerticalSyncEnabled(true);
	m_window.setKeyRepeatEnabled(false);
}

void Game::Run()
{
	while (m_window.isOpen())
	{
		const float dt = std::min(m_clock.restart().asSeconds(), MAX_DELTA_TIME);

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
			return;
		}

		m_screenManager.HandleEvent(*event, m_window);
		if (m_screenManager.IsExitRequested())
		{
			m_window.close();
			return;
		}
	}
}

void Game::Update(const float dt)
{
	if (!m_window.isOpen())
	{
		return;
	}

	m_screenManager.Update(dt);
	if (m_screenManager.IsExitRequested())
	{
		m_window.close();
		return;
	}

	m_audioManager.Update();
}

void Game::Render()
{
	if (!m_window.isOpen())
	{
		return;
	}

	m_screenManager.Render(m_window);
	m_window.display();
}
