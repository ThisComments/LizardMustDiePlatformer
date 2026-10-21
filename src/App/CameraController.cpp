#include "App/CameraController.h"

#include <algorithm>

const float CAMERA_TRACKING_WIDTH = 600.f;
const float CAMERA_TRACKING_HEIGHT = 400.f;

CameraController::CameraController(const sf::Vector2f viewSize, const sf::FloatRect& worldBounds)
	: m_view(sf::FloatRect({0.f, 0.f}, viewSize)),
	  m_worldBounds(worldBounds)
{
	m_view.setCenter({
		m_worldBounds.position.x + m_worldBounds.size.x / 2.f,
		m_worldBounds.position.y + m_worldBounds.size.y / 2.f
	});
}

void CameraController::Update(const sf::Vector2f targetPosition)
{
	sf::Vector2f cameraCenter = m_view.getCenter();

	const float halfTrackingWidth = CAMERA_TRACKING_WIDTH / 2.f;
	const float halfTrackingHeight = CAMERA_TRACKING_HEIGHT / 2.f;

	if (targetPosition.x < cameraCenter.x - halfTrackingWidth)
	{
		cameraCenter.x = targetPosition.x + halfTrackingWidth;
	}
	else if (targetPosition.x > cameraCenter.x + halfTrackingWidth)
	{
		cameraCenter.x = targetPosition.x - halfTrackingWidth;
	}

	if (targetPosition.y < cameraCenter.y - halfTrackingHeight)
	{
		cameraCenter.y = targetPosition.y + halfTrackingHeight;
	}
	else if (targetPosition.y > cameraCenter.y + halfTrackingHeight)
	{
		cameraCenter.y = targetPosition.y - halfTrackingHeight;
	}

	const sf::Vector2f cameraSize = m_view.getSize();

	const float halfCameraWidth = cameraSize.x / 2.f;
	const float halfCameraHeight = cameraSize.y / 2.f;

	const float minCameraX = m_worldBounds.position.x + halfCameraWidth;
	const float maxCameraX = m_worldBounds.position.x + m_worldBounds.size.x - halfCameraWidth;

	const float minCameraY = m_worldBounds.position.y + halfCameraHeight;
	const float maxCameraY = m_worldBounds.position.y + m_worldBounds.size.y - halfCameraHeight;

	cameraCenter.x = std::clamp(cameraCenter.x, minCameraX, maxCameraX);
	cameraCenter.y = std::clamp(cameraCenter.y, minCameraY, maxCameraY);

	m_view.setCenter(cameraCenter);
}

const sf::View& CameraController::GetView() const
{
	return m_view;
}
