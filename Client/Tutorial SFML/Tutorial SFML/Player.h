#pragma once
#include "GameStyle.h"
#include "User.h"
#include <SFML/Graphics.hpp>

struct PlayerRect
{
	sf::RectangleShape rectangle;
	sf::RectangleShape accentBar;
	sf::Text nick;
	bool isCurrent = false;

	PlayerRect() : nick(nullFont) {}

	PlayerRect(User user, sf::Font& font) : nick(font) {
		rectangle.setPosition({
			WINDOW_WIDTH * playersAnchorX,
			WINDOW_HEIGHT * playersAnchorY + user.userIndex * (playersHeight + playersSeparation)
			});
		rectangle.setSize({ playersWidth, playersHeight });
		rectangle.setFillColor(playersBackground);
		rectangle.setOutlineThickness(2.f);
		rectangle.setOutlineColor(sf::Color(255, 255, 255, 18));

		accentBar.setPosition(rectangle.getPosition());
		accentBar.setSize({ 8.f, playersHeight });
		accentBar.setFillColor(gameAccentOColor);

		nick.setString(user.nickname);
		nick.setCharacterSize(playersCharacterSize);
		nick.setStyle(playersTextStyle);
		nick.setFillColor(sf::Color(240, 245, 255));

		sf::FloatRect textBounds = nick.getLocalBounds();
		nick.setPosition({
			rectangle.getPosition().x + 20.f,
			rectangle.getPosition().y + (rectangle.getSize().y - textBounds.size.y) / 2.f - textBounds.position.y - 2.f
			});
	}

	void render(sf::RenderWindow& window) {
		rectangle.setFillColor(isCurrent ? activePlayersBackground : playersBackground);
		accentBar.setPosition(rectangle.getPosition());
		accentBar.setFillColor(isCurrent ? gameAccentXColor : gameAccentOColor);

		if (isCurrent) {
			rectangle.setOutlineColor(sf::Color(gameAccentXColor.r, gameAccentXColor.g, gameAccentXColor.b, 160));
		}
		else {
			rectangle.setOutlineColor(sf::Color(255, 255, 255, 18));
		}

		window.draw(rectangle);
		window.draw(accentBar);
		window.draw(nick);
	}
};