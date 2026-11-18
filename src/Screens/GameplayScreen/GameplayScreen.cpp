#include "Screens/GameplayScreen/GameplayScreen.h"

#include "Infrastructure/Input/PlayerInputEvent.h"
#include "Infrastructure/LevelRepository/LevelRepository.h"
#include "Resources/AssetPaths.h"

namespace
{
	constexpr const char* DEFAULT_LEVEL_FILE = "test_level.txt";

	constexpr const char* JUMP_SOUND = "jump";
	constexpr const char* COLLECT_SOUND = "collect";
	constexpr const char* DAMAGE_SOUND = "damage";
	constexpr const char* HEAL_SOUND = "heal";
	constexpr const char* VICTORY_SOUND = "victory";
	constexpr const char* DEFEAT_SOUND = "defeat";
}

World GameplayScreen::CreateWorld(const ProjectPaths& paths)
{
	return World(LevelRepository().Load(paths.Levels() / DEFAULT_LEVEL_FILE));
}

GameplayScreen::GameplayScreen(
	ResourceManager& resources,
	AudioManager& audioManager,
	const ProjectPaths& paths,
	const sf::Vector2f viewSize
)
	: 	m_audio(audioManager),
		m_world(CreateWorld(paths)),
		m_camera(viewSize, m_world.GetBounds()),
		m_worldRenderer(m_world, m_camera, resources),
		m_hud(resources.GetFont(AssetPaths::UiFont))
{
	m_hud.Update(m_world.GetStats());
	m_camera.Update(m_world.GetPlayer().GetPosition());
}

ScreenCommand GameplayScreen::HandleEvent(const sf::Event& event, const sf::RenderWindow&)
{
	const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
	if (keyPressed && keyPressed->code == sf::Keyboard::Key::Escape)
	{
		ReleaseInput();
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

	m_world.Update(deltaTime);
	m_camera.Update(m_world.GetPlayer().GetPosition());
	m_worldRenderer.Update(deltaTime);

	const ScreenCommand command = ProcessWorldEvents();
	m_hud.Update(m_world.GetStats());

	if (command != ScreenCommand::None)
	{
		ReleaseInput();
	}
	return command;
}

void GameplayScreen::Render(sf::RenderWindow& window) const
{
	m_worldRenderer.Render(window);

	window.setView(window.getDefaultView());
	m_hud.Render(window);
}

void GameplayScreen::OnEnter()
{
	m_audio.PlayMusic();
}

void GameplayScreen::OnExit()
{
	m_audio.StopMusic();
}

ScreenCommand GameplayScreen::ProcessWorldEvents()
{
	ScreenCommand command = ScreenCommand::None;

	for (const WorldEvent& event : m_world.DrainEvents())
	{
		switch (event.type)
		{
			case EVENT_PLAYER_JUMPED:
				m_audio.PlaySfx(JUMP_SOUND);
				break;

			case EVENT_COLLECTIBLE_COLLECTED:
				m_audio.PlaySfx(COLLECT_SOUND);
				break;

			case EVENT_PLAYER_DAMAGED:
				m_audio.PlaySfx(DAMAGE_SOUND);
				break;

			case EVENT_PLAYER_HEALED:
				m_audio.PlaySfx(HEAL_SOUND);
				break;

			case EVENT_VICTORY_REACHED:
				m_audio.PlaySfx(VICTORY_SOUND);
				command = ScreenCommand::Victory;
				break;

			case EVENT_PLAYER_DEFEATED:
				m_audio.PlaySfx(DEFEAT_SOUND);
				command = ScreenCommand::Defeat;
				break;
		}
	}

	return command;
}

void GameplayScreen::ReleaseInput()
{
	m_keyboardHandler.Reset();

	m_world.PushPlayerInput({ INPUT_MOVE_LEFT_RELEASED });
	m_world.PushPlayerInput({ INPUT_MOVE_RIGHT_RELEASED });
	m_world.PushPlayerInput({ INPUT_JUMP_RELEASED });
}