#pragma once

#include <SFML/System/Vector2.hpp>

#include "App/CameraController/CameraController.h"
#include "Audio/AudioManager.h"
#include "Infrastructure/Input/KeyboardHandler.h"
#include "Resources/ProjectPaths.h"
#include "Resources/ResourceManager.h"
#include "Screens/Screen.h"
#include "UI/HUD/HUD.h"
#include "View/WorldRenderer/WorldRenderer.h"
#include "World/World.h"

class GameplayScreen final : public Screen
{
private:
	AudioManager& m_audio;
	World m_world;
	KeyboardHandler m_keyboardHandler;
	CameraController m_camera;
	WorldRenderer m_worldRenderer;
	HUD m_hud;

	static World CreateWorld(const ProjectPaths& paths);

	ScreenCommand ProcessWorldEvents();
	void ReleaseInput();

public:
	GameplayScreen(
		ResourceManager& resources,
		AudioManager& audioManager,
		const ProjectPaths& paths,
		sf::Vector2f viewSize
	);

	ScreenCommand HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	ScreenCommand Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) const override;
	void OnEnter() override;
	void OnExit() override;
};
