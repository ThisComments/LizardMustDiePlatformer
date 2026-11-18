#pragma once

#include <filesystem>
#include <utility>

class ProjectPaths
{
private:
	std::filesystem::path m_root;

public:
	ProjectPaths(std::filesystem::path root)
		: 	m_root(std::move(root))
	{
	}

	static ProjectPaths FromExecutable(const char* executablePath)
	{
		if (executablePath == nullptr || *executablePath == '\0')
		{
			return ProjectPaths(std::filesystem::current_path());
		}
		return ProjectPaths(std::filesystem::absolute(executablePath).parent_path());
	}

	std::filesystem::path Resolve(const std::filesystem::path& relativePath) const
	{
		return m_root / relativePath;
	}

	std::filesystem::path Levels() const
	{
		return m_root / "levels";
	}

	std::filesystem::path AudioSettingsFile() const
	{
		return m_root / "settings" / "audio.toml";
	}
};
