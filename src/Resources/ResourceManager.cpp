#include "Resources/ResourceManager.h"

#include <cmath>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include <toml++/toml.hpp>

namespace
{
	sf::Texture LoadTexture(const std::filesystem::path& path)
	{
		sf::Texture texture;
		if (!texture.loadFromFile(path))
		{
			throw std::runtime_error("Cannot load texture: " + path.string());
		}
		return texture;
	}

	const toml::node& RequireNode(const toml::table& table, const std::string& key, const std::string& where)
	{
		const toml::node* node = table.get(key);
		if (node == nullptr)
		{
			throw std::runtime_error(where + ": missing '" + key + "'");
		}
		return *node;
	}

	const toml::table& RequireTable(
		const toml::table& parent,
		const std::string& key,
		const std::string& where
	)
	{
		const toml::table* table = RequireNode(parent, key, where).as_table();
		if (table == nullptr)
		{
			throw std::runtime_error(where + "." + key + " must be a table");
		}
		return *table;
	}

	std::int64_t RequireInteger(
		const toml::table& table,
		const std::string& key,
		const std::string& where,
		const std::int64_t minValue
	)
	{
		const toml::node& node = RequireNode(table, key, where);
		const std::optional<std::int64_t> value = node.value<std::int64_t>();
		if (!value || *value < minValue || *value > 1000000)
		{
			throw std::runtime_error(where + "." + key + " must be an integer within " +
				std::to_string(minValue) + "..1000000");
		}
		return *value;
	}

	float RequireNumber(const toml::node& node, const std::string& where)
	{
		double value = 0.0;
		if (const auto floating = node.value<double>())
		{
			value = *floating;
		}
		else if (const auto integer = node.value<std::int64_t>())
		{
			value = static_cast<double>(*integer);
		}
		else
		{
			throw std::runtime_error(where + " must be a number");
		}

		if (!std::isfinite(value))
		{
			throw std::runtime_error(where + " must be finite");
		}
		const float result = static_cast<float>(value);
		if (!std::isfinite(result))
		{
			throw std::runtime_error(where + " is outside the supported float range");
		}
		return result;
	}

	sf::Vector2f RequireVector2f(const toml::table& table, const std::string& key)
	{
		const toml::array* values = RequireNode(table, key, "sprite sheet").as_array();
		if (values == nullptr || values->size() != 2)
		{
			throw std::runtime_error("sprite sheet." + key + " must contain exactly two numbers");
		}

		return {
			RequireNumber((*values)[0], "sprite sheet." + key + "[0]"),
			RequireNumber((*values)[1], "sprite sheet." + key + "[1]")
		};
	}

	SpriteSheetAsset::Clip ReadClip(const toml::table& table, const std::string& name)
	{
		const std::string where = "animations." + name;
		SpriteSheetAsset::Clip clip;
		clip.firstFrame = static_cast<int>(RequireInteger(table, "first_frame", where, 0));
		clip.frameCount = static_cast<int>(RequireInteger(table, "frame_count", where, 1));
		clip.fps = RequireNumber(RequireNode(table, "fps", where), where + ".fps");
		const toml::node& loopNode = RequireNode(table, "loop", where);
		const auto loop = loopNode.value<bool>();
		if (!loop)
		{
			throw std::runtime_error(where + ".loop must be true or false");
		}
		clip.loop = *loop;

		if (clip.fps <= 0.f)
		{
			throw std::runtime_error(where + ".fps must be greater than zero");
		}
		return clip;
	}

