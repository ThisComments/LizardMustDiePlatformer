#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>

#include "App/CameraController/CameraController.h"
#include "Infrastructure/Input/KeyboardHandler.h"
#include "Screens/Screen.h"
#include "Systems/RenderDataBuilder/RenderDataBuilder.h"
#include "UI/HUD/HUD.h"
#include "View/WorldRenderer/WorldRenderer.h"
#include "World/World.h"

class GameplayScreen : public Screen
{
private:
	sf::Vector2f m_viewSize;
	World m_world;
	KeyboardHandler m_keyboardHandler;
	RenderDataBuilder m_renderDataBuilder;
	CameraController m_cameraController;
	WorldRenderer m_worldRenderer;
	RenderSnapshot m_snapshot;
	HUD m_hud;

	static World CreateWorld();
	void RefreshView();

public:
	GameplayScreen(const sf::Font& font, sf::Vector2f viewSize);

	ScreenCommand HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	ScreenCommand Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;

	void Restart();
	void ReleaseInput();
};
