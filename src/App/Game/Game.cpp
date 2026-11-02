#include "App/Game/Game.h"

#include <algorithm>
#include <filesystem>
#include <optional>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "Infrastructure/LevelRepository/LevelRepository.h"

namespace
{
	constexpr sf::Vector2u WINDOW_SIZE = {1500, 1200};
	constexpr const char* WINDOW_TITLE = "LIZARD MUST DIE";
	constexpr float MAX_DELTA_TIME = 1.f / 20.f;
	constexpr const char* DEFAULT_LEVEL_FILE = "test_level.txt";

	std::filesystem::path GetDefaultLevelPath()
	{
#ifdef PLATFORMER_LEVELS_DIR
		const std::filesystem::path levelsDir = PLATFORMER_LEVELS_DIR;
#else
		const std::filesystem::path levelsDir =
			std::filesystem::path(__FILE__).parent_path().parent_path().parent_path() / "levels";
#endif
		return levelsDir / DEFAULT_LEVEL_FILE;
	}
}

Game::Game()
	: 	m_window(sf::VideoMode(WINDOW_SIZE), WINDOW_TITLE),
		m_world(LevelRepository().Load(GetDefaultLevelPath())),
		m_cameraController(
			sf::Vector2f(WINDOW_SIZE),
			m_world.GetBounds()
		),
		m_worldRenderer(m_world.GetBounds().size)
{
	m_window.setVerticalSyncEnabled(true);
	m_window.setKeyRepeatEnabled(false);
	m_window.setView(m_cameraController.GetView());
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

		m_world.Update(dt);

		const RenderSnapshot snapshot = m_renderDataBuilder.Build(m_world);
		m_cameraController.Update(snapshot.cameraTargetPosition);
		Render(snapshot);
	}
}

void Game::ProcessEvents()
{
	while (const std::optional event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
		}

		m_keyboardHandler.HandleEvent(*event);
	}

	for (const PlayerInputEvent& inputEvent : m_keyboardHandler.TakeEvents())
	{
		m_world.PushPlayerInput(inputEvent);
	}
}

void Game::Render(const RenderSnapshot& snapshot)
{
	m_worldRenderer.Draw(snapshot, m_window, m_cameraController.GetView());
	m_window.display();
}