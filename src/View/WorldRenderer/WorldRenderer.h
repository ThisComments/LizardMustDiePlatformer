#pragma once

#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>

#include "App/CameraController/CameraController.h"
#include "Model/Entity/Entity.h"
#include "Resources/ResourceManager.h"
#include "View/MapRenderer/MapRenderer.h"
#include "View/ParallaxBackground/ParallaxBackground.h"
#include "World/World.h"

// Draws the world pass: background, tiles and every entity through Entity::Draw().
// It reads the world but never changes it, and knows nothing about concrete entity types.
class WorldRenderer
{
private:
	const World& m_world;
	const CameraController& m_camera;
	ParallaxBackground m_background;
	MapRenderer m_mapRenderer;

	std::vector<const Entity*> SortEntities() const;
	void RenderEntities(sf::RenderTarget& target, const std::vector<const Entity*>& ordered) const;
	void RenderForeground(sf::RenderTarget& target, const std::vector<const Entity*>& ordered) const;

public:
	WorldRenderer(const World& world, const CameraController& camera, ResourceManager& resources);

	// Leaves the world view active on the target; the caller restores the default view for the UI.
	void Render(sf::RenderTarget& target) const;
};
