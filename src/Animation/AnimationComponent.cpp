#include "Animation/AnimationComponent.h"

#include <stdexcept>
#include <string>

namespace
{
	const SpriteSheetAsset::Clip& FindClipOrThrow(
		const SpriteSheetAsset& sheet,
		const std::string_view name
	)
	{
		const SpriteSheetAsset::Clip* clip = sheet.FindClip(name);
		if (clip == nullptr)
		{
			throw std::runtime_error("Sprite sheet " + sheet.GetSource().string() +
				" has no animation '" + std::string(name) + "'");
		}
		return *clip;
	}

	sf::IntRect CalculateFrameRect(
		const int frameIndex,
		const sf::Vector2i frameSize,
		const int columns
	)
	{
		const int column = frameIndex % columns;
		const int row = frameIndex / columns;
		return sf::IntRect(
			{column * frameSize.x, row * frameSize.y},
			{frameSize.x, frameSize.y}
		);
	}
}

AnimationComponent::AnimationComponent(const SpriteSheetAsset& sheet, const std::string_view initialClip)
	: m_sheet(sheet),
	  m_sprite(sheet.GetTexture()),
	  m_clip(&FindClipOrThrow(sheet, initialClip))
{
	const sf::Vector2i frameSize = m_sheet.GetFrameSize();
	const sf::Vector2f origin = m_sheet.GetOrigin();
	m_sprite.setOrigin({
		origin.x * static_cast<float>(frameSize.x),
		origin.y * static_cast<float>(frameSize.y)
	});
	ApplyScale();
	ApplyFrame();
}

void AnimationComponent::Play(const std::string_view clipName)
{
	const SpriteSheetAsset::Clip& clip = FindClipOrThrow(m_sheet, clipName);
	if (&clip == m_clip)
	{
		return;
	}

	m_clip = &clip;
	m_frame = 0;
	m_timer = 0.f;
	m_finished = false;
	ApplyFrame();
}

void AnimationComponent::Update(const float dt)
{
	if (m_finished || dt <= 0.f)
	{
		return;
	}

	m_timer += dt;
	const float frameTime = 1.f / m_clip->fps;

	while (m_timer >= frameTime)
	{
		m_timer -= frameTime;
		++m_frame;

		if (m_frame < m_clip->frameCount)
		{
			continue;
		}

		if (m_clip->loop)
		{
			m_frame = 0;
			continue;
		}

		m_frame = m_clip->frameCount - 1;
		m_timer = 0.f;
		m_finished = true;
		break;
	}

	ApplyFrame();
}

void AnimationComponent::SetPosition(const sf::Vector2f position)
{
	m_sprite.setPosition(position);
}

void AnimationComponent::SetFacingRight(const bool facingRight)
{
	if (m_facingRight == facingRight)
	{
		return;
	}

	m_facingRight = facingRight;
	ApplyScale();
}

bool AnimationComponent::IsFinished() const
{
	return m_finished;
}

float AnimationComponent::GetClipDuration() const
{
	return static_cast<float>(m_clip->frameCount) / m_clip->fps;
}

void AnimationComponent::Draw(sf::RenderTarget& target) const
{
	target.draw(m_sprite);
}

void AnimationComponent::ApplyFrame()
{
	m_sprite.setTextureRect(CalculateFrameRect(
		m_clip->firstFrame + m_frame,
		m_sheet.GetFrameSize(),
		m_sheet.GetColumns()
	));
}

void AnimationComponent::ApplyScale()
{
	const float scale = m_sheet.GetDrawScale();
	m_sprite.setScale({m_facingRight ? scale : -scale, scale});
}
