#include "View/WorldRenderer/WorldRenderer.h"

#include <algorithm>

namespace
{
	constexpr sf::Color BACKGROUND_COLOR(135, 206, 235);
}

WorldRenderer::WorldRenderer(
	const World& world,
	const CameraController& camera,
	ResourceManager& resources
)
	: m_world(world),
	  m_camera(camera),
	  m_background(resources, world.GetBounds().size),
	  m_mapRenderer(resources)
{
}

void WorldRenderer::Render(sf::RenderTarget& target) const
{
	target.clear(BACKGROUND_COLOR);
	target.setView(m_camera.GetView());

	m_background.Draw(target, m_camera.GetView());
	m_mapRenderer.Draw(target, m_world.GetMap(), m_camera.GetView());

	const std::vector<const Entity*> ordered = SortEntities();
	RenderEntities(target, ordered);
	RenderForeground(target, ordered);
}

std::vector<const Entity*> WorldRenderer::SortEntities() const
{
	// Only this temporary list is sorted; the world's own container keeps its order.
	std::vector<const Entity*> ordered;
	ordered.reserve(m_world.GetEntities().size());
	for (const auto& entity : m_world.GetEntities())
	{
		ordered.push_back(entity.get());
	}

	std::stable_sort(
		ordered.begin(),
		ordered.end(),
		[](const Entity* a, const Entity* b)
		{
			if (a->GetRenderLayer() != b->GetRenderLayer())
			{
				return a->GetRenderLayer() < b->GetRenderLayer();
			}
			return a->GetRenderDepth() < b->GetRenderDepth();
		}
	);
	return ordered;
}

void WorldRenderer::RenderEntities(sf::RenderTarget& target, const std::vector<const Entity*>& ordered) const
{
	for (const Entity* entity : ordered)
	{
		if (entity->GetRenderLayer() != RenderLayer::Foreground)
		{
			entity->Draw(target);
		}
	}
}

void WorldRenderer::RenderForeground(sf::RenderTarget& target, const std::vector<const Entity*>& ordered) const
{
	for (const Entity* entity : ordered)
	{
		if (entity->GetRenderLayer() == RenderLayer::Foreground)
		{
			entity->Draw(target);
		}
	}
}
