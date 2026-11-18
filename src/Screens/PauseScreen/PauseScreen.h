#pragma once

#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>

#include "Audio/AudioManager.h"
#include "Screens/Screen.h"
#include "UI/Menu/Menu.h"
#include "UI/Overlay/ScreenOverlay.h"

class PauseScreen final : public Screen
{
private:
	const sf::Font& m_font;
	sf::Vector2f m_screenSize;
	AudioManager& m_audio;
	ScreenOverlay m_overlay;
	Menu m_menu;
	std::string m_trackTitle;
	std::string m_trackArtist;

public:
	PauseScreen(const sf::Font& font, sf::Vector2f screenSize, AudioManager& audio);

	ScreenCommand HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	ScreenCommand Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) const override;
	void OnEnter() override;
	void OnExit() override;
	bool IsOverlay() const override;
};
