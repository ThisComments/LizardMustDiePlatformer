#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

class CameraController
{
private:
	sf::View m_view;
	sf::FloatRect m_worldBounds;

public:
	CameraController(sf::Vector2f viewSize, const sf::FloatRect& worldBounds);

	void Update(sf::Vector2f targetPosition);
	const sf::View& GetView() const;
};
