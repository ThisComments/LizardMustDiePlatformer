#include "Map.h"
#include "World/Map/TileProperties.h"

#include <cmath>
#include <fstream>
#include <stdexcept>
#include <string>

uint32_t GetIndex(const Map& map, const uint32_t col, const uint32_t row)
{
    return row * map.width + col;
}

struct TileChar
{
    char symbol;
    TileType type;
    uint32_t visualId;
};

constexpr TileChar TILE_CHARS[] = {
    {'.', TileType::Empty, TVI_NONE},
    {'G', TileType::Ground, TVI_GROUND},
    {'S', TileType::Stone, TVI_STONE},
    {'W', TileType::Water, TVI_WATER},
    {'I', TileType::Lava, TVI_LAVA},
    {'L', TileType::Ice, TVI_ICE}
};

std::filesystem::path DefaultLevelPath()
{
    #ifdef PLATFORMER_LEVELS_DIR
        const std::filesystem::path levelsDir = PLATFORMER_LEVELS_DIR;
    #else
        const std::filesystem::path levelsDir = std::filesystem::path(__FILE__).parent_path() / "levels";
    #endif
        return levelsDir / "test_level.txt";
}

void MapInit(Map& map)
{
    MapLoad(map, DefaultLevelPath());
}

void MapLoad(Map& map, const std::filesystem::path& levelFile)
{
    std::ifstream file(levelFile);
    if (!file)
    {
        throw std::runtime_error("Cannot open level file: " + levelFile.string());
    }

    Map loaded;
    std::string line;
    uint32_t lineNumber = 0;
    while (std::getline(file, line))
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

        if (loaded.width == 0)
        {
            loaded.width = static_cast<uint32_t>(line.size());
        }
        else if (line.size() != loaded.width)
        {
            throw std::runtime_error(levelFile.string() + ":" + std::to_string(lineNumber) + ": row has " +
                std::to_string(line.size()) + " tiles, expected " + std::to_string(loaded.width));
        }

        for (const char symbol : line)
        {
            const TileChar* tile = nullptr;
            for (const TileChar& entry : TILE_CHARS)
            {
                if (entry.symbol == symbol)
                {
                    tile = &entry;
                }
            }
            if (!tile)
            {
                throw std::runtime_error(levelFile.string() + ":" + std::to_string(lineNumber) +
                    ": invalid tile character '" + std::string(1, symbol) + "'");
            }
            loaded.grid.push_back(tile->type);
            loaded.visualIds.push_back(tile->visualId);
        }
        ++loaded.height;
    }

    if (loaded.height == 0)
    {
        throw std::runtime_error("Level file is empty: " + levelFile.string());
    }

    map = std::move(loaded);
}

TileType MapGetTile(const Map& map, const float worldX, const float worldY)
{
    if (!std::isfinite(worldX) || !std::isfinite(worldY) ||
        worldX < 0.f || worldY < 0.f || map.width == 0 || map.height == 0)
    {
        return TileType::Empty;
    }

    const float mapWidth = static_cast<float>(map.width) * TILE_SIZE;
    const float mapHeight = static_cast<float>(map.height) * TILE_SIZE;
    if (worldX >= mapWidth || worldY >= mapHeight)
    {
        return TileType::Empty;
    }

    const uint32_t col = static_cast<uint32_t>(worldX / TILE_SIZE);
    const uint32_t row = static_cast<uint32_t>(worldY / TILE_SIZE);
    const uint32_t index = GetIndex(map, col, row);
    return index < map.grid.size() ? map.grid[index] : TileType::Empty;
}

uint32_t MapGetVisualId(const Map& map, const float worldX, const float worldY)
{
    if (!std::isfinite(worldX) || !std::isfinite(worldY) ||
        worldX < 0.f || worldY < 0.f || map.width == 0 || map.height == 0)
    {
        return TVI_NONE;
    }

    const float mapWidth = static_cast<float>(map.width) * TILE_SIZE;
    const float mapHeight = static_cast<float>(map.height) * TILE_SIZE;
    if (worldX >= mapWidth || worldY >= mapHeight)
    {
        return TVI_NONE;
    }

    const uint32_t col = static_cast<uint32_t>(worldX / TILE_SIZE);
    const uint32_t row = static_cast<uint32_t>(worldY / TILE_SIZE);
    const uint32_t index = GetIndex(map, col, row);
    return index < map.visualIds.size() ? map.visualIds[index] : TVI_NONE;
}

bool MapIsSolid(const Map& map, const float worldX, const float worldY)
{
    const TileType tileType = MapGetTile(map, worldX, worldY);
    return GetTileProperties(tileType).isSolid;
}
