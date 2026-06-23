#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "RankingStyle.h"
#include "User.h"
#include <string>

class RankingRect : public Object
{
	sf::RectangleShape rectangle;
	sf::RectangleShape accentBar;
	sf::Text rankText;
	sf::Text nickText;
	sf::Text scoreText;

public:
	RankingRect() : rankText(emptyFont), nickText(emptyFont), scoreText(emptyFont) {}

	RankingRect(User user, sf::Font& font, int index)
		: rankText(font), nickText(font), scoreText(font)
	{
		const float posX = WINDOW_WIDTH * rankingsAnchorX;
		const float posY = WINDOW_HEIGHT * rankingsAnchorY + index * (rankingsHeight + rankingsSeparation);

		rectangle.setPosition({ posX, posY });
		rectangle.setSize({ rankingsWidth, rankingsHeight });
		rectangle.setFillColor(index < 3 ? rankingsTopThreeBackground : rankingsBackground);
		rectangle.setOutlineThickness(2.f);
		rectangle.setOutlineColor(rankingsOutlineColor);

		accentBar.setPosition(rectangle.getPosition());
		accentBar.setSize({ 8.f, rankingsHeight });

		if (index == 0)
			accentBar.setFillColor(sf::Color(255, 210, 90));
		else if (index == 1)
			accentBar.setFillColor(sf::Color(180, 190, 210));
		else if (index == 2)
			accentBar.setFillColor(sf::Color(205, 140, 95));
		else
			accentBar.setFillColor(rankingAccentXColor);

		// Rank number
		rankText.setString("#" + std::to_string(index + 1));
		rankText.setCharacterSize(rankingsCharacterSize - 4);
		rankText.setStyle(rankingsTextStyle);
		rankText.setFillColor(rankingsIndexColor);
		{
			sf::FloatRect bounds = rankText.getLocalBounds();
			rankText.setPosition({
				rectangle.getPosition().x + 22.f,
				rectangle.getPosition().y + (rectangle.getSize().y - bounds.size.y) / 2.f - bounds.position.y - 2.f
				});
		}

		// Nickname
		nickText.setString(user.nickname);
		nickText.setCharacterSize(rankingsCharacterSize);
		nickText.setStyle(rankingsTextStyle);
		nickText.setFillColor(rankingsTextColor);
		{
			sf::FloatRect bounds = nickText.getLocalBounds();
			nickText.setPosition({
				rectangle.getPosition().x + 85.f,
				rectangle.getPosition().y + (rectangle.getSize().y - bounds.size.y) / 2.f - bounds.position.y - 2.f
				});
		}

		// Score
		scoreText.setString(std::to_string(user.score));
		scoreText.setCharacterSize(rankingsCharacterSize);
		scoreText.setStyle(rankingsTextStyle);
		scoreText.setFillColor(rankingsTextColor);
		{
			sf::FloatRect bounds = scoreText.getLocalBounds();
			scoreText.setPosition({
				rectangle.getPosition().x + rectangle.getSize().x - bounds.size.x - bounds.position.x - 24.f,
				rectangle.getPosition().y + (rectangle.getSize().y - bounds.size.y) / 2.f - bounds.position.y - 2.f
				});
		}
	}

	void render(sf::RenderWindow& window) override {
		window.draw(rectangle);
		window.draw(accentBar);
		window.draw(rankText);
		window.draw(nickText);
		window.draw(scoreText);
	}
};