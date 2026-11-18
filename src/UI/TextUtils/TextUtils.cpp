#include "UI/TextUtils/TextUtils.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include "UI/UiStyle.h"

sf::String ToSfString(const std::string& utf8Text)
{
	return sf::String::fromUtf8(utf8Text.begin(), utf8Text.end());
}

void CenterTextAt(sf::Text& text, const sf::Vector2f center)
{
	const sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin({
		bounds.position.x + bounds.size.x / 2.f,
		bounds.position.y + bounds.size.y / 2.f
	});
	text.setPosition(center);
}

void AlignTextLeftCenter(sf::Text& text, const sf::Vector2f leftCenter)
{
	const sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin({
		bounds.position.x,
		bounds.position.y + bounds.size.y / 2.f
	});
	text.setPosition(leftCenter);
}

void DrawCenteredText(
	sf::RenderTarget& target,
	const sf::Font& font,
	const std::string& content,
	const unsigned characterSize,
	const sf::Vector2f center,
	const sf::Color color
)
{
	DrawShadowedCenteredText(
		target,
		font,
		content,
		characterSize,
		center,
		color,
		UiStyle::TEXT_SHADOW_COLOR
	);
}

void DrawShadowedCenteredText(
	sf::RenderTarget& target,
	const sf::Font& font,
	const std::string& content,
	const unsigned characterSize,
	const sf::Vector2f center,
	const sf::Color color,
	const sf::Color shadowColor
)
{
	sf::Text text(font, ToSfString(content), characterSize);
	text.setFillColor(color);
	CenterTextAt(text, center);
	DrawTextWithShadow(
		target,
		text,
		shadowColor,
		{ UiStyle::TEXT_SHADOW_OFFSET, UiStyle::TEXT_SHADOW_OFFSET }
	);
}

void DrawTextWithShadow(
	sf::RenderTarget& target,
	const sf::Text& text,
	const sf::Color shadowColor,
	const sf::Vector2f shadowOffset
)
{
	sf::Text shadow = text;
	shadow.setFillColor(shadowColor);
	shadow.setPosition(text.getPosition() + shadowOffset);
	target.draw(shadow);
	target.draw(text);
}

void DrawCard(
	sf::RenderTarget& target,
	const sf::Vector2f position,
	const sf::Vector2f size
)
{
	sf::RectangleShape dropShadow(size);
	dropShadow.setPosition(position + UiStyle::CARD_SHADOW_OFFSET);
	dropShadow.setFillColor(UiStyle::CARD_DROP_SHADOW_COLOR);
	target.draw(dropShadow);

	sf::RectangleShape body(size);
	body.setPosition(position);
	body.setFillColor(UiStyle::CARD_FILL_COLOR);
	body.setOutlineThickness(-UiStyle::CARD_OUTLINE_THICKNESS);
	body.setOutlineColor(UiStyle::CARD_OUTLINE_COLOR);
	target.draw(body);

	const float border = UiStyle::CARD_OUTLINE_THICKNESS;
	const float bevel = UiStyle::CARD_BEVEL_SIZE;

	sf::RectangleShape topBevel({ size.x - border * 2.f, bevel });
	topBevel.setPosition({ position.x + border, position.y + border });
	topBevel.setFillColor(UiStyle::CARD_HIGHLIGHT_BEVEL);
	target.draw(topBevel);

	sf::RectangleShape leftBevel({ bevel, size.y - border * 2.f });
	leftBevel.setPosition({ position.x + border, position.y + border });
	leftBevel.setFillColor(UiStyle::CARD_HIGHLIGHT_BEVEL);
	target.draw(leftBevel);

	sf::RectangleShape bottomBevel({ size.x - border * 2.f, bevel });
	bottomBevel.setPosition({ position.x + border, position.y + size.y - border - bevel });
	bottomBevel.setFillColor(UiStyle::CARD_SHADOW_BEVEL);
	target.draw(bottomBevel);

	sf::RectangleShape rightBevel({ bevel, size.y - border * 2.f });
	rightBevel.setPosition({ position.x + size.x - border - bevel, position.y + border });
	rightBevel.setFillColor(UiStyle::CARD_SHADOW_BEVEL);
	target.draw(rightBevel);
}

void DrawInsetPanel(
	sf::RenderTarget& target,
	const sf::Vector2f position,
	const sf::Vector2f size,
	const sf::Color fillColor,
	const sf::Color outlineColor
)
{
	sf::RectangleShape panel(size);
	panel.setPosition(position);
	panel.setFillColor(fillColor);
	panel.setOutlineThickness(-UiStyle::PANEL_OUTLINE_THICKNESS);
	panel.setOutlineColor(outlineColor);
	target.draw(panel);

	const float border = UiStyle::PANEL_OUTLINE_THICKNESS;
	sf::RectangleShape innerShadow({ size.x - border * 2.f, border });
	innerShadow.setPosition({ position.x + border, position.y + border });
	innerShadow.setFillColor(sf::Color(0, 0, 0, 140));
	target.draw(innerShadow);
}

void DrawDivider(
	sf::RenderTarget& target,
	const sf::Vector2f center,
	const float width
)
{
	sf::RectangleShape line({ width, UiStyle::DIVIDER_THICKNESS });
	line.setPosition({ center.x - width / 2.f, center.y });
	line.setFillColor(UiStyle::DIVIDER_COLOR);
	target.draw(line);
}

void DrawPatternedBackground(sf::RenderTarget& target, const sf::Vector2f screenSize)
{
	sf::RectangleShape background(screenSize);
	background.setFillColor(UiStyle::SCREEN_BACKGROUND_COLOR);
	target.draw(background);

	for (float x = 0.f; x < screenSize.x; x += UiStyle::GRID_CELL_WIDTH)
	{
		sf::RectangleShape verticalLine({ UiStyle::GRID_LINE_THICKNESS, screenSize.y });
		verticalLine.setPosition({ x, 0.f });
		verticalLine.setFillColor(UiStyle::SCREEN_GRID_COLOR);
		target.draw(verticalLine);
	}

	for (float y = 0.f; y < screenSize.y; y += UiStyle::GRID_CELL_HEIGHT)
	{
		sf::RectangleShape horizontalLine({ screenSize.x, UiStyle::GRID_LINE_THICKNESS });
		horizontalLine.setPosition({ 0.f, y });
		horizontalLine.setFillColor(UiStyle::SCREEN_GRID_COLOR);
		target.draw(horizontalLine);
	}
}