#include "Infrastructure/LevelRepository/LevelRepository.h"

#include <fstream>
#include <optional>
#include <sstream>
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
		{ '.', TileType::Empty },
		{ 'G', TileType::Ground },
		{ 'S', TileType::Stone },
		{ 'W', TileType::Water },
		{ 'I', TileType::Ice },
		{ 'L', TileType::Lava },
		{ 'P', TileType::WoodPlanks },
		{ 'N', TileType::Snow },
		{ 'R', TileType::SnowBrick },
		{ 'T', TileType::LizardTech },
		{ 'b', TileType::WoodWallBg },
		{ 'r', TileType::StoneWallBg }
	};

	constexpr const char* OBJECTS_SECTION = "[objects]";

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

	std::optional<CollectibleType> FindCollectibleType(const std::string& name)
	{
		if (name == "coin")
		{
			return COLLECTIBLE_COIN;
		}
		if (name == "gem")
		{
			return COLLECTIBLE_GEM;
		}
		if (name == "speed_potion")
		{
			return COLLECTIBLE_SPEED_POTION;
		}
		return std::nullopt;
	}

	std::string Location(const std::string& sourceName, const int lineNumber)
	{
		return sourceName + ":" + std::to_string(lineNumber) + ": ";
	}

	CellCoordinates ParseCellCoordinates(
		std::istringstream& lineStream,
		const int mapWidth,
		const int mapHeight,
		const std::string& sourceName,
		const int lineNumber
	)
	{
		int col = 0;
		int row = 0;
		if (!(lineStream >> col >> row))
		{
			throw std::runtime_error(
				Location(sourceName, lineNumber) + "expected '<type> <column> <row>'"
			);
		}
		if (col < 0 || col >= mapWidth || row < 0 || row >= mapHeight)
		{
			throw std::runtime_error(
				Location(sourceName, lineNumber) + "position is outside the map"
			);
		}
		return { col, row };
	}

	TileZone ParseTileZone(
		std::istringstream& lineStream,
		const int mapWidth,
		const int mapHeight,
		const std::string& sourceName,
		const int lineNumber
	)
	{
		int col = 0;
		int row = 0;
		int width = 0;
		int height = 0;
		if (!(lineStream >> col >> row >> width >> height))
		{
			throw std::runtime_error(
				Location(sourceName, lineNumber) +
				"expected 'finish <col> <row> <width> <height>'"
			);
		}
		if (col < 0 || row < 0 || width <= 0 || height <= 0 ||
			col + width > mapWidth || row + height > mapHeight)
		{
			throw std::runtime_error(
				Location(sourceName, lineNumber) + "finish zone is outside the map"
			);
		}
		return { col, row, width, height };
	}
}

LevelData LevelRepository::Load(const std::filesystem::path& levelFile) const
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

LevelData LevelRepository::Parse(std::istream& stream, const std::string& sourceName) const
{
	std::vector<TileType> tiles;
	std::vector<std::string> objectLines;
	std::vector<int> objectLineNumbers;
	int width = 0;
	int height = 0;
	int lineNumber = 0;
	bool inObjects = false;
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

		if (line == OBJECTS_SECTION)
		{
			inObjects = true;
			continue;
		}
		if (inObjects)
		{
			objectLines.push_back(line);
			objectLineNumbers.push_back(lineNumber);
			continue;
		}

		if (width == 0)
		{
			width = static_cast<int>(line.size());
		}
		else if (static_cast<int>(line.size()) != width)
		{
			throw std::runtime_error(
				Location(sourceName, lineNumber) + "row has " +
				std::to_string(line.size()) + " tiles, expected " + std::to_string(width)
			);
		}

		for (const char symbol : line)
		{
			const std::optional<TileType> type = FindTileType(symbol);
			if (!type)
			{
				throw std::runtime_error(
					Location(sourceName, lineNumber) +
					"invalid tile character '" + std::string(1, symbol) + "'"
				);
			}
			tiles.push_back(*type);
		}
		++height;
	}

	if (height == 0)
	{
		throw std::runtime_error("Level file is empty: " + sourceName);
	}

	LevelData level{ Map(width, height, std::move(tiles)), {}, {}, {} };
	bool hasPlayerSpawn = false;
	bool hasFinishZone = false;

	for (std::size_t i = 0; i < objectLines.size(); ++i)
	{
		std::istringstream lineStream(objectLines[i]);
		std::string typeName;
		if (!(lineStream >> typeName) || typeName[0] == '#')
		{
			continue;
		}

		if (typeName == "player")
		{
			level.playerSpawn = ParseCellCoordinates(
				lineStream,
				width,
				height,
				sourceName,
				objectLineNumbers[i]
			);
			hasPlayerSpawn = true;
			continue;
		}

		if (typeName == "finish")
		{
			level.finishZone = ParseTileZone(
				lineStream,
				width,
				height,
				sourceName,
				objectLineNumbers[i]
			);
			hasFinishZone = true;
			continue;
		}

		const std::optional<CollectibleType> type = FindCollectibleType(typeName);
		if (!type)
		{
			throw std::runtime_error(
				Location(sourceName, objectLineNumbers[i]) +
				"unknown object type '" + typeName + "'"
			);
		}

		const CellCoordinates cell = ParseCellCoordinates(
			lineStream,
			width,
			height,
			sourceName,
			objectLineNumbers[i]
		);
		level.collectibles.push_back({ *type, cell.col, cell.row });
	}

	if (!hasPlayerSpawn)
	{
		throw std::runtime_error(sourceName + ": missing 'player <col> <row>' in [objects]");
	}

	if (!hasFinishZone)
	{
		throw std::runtime_error(
			sourceName + ": missing 'finish <col> <row> <width> <height>' in [objects]"
		);
	}

	return level;
}
