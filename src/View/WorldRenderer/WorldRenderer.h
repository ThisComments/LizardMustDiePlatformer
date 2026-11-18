#pragma once

#include <unordered_map>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>

#include "Animation/AnimationComponent.h"
#include "App/CameraController/CameraController.h"
#include "Model/Entity/Entity.h"
#include "Resources/ResourceManager.h"
#include "View/MapRenderer/MapRenderer.h"
#include "View/ParallaxBackground/ParallaxBackground.h"
#include "View/WorldRenderer/RenderLayer.h"
#include "World/World.h"

class WorldRenderer
{
private:
	struct EntityVisualState
	{
		VisualId visualId;
		RenderLayer layer;
		float renderDepth;
		AnimationComponent animation;
	};

	const World& m_world;
	const CameraController& m_camera;
	ResourceManager& m_resources;
	ParallaxBackground m_background;
	MapRenderer m_mapRenderer;
	std::unordered_map<const Entity*, EntityVisualState> m_entityVisuals;
	std::vector<EntityVisualState> m_finishingVisuals;
	float m_nextPickupPhase;

	bool IsEntityAliveInWorld(const Entity* targetEntity) const;
	void TransitionRemovedEntities();
	void UpdateFinishingVisuals(float dt);
	std::vector<const EntityVisualState*> SortVisualStates() const;
	void RenderVisualStates(
		sf::RenderTarget& target,
		const std::vector<const EntityVisualState*>& ordered
	) const;

public:
	WorldRenderer(
		const World& world,
		const CameraController& camera,
		ResourceManager& resourceManager
	);

	void Update(float dt);
	void Render(sf::RenderTarget& target) const;
};
