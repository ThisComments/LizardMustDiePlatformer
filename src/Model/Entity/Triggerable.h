#pragma once

#include <SFML/Graphics/Rect.hpp>

struct EntityContext;

// Contract of an object that reacts when the player overlaps its area (pickups, later
// checkpoints, chests, ...). A trigger never pushes the player or changes its velocity.
class Triggerable
{
public:
	virtual ~Triggerable() = default;

	virtual sf::FloatRect GetTriggerBounds() const = 0;

	// Called on every update while the player overlaps the trigger area,
	// so implementations must guard against repeated activation themselves.
	virtual void OnPlayerEnter(EntityContext& context) = 0;
};
