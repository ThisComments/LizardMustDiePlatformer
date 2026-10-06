#include <algorithm>
#include "Game.h"

const float CAMERA_TRACKING_WIDTH = 600.f;
const float CAMERA_TRACKING_HEIGHT = 400.f;

Game::Game()
    :   m_window(sf::VideoMode({1500, 1200}), "Chizhov"),
        m_camera(sf::FloatRect({0.f, 0.f}, {1500.f, 1200.f}))
{
    sf::FloatRect worldBounds = m_world.GetBounds();

	m_camera.setCenter({
		worldBounds.position.x + worldBounds.size.x / 2.f,
		worldBounds.position.y + worldBounds.size.y / 2.f
	});

	m_window.setView(m_camera);
}

void Game::Run()
{
    while (m_window.isOpen())
    {
        const float dt = m_clock.restart().asSeconds();

        while (const std::optional event = m_window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                m_window.close();
            }
        }

        m_keyboardHandler.Update();
        PlayerInput input = m_keyboardHandler.GetInput();

        m_world.Update(input, dt);
        UpdateCamera();
        m_window.clear();
        m_window.setView(m_camera);
        m_world.Draw(m_window);
        m_window.display();
    }
}

void Game::UpdateCamera()
{
	sf::Vector2f cameraCenter = m_camera.getCenter();
	sf::Vector2f playerPosition = m_world.GetPositionPlayer();

	const float halfTrackingWidth = CAMERA_TRACKING_WIDTH / 2.f;
	const float halfTrackingHeight = CAMERA_TRACKING_HEIGHT / 2.f;

	if (playerPosition.x < cameraCenter.x - halfTrackingWidth)
	{
		cameraCenter.x = playerPosition.x + halfTrackingWidth;
	}
	else if (playerPosition.x > cameraCenter.x + halfTrackingWidth)
	{
		cameraCenter.x = playerPosition.x - halfTrackingWidth;
	}

	if (playerPosition.y < cameraCenter.y - halfTrackingHeight)
	{
		cameraCenter.y = playerPosition.y + halfTrackingHeight;
	}
	else if (playerPosition.y > cameraCenter.y + halfTrackingHeight)
	{
		cameraCenter.y = playerPosition.y - halfTrackingHeight;
	}

	sf::FloatRect worldBounds = m_world.GetBounds();
	sf::Vector2f cameraSize = m_camera.getSize();

	const float halfCameraWidth = cameraSize.x / 2.f;
	const float halfCameraHeight = cameraSize.y / 2.f;

	const float minCameraX = worldBounds.position.x + halfCameraWidth;
	const float maxCameraX = worldBounds.position.x + worldBounds.size.x - halfCameraWidth;

	const float minCameraY = worldBounds.position.y + halfCameraHeight;
	const float maxCameraY = worldBounds.position.y + worldBounds.size.y - halfCameraHeight;

	cameraCenter.x = std::clamp(cameraCenter.x, minCameraX, maxCameraX);
	cameraCenter.y = std::clamp(cameraCenter.y, minCameraY, maxCameraY);

	m_camera.setCenter(cameraCenter);
}