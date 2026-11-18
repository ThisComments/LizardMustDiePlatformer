#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Model/Entity/VisualId.h"

struct EntityContext;

class Entity
{
public:
	virtual ~Entity() = default;

	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	virtual void Update(float dt, EntityContext& context) = 0;

	virtual sf::Vector2f GetPosition() const = 0;
	virtual sf::FloatRect GetBounds() const = 0;

	virtual sf::Vector2f GetVisualPosition() const
	{
		return GetPosition();
	}

	virtual float GetRenderDepth() const
	{
		return GetVisualPosition().y;
	}

	virtual bool IsFacingRight() const
	{
		return true;
	}

	virtual float GetDamageTimer() const
	{
		return 0.f;
	}

	virtual float GetHealTimer() const
	{
		return 0.f;
	}

	virtual bool IsPendingRemoval() const = 0;

	VisualId GetVisualId() const
	{
		return visualId;
	}

protected:
	explicit Entity(const VisualId initialVisualId = VISUAL_NONE)
		: visualId(initialVisualId)
	{
	}

	VisualId visualId;
};
