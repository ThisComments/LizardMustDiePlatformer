#include "UI/Menu/Menu.h"

#include <SFML/Window/Mouse.hpp>

#include "UI/ScreenLayout.h"
#include "UI/UiStyle.h"

namespace
{
	bool IsPreviousKey(const sf::Keyboard::Key key)
	{
		return key == sf::Keyboard::Key::Up || key == sf::Keyboard::Key::W;
	}

	bool IsNextKey(const sf::Keyboard::Key key)
	{
		return key == sf::Keyboard::Key::Down || key == sf::Keyboard::Key::S;
	}

	bool IsConfirmKey(const sf::Keyboard::Key key)
	{
		return key == sf::Keyboard::Key::Enter || key == sf::Keyboard::Key::Space;
	}

	sf::Vector2f ToUiCoordinates(const sf::Vector2i pixel, const sf::RenderWindow& window)
	{
		return window.mapPixelToCoords(pixel, window.getDefaultView());
	}
}

Menu::Menu(
	const sf::Font& font,
	const std::vector<std::string>& labels,
	const sf::Vector2f screenSize
)
	: 	m_selected(0)
{
	m_buttons.reserve(labels.size());
	for (std::size_t i = 0; i < labels.size(); ++i)
	{
		m_buttons.emplace_back(
			font,
			labels[i],
			ScreenLayout::ButtonPosition(screenSize, i),
			UiStyle::BUTTON_SIZE
		);
	}

	Reset();
}

std::optional<std::size_t> Menu::HandleEvent(
	const sf::Event& event,
	const sf::RenderWindow& window
)
{
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		return HandleKeyPressed(keyPressed->code);
	}

	if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
	{
		const std::optional<std::size_t> hit =
			FindButtonAt(ToUiCoordinates(mouseMoved->position, window));
		if (hit)
		{
			Select(*hit);
		}
		return std::nullopt;
	}

	if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mousePressed->button != sf::Mouse::Button::Left)
		{
			return std::nullopt;
		}

		ClearPressed();
		const std::optional<std::size_t> hit =
			FindButtonAt(ToUiCoordinates(mousePressed->position, window));
		if (hit)
		{
			Select(*hit);
			m_pressed = hit;
			m_buttons[*hit].SetPressed(true);
		}
		return std::nullopt;
	}

	if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (mouseReleased->button != sf::Mouse::Button::Left)
		{
			return std::nullopt;
		}

		return HandleMouseReleased(ToUiCoordinates(mouseReleased->position, window));
	}

	return std::nullopt;
}

void Menu::Reset()
{
	ClearPressed();
	m_selected = 0;
	for (std::size_t i = 0; i < m_buttons.size(); ++i)
	{
		m_buttons[i].SetSelected(i == m_selected);
	}
}

void Menu::Render(sf::RenderTarget& target) const
{
	target.setView(target.getDefaultView());
	for (const Button& button : m_buttons)
	{
		button.Render(target);
	}
}

void Menu::Select(const std::size_t index)
{
	if (index == m_selected)
	{
		return;
	}

	m_buttons[m_selected].SetSelected(false);
	m_selected = index;
	m_buttons[m_selected].SetSelected(true);
}

void Menu::ClearPressed()
{
	if (m_pressed)
	{
		m_buttons[*m_pressed].SetPressed(false);
		m_pressed.reset();
	}
}

std::optional<std::size_t> Menu::FindButtonAt(const sf::Vector2f point) const
{
	for (std::size_t i = 0; i < m_buttons.size(); ++i)
	{
		if (m_buttons[i].Contains(point))
		{
			return i;
		}
	}
	return std::nullopt;
}

std::optional<std::size_t> Menu::HandleKeyPressed(const sf::Keyboard::Key key)
{
	if (m_buttons.empty())
	{
		return std::nullopt;
	}

	if (IsPreviousKey(key))
	{
		Select((m_selected + m_buttons.size() - 1) % m_buttons.size());
	}
	else if (IsNextKey(key))
	{
		Select((m_selected + 1) % m_buttons.size());
	}
	else if (IsConfirmKey(key))
	{
		return m_selected;
	}

	return std::nullopt;
}

std::optional<std::size_t> Menu::HandleMouseReleased(const sf::Vector2f point)
{
	const std::optional<std::size_t> pressed = m_pressed;
	ClearPressed();

	const std::optional<std::size_t> hit = FindButtonAt(point);
	if (hit && pressed && *hit == *pressed)
	{
		return hit;
	}
	return std::nullopt;
}
