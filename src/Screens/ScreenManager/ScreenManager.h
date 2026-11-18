#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "Audio/AudioManager.h"
#include "Resources/ProjectPaths.h"
#include "Resources/ResourceManager.h"
#include "Screens/Screen.h"
#include "Screens/ScreenCommand.h"

class ScreenManager
{
private:
	ResourceManager& m_resources;
	AudioManager& m_audio;
	const ProjectPaths& m_paths;
	const sf::Font& m_font;
	sf::Vector2f m_screenSize;
	std::vector<std::unique_ptr<Screen>> m_stack;
	bool m_exitRequested;

	std::unique_ptr<Screen> CreateMainMenu() const;
	std::unique_ptr<Screen> CreateGameplay() const;
	std::unique_ptr<Screen> CreateHowToPlay() const;
	std::unique_ptr<Screen> CreatePause() const;
	std::unique_ptr<Screen> CreateGameOver(bool isVictory) const;

	void Apply(ScreenCommand command);
	void Push(std::unique_ptr<Screen> screen);
	void Pop();
	void ReplaceAll(std::unique_ptr<Screen> screen);

public:
	ScreenManager(
		ResourceManager& resources,
		AudioManager& audio,
		const ProjectPaths& paths,
		sf::Vector2f screenSize
	);

	~ScreenManager();

	ScreenManager(const ScreenManager&) = delete;
	ScreenManager& operator=(const ScreenManager&) = delete;

	void HandleEvent(const sf::Event& event, const sf::RenderWindow& window);
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window) const;

	bool IsExitRequested() const;
};
