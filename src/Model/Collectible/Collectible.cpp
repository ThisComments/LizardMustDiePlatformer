#include "Model/Collectible/Collectible.h"

#include "Model/Collectible/CollectibleDefs.h"
#include "World/EntityContext.h"

namespace
{
	constexpr const char* IDLE_CLIP = "idle";
	constexpr const char* COLLECT_CLIP = "collect";

	const SpriteSheetAsset& RequireCollectibleClips(const SpriteSheetAsset& sheet)
	{
		sheet.RequireClips({IDLE_CLIP, COLLECT_CLIP});
		return sheet;
	}
}

Collectible::Collectible(
	const CollectibleType type,
	const sf::Vector2f position,
	const SpriteSheetAsset& sheet,
	const float animationPhase
)
	: m_state(CollectibleState::Available),
	  m_position(position),
	  m_bounds(position - GetCollectibleDef(type).size / 2.f, GetCollectibleDef(type).size),
	  m_points(GetCollectibleDef(type).points),
	  m_animation(RequireCollectibleClips(sheet), IDLE_CLIP)
{
	m_animation.SetPosition(m_position);
	m_animation.Update(animationPhase * m_animation.GetClipDuration());
}

void Collectible::Update(const float dt, EntityContext&)
{
	m_animation.Update(dt);

	if (m_state == CollectibleState::Collecting && m_animation.IsFinished())
	{
		m_state = CollectibleState::Finished;
	}
}

void Collectible::Draw(sf::RenderTarget& target) const
{
	m_animation.Draw(target);
}

sf::Vector2f Collectible::GetPosition() const
{
	return m_position;
}

sf::FloatRect Collectible::GetBounds() const
{
	return m_bounds;
}

RenderLayer Collectible::GetRenderLayer() const
{
	return RenderLayer::Pickups;
}

float Collectible::GetRenderDepth() const
{
	return m_position.y;
}

bool Collectible::IsPendingRemoval() const
{
	return m_state == CollectibleState::Finished;
}

sf::FloatRect Collectible::GetTriggerBounds() const
{
	return m_bounds;
}

void Collectible::OnPlayerEnter(EntityContext& context)
{
	Collect(context);
}

void Collectible::Collect(EntityContext& context)
{
	// The state changes before anything else so repeated trigger calls cannot score twice.
	if (m_state != CollectibleState::Available)
	{
		return;
	}

	m_state = CollectibleState::Collecting;
	m_animation.Play(COLLECT_CLIP);
	context.events.push_back({WorldEventType::CollectibleCollected, m_points});
}
