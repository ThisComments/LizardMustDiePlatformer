#include "Screens/ScreenManager/ScreenManager.h"

#include <utility>

#include "Resources/AssetPaths.h"
#include "Screens/GameOverScreen/GameOverScreen.h"
#include "Screens/GameplayScreen/GameplayScreen.h"
#include "Screens/HowToPlayScreen/HowToPlayScreen.h"
#include "Screens/MainMenuScreen/MainMenuScreen.h"
#include "Screens/PauseScreen/PauseScreen.h"

ScreenManager::ScreenManager(
	ResourceManager& resources,
	AudioManager& audio,
	const ProjectPaths& paths,
	const sf::Vector2f screenSize
)
	: 	m_resources(resources),
		m_audio(audio),
		m_paths(paths),
		m_font(resources.GetFont(AssetPaths::UiFont)),
		m_screenSize(screenSize),
		m_exitRequested(false)
{
	Push(CreateMainMenu());
}

ScreenManager::~ScreenManager()
{
	while (!m_stack.empty())
	{
		Pop();
	}
}

void ScreenManager::HandleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	Apply(m_stack.back()->HandleEvent(event, window));
}

void ScreenManager::Update(const float deltaTime)
{
	Apply(m_stack.back()->Update(deltaTime));
}

void ScreenManager::Render(sf::RenderWindow& window) const
{
	std::size_t first = m_stack.size() - 1;
	while (first > 0 && m_stack[first]->IsOverlay())
	{
		--first;
	}

	for (std::size_t i = first; i < m_stack.size(); ++i)
	{
		m_stack[i]->Render(window);
	}
}

bool ScreenManager::IsExitRequested() const
{
	return m_exitRequested;
}

std::unique_ptr<Screen> ScreenManager::CreateMainMenu() const
{
	return std::make_unique<MainMenuScreen>(m_font, m_screenSize);
}

std::unique_ptr<Screen> ScreenManager::CreateGameplay() const
{
	return std::make_unique<GameplayScreen>(m_resources, m_audio, m_paths, m_screenSize);
}

std::unique_ptr<Screen> ScreenManager::CreatePause() const
{
	return std::make_unique<PauseScreen>(m_font, m_screenSize, m_audio);
}

std::unique_ptr<Screen> ScreenManager::CreateHowToPlay() const
{
	return std::make_unique<HowToPlayScreen>(m_font, m_screenSize);
}

std::unique_ptr<Screen> ScreenManager::CreateGameOver(const bool isVictory) const
{
	return std::make_unique<GameOverScreen>(
		m_font,
		m_screenSize,
		isVictory ? GameOverReason::Victory : GameOverReason::Defeat,
		m_audio
	);
}

void ScreenManager::Apply(const ScreenCommand command)
{
	switch (command)
	{
		case ScreenCommand::None:
			break;

		case ScreenCommand::StartGame:
		case ScreenCommand::Restart:
			ReplaceAll(CreateGameplay());
			break;

		case ScreenCommand::HowToPlay:
			ReplaceAll(CreateHowToPlay());
			break;

		case ScreenCommand::Pause:
			Push(CreatePause());
			break;

		case ScreenCommand::Resume:
			Pop();
			break;

		case ScreenCommand::ReturnToMenu:
			ReplaceAll(CreateMainMenu());
			break;

		case ScreenCommand::Victory:
			Push(CreateGameOver(true));
			break;

		case ScreenCommand::Defeat:
			Push(CreateGameOver(false));
			break;

		case ScreenCommand::Exit:
			m_exitRequested = true;
			break;
	}
}

void ScreenManager::Push(std::unique_ptr<Screen> screen)
{
	m_stack.push_back(std::move(screen));
	m_stack.back()->OnEnter();
}

void ScreenManager::Pop()
{
	m_stack.back()->OnExit();
	m_stack.pop_back();
}

void ScreenManager::ReplaceAll(std::unique_ptr<Screen> screen)
{
	while (!m_stack.empty())
	{
		Pop();
	}
	Push(std::move(screen));
}
