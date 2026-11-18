#pragma once

#include <SFML/Graphics/Rect.hpp>

class Triggerable
{
public:
	virtual ~Triggerable() = default;

	virtual sf::FloatRect GetTriggerBounds() const = 0;
};