	std::unique_ptr<SpriteSheetAsset> LoadSpriteSheet(
		const std::filesystem::path& imagePath,
		const std::filesystem::path& relativePath
	)
	{
		sf::Texture texture = LoadTexture(imagePath);
		std::filesystem::path configPath = imagePath;
		configPath.replace_extension(".toml");

		toml::table root;
		try
		{
			root = toml::parse_file(configPath.string());
		}
		catch (const toml::parse_error& error)
		{
			throw std::runtime_error("Invalid animation TOML " + configPath.string() + ": " + error.what());
		}

		const toml::array* frameSizeValues = RequireNode(root, "frame_size", "sprite sheet").as_array();
		if (frameSizeValues == nullptr || frameSizeValues->size() != 2)
		{
			throw std::runtime_error("sprite sheet.frame_size must contain two positive integers");
		}

		const auto frameWidth = (*frameSizeValues)[0].value<std::int64_t>();
		const auto frameHeight = (*frameSizeValues)[1].value<std::int64_t>();
		if (!frameWidth || !frameHeight || *frameWidth < 1 || *frameHeight < 1 ||
			*frameWidth > 1000000 || *frameHeight > 1000000)
		{
			throw std::runtime_error("sprite sheet.frame_size must contain two positive integers");
		}

		const int columns = static_cast<int>(RequireInteger(root, "columns", "sprite sheet", 1));
		const sf::Vector2f origin = RequireVector2f(root, "origin");
		if (origin.x < 0.f || origin.x > 1.f || origin.y < 0.f || origin.y > 1.f)
		{
			throw std::runtime_error("sprite sheet.origin coordinates must be within 0..1");
		}

		float drawScale = 1.f;
		if (const toml::node* scaleNode = root.get("draw_scale"))
		{
			drawScale = RequireNumber(*scaleNode, "sprite sheet.draw_scale");
			if (drawScale <= 0.f)
			{
				throw std::runtime_error("sprite sheet.draw_scale must be greater than zero");
			}
		}

		const toml::table& animationTable = RequireTable(root, "animations", "sprite sheet");
		if (animationTable.empty())
		{
			throw std::runtime_error("sprite sheet.animations must contain at least one animation");
		}

		std::map<std::string, SpriteSheetAsset::Clip, std::less<>> clips;
		for (const auto& [key, node] : animationTable)
		{
			const std::string name(key.str());
			const toml::table* clipTable = node.as_table();
			if (clipTable == nullptr)
			{
				throw std::runtime_error("animations." + name + " must be a table");
			}
			clips.emplace(name, ReadClip(*clipTable, name));
		}

		const long long frameWidthPixels = *frameWidth;
		const long long frameHeightPixels = *frameHeight;
		if (static_cast<long long>(columns) * frameWidthPixels > texture.getSize().x)
		{
			throw std::runtime_error("Animation columns do not fit texture width: " + configPath.string());
		}

		for (const auto& [name, clip] : clips)
		{
			const long long lastFrame = static_cast<long long>(clip.firstFrame) + clip.frameCount - 1;
			const long long lastRow = lastFrame / columns;
			if ((lastRow + 1) * frameHeightPixels > texture.getSize().y)
			{
				throw std::runtime_error("Animation '" + name + "' does not fit texture height: " +
					configPath.string());
			}
		}

		return std::make_unique<SpriteSheetAsset>(
			std::move(texture),
			sf::Vector2i(static_cast<int>(*frameWidth), static_cast<int>(*frameHeight)),
			columns,
			origin,
			drawScale,
			std::move(clips),
			relativePath
		);
	}
}

ResourceManager::ResourceManager(ProjectPaths paths)
	: 	m_paths(std::move(paths))
{
}

const sf::Texture& ResourceManager::GetTexture(const std::filesystem::path& relativePath)
{
	const std::filesystem::path key = relativePath.lexically_normal();
	const auto cached = m_textures.find(key);
	if (cached != m_textures.end())
	{
		return *cached->second;
	}

	auto texture = std::make_unique<sf::Texture>(LoadTexture(m_paths.Resolve(key)));
	return *m_textures.emplace(key, std::move(texture)).first->second;
}

const SpriteSheetAsset& ResourceManager::GetSpriteSheet(const std::filesystem::path& relativePath)
{
	const std::filesystem::path key = relativePath.lexically_normal();
	const auto cached = m_spriteSheets.find(key);
	if (cached != m_spriteSheets.end())
	{
		return *cached->second;
	}

	auto sheet = LoadSpriteSheet(m_paths.Resolve(key), key);
	return *m_spriteSheets.emplace(key, std::move(sheet)).first->second;
}

const sf::Font& ResourceManager::GetFont(const std::filesystem::path& relativePath)
{
	const std::filesystem::path key = relativePath.lexically_normal();
	const auto cached = m_fonts.find(key);
	if (cached != m_fonts.end())
	{
		return *cached->second;
	}

	auto font = std::make_unique<sf::Font>();
	const std::filesystem::path absolutePath = m_paths.Resolve(key);
	if (!font->openFromFile(absolutePath))
	{
		throw std::runtime_error("Cannot load font: " + absolutePath.string());
	}
	return *m_fonts.emplace(key, std::move(font)).first->second;
}
