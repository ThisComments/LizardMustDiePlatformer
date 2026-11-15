#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "Resources/ProjectPaths.h"

class AudioManager
{
private:
	struct SoundEffect
	{
		sf::SoundBuffer buffer;
		std::vector<sf::Sound> voices;
		std::size_t nextVoice = 0;
		float baseVolume = 100.f;

		SoundEffect(const std::filesystem::path& file, std::size_t voiceCount, float volume);
		void Play(float volumeScale);
		void SetVolumeScale(float scale);
	};

	struct MusicTrack
	{
		std::string id;
		std::filesystem::path file;
		std::string title;
		std::string artist;
	};

	std::map<std::string, SoundEffect, std::less<>> m_effects;
	sf::Music m_music;
	std::vector<MusicTrack> m_playlist;
	std::vector<std::size_t> m_playlistOrder;
	std::size_t m_playlistPosition = 0;

	float m_masterVolume = 100.f;
	float m_musicVolume = 100.f;
	float m_effectsVolume = 100.f;
	bool m_repeatPlaylist = true;
	bool m_shuffle = false;
	bool m_hasCurrentTrack = false;

	void LoadSettings(const ProjectPaths& paths);
	void ApplyVolumes();
	void BuildPlaylistOrder();
	bool StartTrack(std::size_t position);
	void PlayNextTrack();

public:
	AudioManager(const ProjectPaths& paths);

	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	void PlaySfx(const std::string& name);

	void PlayMusic();
	void PauseMusic();
	void ResumeMusic();
	void StopMusic();
	bool PlayTrack(const std::string& id);

	void SetMasterVolume(float volume);
	void SetMusicVolume(float volume);
	void SetEffectsVolume(float volume);

	const std::string& GetCurrentTrackTitle() const;
	const std::string& GetCurrentArtist() const;

	void Update();
};
