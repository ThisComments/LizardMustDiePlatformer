#include "View/WorldRenderer/WorldRenderer.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

#include <SFML/Graphics/Color.hpp>

#include "Resources/AssetPaths.h"

namespace
{
	constexpr sf::Color BACKGROUND_COLOR(135, 206, 235);
	constexpr sf::Color NORMAL_SPRITE_COLOR(255, 255, 255, 255);
	constexpr sf::Color DAMAGE_BLINK_COLOR(255, 95, 95, 110);
	constexpr sf::Color HEAL_FLASH_COLOR(90, 255, 130, 255);
	constexpr sf::Color HEAL_BLINK_COLOR(90, 255, 130, 130);

	constexpr float DAMAGE_BLINK_PERIOD = 0.1f;
	constexpr float COLLECTIBLE_PHASE_STEP = 0.173f;

	constexpr const char* IDLE_CLIP = "idle";
	constexpr const char* RUN_CLIP = "run";
	constexpr const char* JUMP_CLIP = "jump";
	constexpr const char* FALL_CLIP = "fall";
	constexpr const char* COLLECT_CLIP = "collect";

	struct VisualDescriptor
	{
		const char* spriteSheetPath;
		const char* clipName;
		const char* finishClipName;
		RenderLayer layer;
	};

	VisualDescriptor ResolveVisualDescriptor(const VisualId visualId)
	{
		switch (visualId)
		{
			case VISUAL_HERO_IDLE:
				return { AssetPaths::HeroSheet, IDLE_CLIP, nullptr, LAYER_CHARACTERS };
			case VISUAL_HERO_RUN:
				return { AssetPaths::HeroSheet, RUN_CLIP, nullptr, LAYER_CHARACTERS };
			case VISUAL_HERO_JUMP:
				return { AssetPaths::HeroSheet, JUMP_CLIP, nullptr, LAYER_CHARACTERS };
			case VISUAL_HERO_FALL:
				return { AssetPaths::HeroSheet, FALL_CLIP, nullptr, LAYER_CHARACTERS };
			case VISUAL_COIN:
				return { AssetPaths::CoinSheet, IDLE_CLIP, COLLECT_CLIP, LAYER_PICKUPS };
			case VISUAL_GEM:
				return { AssetPaths::GemSheet, IDLE_CLIP, COLLECT_CLIP, LAYER_PICKUPS };
			case VISUAL_SPEED_POTION:
				return { AssetPaths::SpeedPotion, IDLE_CLIP, nullptr, LAYER_PICKUPS };
			case VISUAL_NONE:
				break;
		}

		throw std::logic_error("Unsupported VisualId in WorldRenderer");
	}

	sf::Color ResolveEntityColor(const Entity& entity)
	{
		const float damageTimer = entity.GetDamageTimer();
		const float healTimer = entity.GetHealTimer();

		if (damageTimer > 0.f)
		{
			const int blinkStep = static_cast<int>(damageTimer / DAMAGE_BLINK_PERIOD);
			const bool isDimPhase = (blinkStep % 2 == 0);
			if (healTimer > 0.f)
			{
				return isDimPhase ? HEAL_BLINK_COLOR : HEAL_FLASH_COLOR;
			}
			return isDimPhase ? DAMAGE_BLINK_COLOR : NORMAL_SPRITE_COLOR;
		}

		if (healTimer > 0.f)
		{
			return HEAL_FLASH_COLOR;
		}

		return NORMAL_SPRITE_COLOR;
	}
}

WorldRenderer::WorldRenderer(
	const World& world,
	const CameraController& camera,
	ResourceManager& resourceManager
)
	: 	m_world(world),
		m_camera(camera),
		m_resources(resourceManager),
		m_background(resourceManager, world.GetBounds().size),
		m_mapRenderer(resourceManager),
		m_nextPickupPhase(0.f)
{
	Update(0.f);
}

