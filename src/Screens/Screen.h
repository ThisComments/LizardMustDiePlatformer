#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "Screens/ScreenCommand.h"

class Screen
{
public:
	virtual ~Screen() = default;

	virtual ScreenCommand HandleEvent(const sf::Event& event, const sf::RenderWindow& window) = 0;
	virtual ScreenCommand Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow& window) = 0;

	virtual void OnEnter()
	{
	}

	virtual bool IsOverlay() const
	{
		return false;
	}
};
