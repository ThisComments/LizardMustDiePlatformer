#pragma once

#include <filesystem>
#include <map>
#include <memory>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Resources/ProjectPaths.h"
#include "Resources/SpriteSheetAsset.h"

class ResourceManager
{
private:
	template <typename T>
	using Cache = std::map<std::filesystem::path, std::unique_ptr<T>>;

	ProjectPaths m_paths;
	Cache<sf::Texture> m_textures;
	Cache<SpriteSheetAsset> m_spriteSheets;
	Cache<sf::Font> m_fonts;

public:
	ResourceManager(ProjectPaths paths);

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	const sf::Texture& GetTexture(const std::filesystem::path& relativePath);

	const SpriteSheetAsset& GetSpriteSheet(const std::filesystem::path& relativePath);

	const sf::Font& GetFont(const std::filesystem::path& relativePath);
};
