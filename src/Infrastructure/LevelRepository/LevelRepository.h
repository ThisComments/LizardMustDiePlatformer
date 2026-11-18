#pragma once

#include <filesystem>
#include <istream>
#include <string>

#include "Infrastructure/LevelRepository/LevelData.h"

class LevelRepository
{
private:
	LevelData Parse(std::istream& stream, const std::string& sourceName) const;

public:
	LevelData Load(const std::filesystem::path& levelFile) const;
};
