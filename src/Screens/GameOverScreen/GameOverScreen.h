#pragma once

#include <cstdint>

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>

#include "Audio/AudioManager.h"
#include "Screens/Screen.h"
#include "UI/Menu/Menu.h"
#include "UI/Overlay/ScreenOverlay.h"

enum class GameOverReason : std::uint8_t
{
	Victory,
	Defeat
};

class GameOverScreen final : public Screen
{
private:
	const sf::Font& m_font;
	sf::Vector2f m_screenSize;
	ScreenOverlay m_overlay;
	AudioManager& m_audio;
	Menu m_menu;
	GameOverReason m_reason;

public:
	GameOverScreen(const sf::Font& font, sf::Vector2f screenSize, GameOverReason reason, AudioManager& audio);

	ScreenCommand HandleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	ScreenCommand Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) const override;
	void OnEnter() override;
	bool IsOverlay() const override;
};
