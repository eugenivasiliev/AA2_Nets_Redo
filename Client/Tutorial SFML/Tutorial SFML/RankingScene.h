#pragma once
#include "Scene.h"
#include <vector>
#include "Ranking.h"
#include "RankingStyle.h"
#include "ObjectShape.h"
#include "ObjectText.h"
#include "ObjectCircle.h"
#include "NetworkManager.h"
#include "Button.h"

#define RANKING_QUERY_TIME 1.0f

class RankingScene : public Scene
{
	sf::Font* font = nullptr;
	int rankingListLength = 0;
	sf::Clock rankingClock;

public:
	void enter(SharedMemory* _sharedMemory) override {
		sharedMemory = _sharedMemory;
		font = new sf::Font(FONT_PATH);

		std::vector<User> users;
		users.push_back({ "eauna", 69420, 0 });
		users.push_back({ "foo", 1, 1 });
		users.push_back({ "bar", 2, 2 });
		users.push_back({ "fizz", 3, 3 });

		// =========================
		// BACKGROUND
		// =========================
		sf::RectangleShape background;
		background.setSize({ WINDOW_WIDTH, WINDOW_HEIGHT });
		background.setPosition({ 0.f, 0.f });
		background.setFillColor(rankingBackgroundColor);
		objects.push_back(new ObjectShape(background));

		// =========================
		// SUBTLE GRID (3x3)
		// =========================
		float gridWidth = WINDOW_WIDTH * 0.50f;
		float gridHeight = WINDOW_HEIGHT * 0.60f;

		float startX = WINDOW_WIDTH * 0.5f - gridWidth / 2.f;
		float startY = WINDOW_HEIGHT * 0.5f - gridHeight / 2.f;

		for (int i = 1; i <= 2; i++) {
			sf::RectangleShape line;
			line.setSize({ 2.f, gridHeight });
			line.setPosition({ startX + (gridWidth / 3.f) * i, startY });
			line.setFillColor(rankingGridColor);
			objects.push_back(new ObjectShape(line));
		}

		for (int i = 1; i <= 2; i++) {
			sf::RectangleShape line;
			line.setSize({ gridWidth, 2.f });
			line.setPosition({ startX, startY + (gridHeight / 3.f) * i });
			line.setFillColor(rankingGridColor);
			objects.push_back(new ObjectShape(line));
		}

		// =========================
		// DECORATIVE X
		// =========================
		sf::RectangleShape xLine1;
		xLine1.setSize({ 120.f, 4.f });
		xLine1.setOrigin({ xLine1.getSize().x / 2.f, xLine1.getSize().y / 2.f });
		xLine1.setPosition({ startX + 90.f, startY + 90.f });
		xLine1.setFillColor(rankingDecorativeXColor);
		xLine1.setRotation(sf::degrees(45.f));
		objects.push_back(new ObjectShape(xLine1));

		sf::RectangleShape xLine2 = xLine1;
		xLine2.setRotation(sf::degrees(-45.f));
		objects.push_back(new ObjectShape(xLine2));

		// =========================
		// DECORATIVE O
		// =========================
		sf::CircleShape oShape;
		oShape.setRadius(40.f);
		oShape.setPosition({
			startX + gridWidth - 120.f,
			startY + gridHeight - 120.f
			});
		oShape.setFillColor(sf::Color::Transparent);
		oShape.setOutlineThickness(4.f);
		oShape.setOutlineColor(rankingDecorativeOColor);
		objects.push_back(new ObjectCircle(oShape));

		// =========================
		// PANEL SHADOW
		// =========================
		sf::RectangleShape panelShadow;
		panelShadow.setSize(rankingPanelSize);
		panelShadow.setPosition({
			WINDOW_WIDTH / 2.f - rankingPanelSize.x / 2.f + 8.f,
			WINDOW_HEIGHT / 2.f - rankingPanelSize.y / 2.f + 10.f
			});
		panelShadow.setFillColor(sf::Color(0, 0, 0, 100));
		objects.push_back(new ObjectShape(panelShadow));

		// =========================
		// PANEL
		// =========================
		sf::RectangleShape panel;
		panel.setSize(rankingPanelSize);
		panel.setPosition({
			WINDOW_WIDTH / 2.f - rankingPanelSize.x / 2.f,
			WINDOW_HEIGHT / 2.f - rankingPanelSize.y / 2.f
			});
		panel.setFillColor(rankingPanelColor);
		panel.setOutlineThickness(rankingPanelOutlineThickness);
		panel.setOutlineColor(rankingPanelOutlineColor);
		objects.push_back(new ObjectShape(panel));

		sf::RectangleShape accentLine;
		accentLine.setSize({ rankingPanelSize.x, 6.f });
		accentLine.setPosition(panel.getPosition());
		accentLine.setFillColor(rankingAccentXColor);
		objects.push_back(new ObjectShape(accentLine));

		// =========================
		// TITLE
		// =========================
		sf::Text title(*font, rankingTitleText);
		title.setCharacterSize(rankingTitleSize);
		title.setFillColor(rankingTitleColor);
		{
			sf::FloatRect bounds = title.getLocalBounds();
			title.setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				panel.getPosition().y + 26.f
				});
		}
		objects.push_back(new ObjectText(title));

		// =========================
		// SUBTITLE
		// =========================
		sf::Text subtitle(*font, rankingSubtitleText);
		subtitle.setCharacterSize(rankingSubtitleSize);
		subtitle.setFillColor(rankingSubtitleColor);
		{
			sf::FloatRect bounds = subtitle.getLocalBounds();
			subtitle.setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				panel.getPosition().y + 78.f
				});
		}
		objects.push_back(new ObjectText(subtitle));

		// =========================
		// FOOTER
		// =========================
		sf::Text footer(*font, rankingFooterText);
		footer.setCharacterSize(rankingFooterSize);
		footer.setFillColor(rankingFooterColor);
		{
			sf::FloatRect bounds = footer.getLocalBounds();
			footer.setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				panel.getPosition().y + rankingPanelSize.y - 40.f
				});
		}
		objects.push_back(new ObjectText(footer));

		// LOBBY BUTTON
		sf::RectangleShape lobbyRect;
		lobbyRect.setSize({ WINDOW_WIDTH * .1f, WINDOW_HEIGHT * 0.085f });
		lobbyRect.setPosition({
			WINDOW_WIDTH * .9f - lobbyRect.getSize().x / 2.f,
			WINDOW_HEIGHT * .8f
			});
		lobbyRect.setFillColor(sf::Color(88, 101, 242));

		sf::Text lobbyText(*font, "Lobby");
		lobbyText.setCharacterSize(28);
		lobbyText.setFillColor(sf::Color::White);

		Button* lobbyButton = new Button(lobbyRect, lobbyText, [this]() {
			nextScene = SceneOption::LOBBY;
			});
		objects.push_back(lobbyButton);

		NT->SendRanking();
		rankingClock.restart();
	}

	bool update(sf::RenderWindow& window) override {
		if (rankingClock.getElapsedTime().asSeconds() > RANKING_QUERY_TIME) {
			rankingClock.restart();
			NT->SendRanking();
		}

		// =========================
		// RANKING LIST
		// =========================
		for (int i = 0; i < rankingListLength; i++) {
			delete objects.back();
			objects.pop_back();
		}

		rankingListLength = NT->GetRankingUsers().size();

		for (int i = 0; i < rankingListLength; i++)
			objects.push_back(new RankingRect(NT->GetRankingUsers()[i], *font, i));

		return Scene::update(window);
	}
};