#pragma once

#include <string_view>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "Resources/SpriteSheetAsset.h"

class AnimationComponent
{
private:
	const SpriteSheetAsset* m_sheet;
	sf::Sprite m_sprite;
	const SpriteSheetAsset::Clip* m_clip;
	int m_frame = 0;
	float m_timer = 0.f;
	bool m_finished = false;
	bool m_facingRight = true;
	
	void ApplyFrame();
	void ApplyScale();

public:
	AnimationComponent(const SpriteSheetAsset& sheetAsset, std::string_view initialClip);

	void Play(std::string_view clipName);
	void Update(float dt);

	void SetPosition(sf::Vector2f position);
	void SetFacingRight(bool isFacingRight);
	void SetColor(sf::Color color);

	bool IsFinished() const;
	float GetClipDuration() const;
	void Draw(sf::RenderTarget& target) const;
};
