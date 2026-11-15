#pragma once

#include <filesystem>
#include <functional>
#include <initializer_list>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

class SpriteSheetAsset
{
public:
	struct Clip
	{
		int firstFrame = 0;
		int frameCount = 1;
		float fps = 1.f;
		bool loop = true;
	};

private:
	sf::Texture m_texture;
	sf::Vector2i m_frameSize;
	int m_columns;
	sf::Vector2f m_origin;
	float m_drawScale;
	std::map<std::string, Clip, std::less<>> m_clips;
	std::filesystem::path m_source;

public:
	SpriteSheetAsset(
		sf::Texture texture,
		sf::Vector2i frameSize,
		int columns,
		sf::Vector2f origin,
		float drawScale,
		std::map<std::string, Clip, std::less<>> clips,
		std::filesystem::path source
	)
		: 	m_texture(std::move(texture)),
			m_frameSize(frameSize),
			m_columns(columns),
			m_origin(origin),
			m_drawScale(drawScale),
			m_clips(std::move(clips)),
			m_source(std::move(source))
	{
	}

	SpriteSheetAsset(const SpriteSheetAsset&) = delete;
	SpriteSheetAsset& operator=(const SpriteSheetAsset&) = delete;

	const sf::Texture& GetTexture() const
	{
		return m_texture;
	}

	sf::Vector2i GetFrameSize() const
	{
		return m_frameSize;
	}

	int GetColumns() const
	{
		return m_columns;
	}

	sf::Vector2f GetOrigin() const
	{
		return m_origin;
	}

	float GetDrawScale() const
	{
		return m_drawScale;
	}

	const Clip* FindClip(const std::string_view name) const
	{
		const auto it = m_clips.find(name);
		return it == m_clips.end() ? nullptr : &it->second;
	}

	void RequireClips(const std::initializer_list<std::string_view> names) const
	{
		for (const std::string_view name : names)
		{
			if (FindClip(name) == nullptr)
			{
				throw std::runtime_error("Sprite sheet " + m_source.string() +
					" has no animation '" + std::string(name) + "'");
			}
		}
	}

	const std::filesystem::path& GetSource() const
	{
		return m_source;
	}
};