void WorldRenderer::Update(const float dt)
{
	TransitionRemovedEntities();

	for (const std::unique_ptr<Entity>& entityPtr : m_world.GetEntities())
	{
		const Entity* entity = entityPtr.get();
		const VisualId currentVisualId = entity->GetVisualId();
		if (currentVisualId == VISUAL_NONE)
		{
			continue;
		}

		const VisualDescriptor descriptor = ResolveVisualDescriptor(currentVisualId);
		auto it = m_entityVisuals.find(entity);

		if (it == m_entityVisuals.end())
		{
			const SpriteSheetAsset& sheet = m_resources.GetSpriteSheet(descriptor.spriteSheetPath);
			if (descriptor.finishClipName != nullptr)
			{
				sheet.RequireClips({ descriptor.clipName, descriptor.finishClipName });
			}
			else
			{
				sheet.RequireClips({ descriptor.clipName });
			}

			auto [insertedIt, inserted] = m_entityVisuals.emplace(
				entity,
				EntityVisualState{
					currentVisualId,
					descriptor.layer,
					entity->GetRenderDepth(),
					AnimationComponent(sheet, descriptor.clipName)
				}
			);

			if (descriptor.layer == LAYER_PICKUPS)
			{
				insertedIt->second.animation.Update(
					m_nextPickupPhase * insertedIt->second.animation.GetClipDuration()
				);
				m_nextPickupPhase = std::fmod(m_nextPickupPhase + COLLECTIBLE_PHASE_STEP, 1.f);
			}

			it = insertedIt;
		}
		else if (it->second.visualId != currentVisualId)
		{
			it->second.visualId = currentVisualId;
			it->second.layer = descriptor.layer;
			it->second.animation.Play(descriptor.clipName);
		}

		it->second.renderDepth = entity->GetRenderDepth();
		it->second.animation.SetFacingRight(entity->IsFacingRight());
		it->second.animation.SetPosition(entity->GetVisualPosition());
		it->second.animation.SetColor(ResolveEntityColor(*entity));
		it->second.animation.Update(dt);
	}

	UpdateFinishingVisuals(dt);
}

void WorldRenderer::Render(sf::RenderTarget& target) const
{
	target.clear(BACKGROUND_COLOR);
	target.setView(m_camera.GetView());

	m_background.Draw(target, m_camera.GetView());
	m_mapRenderer.Draw(target, m_world.GetMap(), m_camera.GetView());

	const std::vector<const EntityVisualState*> ordered = SortVisualStates();
	RenderVisualStates(target, ordered);
}

bool WorldRenderer::IsEntityAliveInWorld(const Entity* targetEntity) const
{
	for (const std::unique_ptr<Entity>& entity : m_world.GetEntities())
	{
		if (entity.get() == targetEntity)
		{
			return true;
		}
	}
	return false;
}

void WorldRenderer::TransitionRemovedEntities()
{
	for (auto it = m_entityVisuals.begin(); it != m_entityVisuals.end();)
	{
		if (!IsEntityAliveInWorld(it->first))
		{
			const VisualDescriptor descriptor = ResolveVisualDescriptor(it->second.visualId);
			if (descriptor.finishClipName != nullptr)
			{
				it->second.animation.SetColor(NORMAL_SPRITE_COLOR);
				it->second.animation.Play(descriptor.finishClipName);
				m_finishingVisuals.push_back(std::move(it->second));
			}
			it = m_entityVisuals.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void WorldRenderer::UpdateFinishingVisuals(const float dt)
{
	for (EntityVisualState& state : m_finishingVisuals)
	{
		state.animation.Update(dt);
	}

	m_finishingVisuals.erase(
		std::remove_if(
			m_finishingVisuals.begin(),
			m_finishingVisuals.end(),
			[](const EntityVisualState& state)
			{
				return state.animation.IsFinished();
			}
		),
		m_finishingVisuals.end()
	);
}

std::vector<const WorldRenderer::EntityVisualState*> WorldRenderer::SortVisualStates() const
{
	std::vector<const EntityVisualState*> ordered;
	ordered.reserve(m_entityVisuals.size() + m_finishingVisuals.size());

	for (const auto& [entity, state] : m_entityVisuals)
	{
		ordered.push_back(&state);
	}

	for (const EntityVisualState& state : m_finishingVisuals)
	{
		ordered.push_back(&state);
	}

	std::stable_sort(
		ordered.begin(),
		ordered.end(),
		[](const EntityVisualState* a, const EntityVisualState* b)
		{
			if (a->layer != b->layer)
			{
				return a->layer < b->layer;
			}
			return a->renderDepth < b->renderDepth;
		}
	);

	return ordered;
}

void WorldRenderer::RenderVisualStates(
	sf::RenderTarget& target,
	const std::vector<const EntityVisualState*>& ordered
) const
{
	for (const EntityVisualState* state : ordered)
	{
		state->animation.Draw(target);
	}
}
