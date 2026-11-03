#include "Screens/GameplayScreen/GameplayScreen.h"

#include <filesystem>

#include "Infrastructure/Input/PlayerInputEvent.h"
#include "Infrastructure/LevelRepository/LevelRepository.h"

namespace
{
	constexpr const char* DEFAULT_LEVEL_FILE = "test_level.txt";

	constexpr int HUD_HEALTH = 100;
	constexpr int HUD_MAX_HEALTH = 100;
	constexpr int HUD_POTIONS = 3;
	constexpr int HUD_LEVEL = 1;

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

World GameplayScreen::CreateWorld()
{
	return World(LevelRepository().Load(GetDefaultLevelPath()));
}

GameplayScreen::GameplayScreen(const sf::Font& font, const sf::Vector2f viewSize)
	: 	m_viewSize(viewSize),
		m_world(CreateWorld()),
		m_cameraController(viewSize, m_world.GetBounds()),
		m_worldRenderer(m_world.GetBounds().size),
		m_hud(font)
{
	m_hud.SetData({HUD_HEALTH, HUD_MAX_HEALTH, HUD_POTIONS, HUD_LEVEL});
	RefreshView();
}

ScreenCommand GameplayScreen::HandleEvent(const sf::Event& event, const sf::RenderWindow&)
{
	const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
	if (keyPressed && keyPressed->code == sf::Keyboard::Key::Escape)
	{
		return ScreenCommand::Pause;
	}

	m_keyboardHandler.HandleEvent(event);
	return ScreenCommand::None;
}

ScreenCommand GameplayScreen::Update(const float deltaTime)
{
	for (const PlayerInputEvent& inputEvent : m_keyboardHandler.TakeEvents())
	{
		m_world.PushPlayerInput(inputEvent);
	}

	const WorldResult result = m_world.Update(deltaTime);
	RefreshView();

	switch (result)
	{
		case WorldResult::None:
			return ScreenCommand::None;

		case WorldResult::Victory:
			return ScreenCommand::Victory;

		case WorldResult::Defeat:
			return ScreenCommand::Defeat;
	}

	return ScreenCommand::None;
}

void GameplayScreen::Render(sf::RenderWindow& window)
{
	m_worldRenderer.Draw(m_snapshot, window, m_cameraController.GetView());
	m_hud.Render(window);
}

void GameplayScreen::Restart()
{
	m_world = CreateWorld();
	m_cameraController = CameraController(m_viewSize, m_world.GetBounds());
	m_keyboardHandler.Reset();
	RefreshView();
}

void GameplayScreen::ReleaseInput()
{
	m_keyboardHandler.Reset();

	m_world.PushPlayerInput({PlayerInputEventType::MoveLeftReleased});
	m_world.PushPlayerInput({PlayerInputEventType::MoveRightReleased});
	m_world.PushPlayerInput({PlayerInputEventType::JumpReleased});
}

void GameplayScreen::RefreshView()
{
	m_snapshot = m_renderDataBuilder.Build(m_world);
	m_cameraController.Update(m_snapshot.cameraTargetPosition);
}
