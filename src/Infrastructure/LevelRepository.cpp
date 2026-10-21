#include "Infrastructure/LevelRepository.h"

#include <fstream>
#include <optional>
#include <stdexcept>
#include <system_error>
#include <vector>

namespace
{
	struct TileSymbol
	{
		char symbol;
		TileType type;
	};

	constexpr TileSymbol TILE_SYMBOLS[] = {
		{'.', TileType::Empty},
		{'G', TileType::Ground},
		{'S', TileType::Stone},
		{'W', TileType::Water},
		{'I', TileType::Ice},
		{'L', TileType::Lava},
		{'P', TileType::WoodPlanks}
	};

	std::optional<TileType> FindTileType(const char symbol)
	{
		for (const TileSymbol& entry : TILE_SYMBOLS)
		{
			if (entry.symbol == symbol)
			{
				return entry.type;
			}
		}
		return std::nullopt;
	}

	std::string Location(const std::string& sourceName, const int lineNumber)
	{
		return sourceName + ":" + std::to_string(lineNumber) + ": ";
	}
}

Map LevelRepository::Load(const std::filesystem::path& levelFile) const
{
	std::error_code error;
	if (!std::filesystem::exists(levelFile, error))
	{
		throw std::runtime_error("Level file does not exist: " + levelFile.string());
	}

	if (!std::filesystem::is_regular_file(levelFile, error))
	{
		throw std::runtime_error("Level path is not a regular file: " + levelFile.string());
	}

	std::ifstream file(levelFile);
	if (!file)
	{
		throw std::runtime_error("Cannot open level file: " + levelFile.string());
	}

	return Parse(file, levelFile.string());
}

Map LevelRepository::Parse(std::istream& stream, const std::string& sourceName) const
{
	std::vector<TileType> tiles;
	int width = 0;
	int height = 0;
	int lineNumber = 0;
	std::string line;

	while (std::getline(stream, line))
	{
		++lineNumber;
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}
		if (line.empty())
		{
			continue;
		}

		if (width == 0)
		{
			width = static_cast<int>(line.size());
		}
		else if (line.size() != width)
		{
			throw std::runtime_error(Location(sourceName, lineNumber) + "row has " +
				std::to_string(line.size()) + " tiles, expected " + std::to_string(width));
		}

		for (const char symbol : line)
		{
			const std::optional<TileType> type = FindTileType(symbol);
			if (!type)
			{
				throw std::runtime_error(Location(sourceName, lineNumber) +
					"invalid tile character '" + std::string(1, symbol) + "'");
			}
			tiles.push_back(*type);
		}
		++height;
	}

	if (height == 0)
	{
		throw std::runtime_error("Level file is empty: " + sourceName);
	}

	return Map(width, height, std::move(tiles));
}
