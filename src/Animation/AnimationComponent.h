#pragma once

#include <string_view>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "Resources/SpriteSheetAsset.h"

// Plays clips from one sprite sheet. Physics and input stay outside this class.
class AnimationComponent
{
private:
	const SpriteSheetAsset& m_sheet;
	sf::Sprite m_sprite;
	const SpriteSheetAsset::Clip* m_clip;
	int m_frame = 0;
	float m_timer = 0.f;
	bool m_finished = false;
	bool m_facingRight = true;

	void ApplyFrame();
	void ApplyScale();

public:
	AnimationComponent(const SpriteSheetAsset& sheet, std::string_view initialClip);

	void Play(std::string_view clipName);
	void Update(float dt);

	void SetPosition(sf::Vector2f position);
	void SetFacingRight(bool facingRight);

	bool IsFinished() const;
	float GetClipDuration() const;
	void Draw(sf::RenderTarget& target) const;
};
