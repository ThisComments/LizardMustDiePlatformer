#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Model/Entity/RenderLayer.h"

struct EntityContext;

// Common interface of every dynamic object that lives in the World.
// An entity never loads resources, touches the window, or talks to audio/UI:
// it reports facts through EntityContext::events instead.
class Entity
{
protected:
	Entity() = default;

public:
	virtual ~Entity() = default;

	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	virtual void Update(float dt, EntityContext& context) = 0;
	virtual void Draw(sf::RenderTarget& target) const = 0;

	virtual sf::Vector2f GetPosition() const = 0;
	virtual sf::FloatRect GetBounds() const = 0;

	virtual RenderLayer GetRenderLayer() const = 0;
	virtual float GetRenderDepth() const = 0;

	virtual bool IsPendingRemoval() const = 0;
};
