#include "Audio/AudioManager.h"

#include <algorithm>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include <stdexcept>
#include <system_error>
#include <string>
#include <utility>

#include <toml++/toml.hpp>

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tstring.h>

namespace
{
	float ClampVolume(const float volume)
	{
		return std::clamp(volume, 0.f, 100.f);
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

	std::string RequireString(const toml::table& table, const std::string& key, const std::string& where)
	{
		const auto value = RequireNode(table, key, where).value<std::string>();
		if (!value || value->empty())
		{
			throw std::runtime_error(where + "." + key + " must be a non-empty string");
		}
		return *value;
	}

	float ReadNumber(const toml::node& node, const std::string& where)
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

	float ReadVolume(const toml::table& table, const std::string& key, const std::string& where)
	{
		const toml::node* node = table.get(key);
		if (node == nullptr)
		{
			return 100.f;
		}

		const float volume = ReadNumber(*node, where + "." + key);
		if (volume < 0.f || volume > 100.f)
		{
			throw std::runtime_error(where + "." + key + " must be within 0..100");
		}
		return volume;
	}

	bool ReadBool(const toml::table& table, const std::string& key, const bool fallback, const std::string& where)
	{
		const toml::node* node = table.get(key);
		if (node == nullptr)
		{
			return fallback;
		}

		const auto value = node->value<bool>();
		if (!value)
		{
			throw std::runtime_error(where + "." + key + " must be true or false");
		}
		return *value;
	}

	std::size_t ReadVoiceCount(const toml::table& table, const std::string& where)
	{
		const toml::node* node = table.get("voices");
		if (node == nullptr)
		{
			return 1;
		}

		const auto value = node->value<std::int64_t>();
		if (!value || *value < 1 || *value > 16)
		{
			throw std::runtime_error(where + ".voices must be an integer within 1..16");
		}
		return static_cast<std::size_t>(*value);
	}

	std::filesystem::path ToPath(const std::string& value)
	{
		return std::filesystem::u8path(value);
	}

	bool IsRegularFile(const std::filesystem::path& path)
	{
		std::error_code error;
		return std::filesystem::is_regular_file(path, error);
	}

	std::pair<std::string, std::string> ReadTrackTags(const std::filesystem::path& file)
	{
		const TagLib::FileRef reference(file.c_str(), false);
		if (reference.isNull() || reference.tag() == nullptr)
		{
			return {};
		}

		const TagLib::Tag* tag = reference.tag();
		return {tag->title().to8Bit(true), tag->artist().to8Bit(true)};
	}
}

AudioManager::SoundEffect::SoundEffect(
	const std::filesystem::path& file,
	const std::size_t voiceCount,
	const float volume
)
	: 	baseVolume(volume)
{
	if (!buffer.loadFromFile(file))
	{
		throw std::runtime_error("Cannot load sound effect: " + file.string());
	}

	voices.reserve(voiceCount);
	for (std::size_t i = 0; i < voiceCount; ++i)
	{
		voices.emplace_back(buffer);
	}
}

void AudioManager::SoundEffect::Play(const float volumeScale)
{
	for (sf::Sound& voice : voices)
	{
		if (voice.getStatus() != sf::Sound::Status::Playing)
		{
			voice.setVolume(baseVolume * volumeScale);
			voice.play();
			return;
		}
	}

	if (!voices.empty())
	{
		voices[nextVoice].setVolume(baseVolume * volumeScale);
		voices[nextVoice].play();
		nextVoice = (nextVoice + 1) % voices.size();
	}
}

void AudioManager::SoundEffect::SetVolumeScale(const float scale)
{
	for (sf::Sound& voice : voices)
	{
		voice.setVolume(baseVolume * scale);
	}
}

AudioManager::AudioManager(const ProjectPaths& paths)
{
	try
	{
		LoadSettings(paths);
	}
	catch (const std::exception& error)
	{
		std::cerr << "Audio disabled: " << error.what() << '\n';
	}

	ApplyVolumes();
}

void AudioManager::LoadSettings(const ProjectPaths& paths)
{
	const std::filesystem::path settingsPath = paths.AudioSettingsFile();
	toml::table root;
	try
	{
		root = toml::parse_file(settingsPath.string());
	}
	catch (const toml::parse_error& error)
	{
		throw std::runtime_error("Invalid audio TOML " + settingsPath.string() + ": " + error.what());
	}

	const toml::table& volume = RequireTable(root, "volume", "audio settings");
	m_masterVolume = ReadVolume(volume, "master", "volume");
	m_musicVolume = ReadVolume(volume, "music", "volume");
	m_effectsVolume = ReadVolume(volume, "effects", "volume");

	const toml::table& music = RequireTable(root, "music", "audio settings");
	m_repeatPlaylist = ReadBool(music, "repeat_playlist", true, "music");
	m_shuffle = ReadBool(music, "shuffle", false, "music");

	const toml::array* playlist = RequireNode(music, "playlist", "music").as_array();
	if (playlist == nullptr)
	{
		throw std::runtime_error("music.playlist must be an array of tables");
	}

	std::set<std::string> trackIds;
	for (const toml::node& item : *playlist)
	{
		const toml::table* track = item.as_table();
		if (track == nullptr)
		{
			throw std::runtime_error("Every music.playlist item must be a table");
		}

		const std::string id = RequireString(*track, "id", "music.playlist[]");
		if (!trackIds.insert(id).second)
		{
			throw std::runtime_error("music.playlist contains duplicate id '" + id + "'");
		}

		const std::filesystem::path file = paths.Resolve(ToPath(
			RequireString(*track, "file", "music.playlist[]")
		));
		if (!IsRegularFile(file))
		{
			std::cerr << "Warning: music track not found, skipping: " << file.string() << '\n';
			continue;
		}

		const auto [title, artist] = ReadTrackTags(file);
		m_playlist.push_back({
			id,
			file,
			title.empty() ? file.stem().u8string() : title,
			artist
		});
	}

	const toml::table& sounds = RequireTable(root, "sounds", "audio settings");
	for (const auto& [key, node] : sounds)
	{
		const std::string name(key.str());
		const toml::table* sound = node.as_table();
		if (sound == nullptr)
		{
			throw std::runtime_error("sounds." + name + " must be a table");
		}

		const std::filesystem::path file = paths.Resolve(ToPath(RequireString(*sound, "file", "sounds." + name)));
		const float volumeValue = ReadVolume(*sound, "volume", "sounds." + name);
		const std::size_t voiceCount = ReadVoiceCount(*sound, "sounds." + name);
		if (!IsRegularFile(file))
		{
			std::cerr << "Warning: sound effect not found, skipping '" << name << "': " << file.string() << '\n';
			continue;
		}

		try
		{
			m_effects.try_emplace(name, file, voiceCount, volumeValue);
		}
		catch (const std::exception& error)
		{
			std::cerr << "Warning: sound effect '" << name << "' skipped: " << error.what() << '\n';
		}
	}

	BuildPlaylistOrder();
}

void AudioManager::PlaySfx(const std::string& name)
{
	const auto it = m_effects.find(name);
	if (it == m_effects.end())
	{
		std::cerr << "Warning: sound effect '" << name << "' is not loaded\n";
		return;
	}

	it->second.Play(m_effectsVolume / 100.f * m_masterVolume / 100.f);
}

void AudioManager::PlayMusic()
{
	if (m_hasCurrentTrack || m_playlistOrder.empty())
	{
		return;
	}

	if (m_shuffle)
	{
		BuildPlaylistOrder();
	}
	StartTrack(0);
}

void AudioManager::PauseMusic()
{
	if (m_music.getStatus() == sf::SoundSource::Status::Playing)
	{
		m_music.pause();
	}
}

void AudioManager::ResumeMusic()
{
	if (m_music.getStatus() == sf::SoundSource::Status::Paused)
	{
		m_music.play();
	}
}

void AudioManager::StopMusic()
{
	m_music.stop();
	m_hasCurrentTrack = false;
}

bool AudioManager::PlayTrack(const std::string& id)
{
	const auto track = std::find_if(m_playlist.begin(), m_playlist.end(),
		[&id](const MusicTrack& candidate) { return candidate.id == id; });
	if (track == m_playlist.end())
	{
		std::cerr << "Warning: music track '" << id << "' is not loaded\n";
		return false;
	}

	const std::size_t trackIndex = static_cast<std::size_t>(track - m_playlist.begin());
	const auto position = std::find(m_playlistOrder.begin(), m_playlistOrder.end(), trackIndex);
	if (position == m_playlistOrder.end())
	{
		return false;
	}

	return StartTrack(static_cast<std::size_t>(position - m_playlistOrder.begin()));
}

void AudioManager::SetMasterVolume(const float volume)
{
	m_masterVolume = ClampVolume(volume);
	ApplyVolumes();
}

void AudioManager::SetMusicVolume(const float volume)
{
	m_musicVolume = ClampVolume(volume);
	ApplyVolumes();
}

void AudioManager::SetEffectsVolume(const float volume)
{
	m_effectsVolume = ClampVolume(volume);
	ApplyVolumes();
}

const std::string& AudioManager::GetCurrentTrackTitle() const
{
	if (!m_hasCurrentTrack || m_playlistOrder.empty())
	{
		return "";
	}
	return m_playlist[m_playlistOrder[m_playlistPosition]].title;
}

const std::string& AudioManager::GetCurrentArtist() const
{
	if (!m_hasCurrentTrack || m_playlistOrder.empty())
	{
		return "";
	}
	return m_playlist[m_playlistOrder[m_playlistPosition]].artist;
}

void AudioManager::Update()
{
	if (m_hasCurrentTrack && m_music.getStatus() == sf::SoundSource::Status::Stopped)
	{
		PlayNextTrack();
	}
}

void AudioManager::ApplyVolumes()
{
	const float masterScale = m_masterVolume / 100.f;
	m_music.setVolume(m_musicVolume * masterScale);
	const float effectsScale = m_effectsVolume / 100.f * masterScale;
	for (auto& [name, effect] : m_effects)
	{
		effect.SetVolumeScale(effectsScale);
	}
}

void AudioManager::BuildPlaylistOrder()
{
	m_playlistOrder.resize(m_playlist.size());
	std::iota(m_playlistOrder.begin(), m_playlistOrder.end(), std::size_t{0});
	if (m_shuffle)
	{
		std::mt19937 generator(std::random_device{}());
		std::shuffle(m_playlistOrder.begin(), m_playlistOrder.end(), generator);
	}
}

bool AudioManager::StartTrack(const std::size_t position)
{
	if (m_playlistOrder.empty())
	{
		return false;
	}

	for (std::size_t attempt = 0; attempt < m_playlistOrder.size(); ++attempt)
	{
		const std::size_t candidate = (position + attempt) % m_playlistOrder.size();
		const MusicTrack& track = m_playlist[m_playlistOrder[candidate]];
		m_music.stop();
		if (!m_music.openFromFile(track.file))
		{
			std::cerr << "Warning: cannot open music track: " << track.file.string() << '\n';
			continue;
		}

		m_playlistPosition = candidate;
		m_music.setLooping(false);
		m_music.setVolume(m_musicVolume * m_masterVolume / 100.f);
		m_music.play();
		m_hasCurrentTrack = true;
		return true;
	}

	StopMusic();
	return false;
}

void AudioManager::PlayNextTrack()
{
	std::size_t next = m_playlistPosition + 1;
	if (next >= m_playlistOrder.size())
	{
		if (!m_repeatPlaylist)
		{
			StopMusic();
			return;
		}

		if (m_shuffle)
		{
			BuildPlaylistOrder();
		}
		next = 0;
	}

	StartTrack(next);
}
