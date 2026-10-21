#pragma once

#include <filesystem>
#include <istream>
#include <string>

#include "World/Map/Map.h"

class LevelRepository
{
private:
	Map Parse(std::istream& stream, const std::string& sourceName) const;

public:
	Map Load(const std::filesystem::path& levelFile) const;
};
