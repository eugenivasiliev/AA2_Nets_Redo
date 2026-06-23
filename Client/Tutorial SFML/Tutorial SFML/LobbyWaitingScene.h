#pragma once
#include "Scene.h"
#include "config.h"
#include "NetworkManager.h"
#include "LobbyManager.h"
#include "ObjectShape.h"
#include "ObjectText.h"
#include "ObjectCircle.h"
#include "WaitingRoomStyle.h"
#include "DynamicTextObject.h"
#include <string>
#include "TimeManager.h"

class LobbyWaitingScene : public Scene
{
	sf::Font* font = nullptr;

	std::string id = "";
	int playerCount = 1;

	sf::Text* roomCodeValue = nullptr;
	sf::Text* playerCountValue = nullptr;

	bool sendedStartGamePacket = false;

	float startGameWaitTime = 0.0f;

public:
	void enter(SharedMemory* _sharedMemory) override {
		sharedMemory = _sharedMemory;
		font = new sf::Font(FONT_PATH);

		id = LM->GetRoomId();
		if (id.empty())
			id = "----";

		playerCount = LM->GetUpdatedPlayerCount();
		if (playerCount <= 0)
			playerCount = 1;

		sf::RectangleShape background;
		background.setSize({ WINDOW_WIDTH, WINDOW_HEIGHT });
		background.setPosition({ 0.f, 0.f });
		background.setFillColor(waitingBackgroundColor);
		objects.push_back(new ObjectShape(background));

		float gridWidth = WINDOW_WIDTH * 0.50f;
		float gridHeight = WINDOW_HEIGHT * 0.50f;

		float startX = WINDOW_WIDTH * 0.5f - gridWidth / 2.f;
		float startY = WINDOW_HEIGHT * 0.5f - gridHeight / 2.f;

		for (int i = 1; i <= 2; i++) {
			sf::RectangleShape line;
			line.setSize({ 2.f, gridHeight });
			line.setPosition({ startX + (gridWidth / 3.f) * i, startY });
			line.setFillColor(waitingGridColor);
			objects.push_back(new ObjectShape(line));
		}

		for (int i = 1; i <= 2; i++) {
			sf::RectangleShape line;
			line.setSize({ gridWidth, 2.f });
			line.setPosition({ startX, startY + (gridHeight / 3.f) * i });
			line.setFillColor(waitingGridColor);
			objects.push_back(new ObjectShape(line));
		}

		sf::RectangleShape xLine1;
		xLine1.setSize({ 120.f, 4.f });
		xLine1.setOrigin({ xLine1.getSize().x / 2.f, xLine1.getSize().y / 2.f });
		xLine1.setPosition({ startX + 90.f, startY + 90.f });
		xLine1.setFillColor(waitingDecorativeXColor);
		xLine1.setRotation(sf::degrees(45.f));
		objects.push_back(new ObjectShape(xLine1));

		sf::RectangleShape xLine2 = xLine1;
		xLine2.setRotation(sf::degrees(-45.f));
		objects.push_back(new ObjectShape(xLine2));

		sf::CircleShape oShape;
		oShape.setRadius(40.f);
		oShape.setPosition({
			startX + gridWidth - 120.f,
			startY + gridHeight - 120.f
			});
		oShape.setFillColor(sf::Color::Transparent);
		oShape.setOutlineThickness(4.f);
		oShape.setOutlineColor(waitingDecorativeOColor);
		objects.push_back(new ObjectCircle(oShape));

		sf::RectangleShape panelShadow;
		panelShadow.setSize(waitingPanelSize);
		panelShadow.setPosition({
			WINDOW_WIDTH / 2.f - waitingPanelSize.x / 2.f + 8.f,
			WINDOW_HEIGHT / 2.f - waitingPanelSize.y / 2.f + 10.f
			});
		panelShadow.setFillColor(sf::Color(0, 0, 0, 100));
		objects.push_back(new ObjectShape(panelShadow));

		sf::RectangleShape panel;
		panel.setSize(waitingPanelSize);
		panel.setPosition({
			WINDOW_WIDTH / 2.f - waitingPanelSize.x / 2.f,
			WINDOW_HEIGHT / 2.f - waitingPanelSize.y / 2.f
			});
		panel.setFillColor(waitingPanelColor);
		panel.setOutlineThickness(waitingPanelOutlineThickness);
		panel.setOutlineColor(waitingPanelOutlineColor);
		objects.push_back(new ObjectShape(panel));

		sf::RectangleShape accentLine;
		accentLine.setSize({ waitingPanelSize.x, 6.f });
		accentLine.setPosition(panel.getPosition());
		accentLine.setFillColor(waitingAccentXColor);
		objects.push_back(new ObjectShape(accentLine));

		sf::Text title(*font, waitingTitleText);
		title.setCharacterSize(waitingTitleSize);
		title.setFillColor(waitingTitleColor);
		{
			sf::FloatRect bounds = title.getLocalBounds();
			title.setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				panel.getPosition().y + 26.f
				});
		}
		objects.push_back(new ObjectText(title));

		sf::Text subtitle(*font, waitingSubtitleText);
		subtitle.setCharacterSize(waitingSubtitleSize);
		subtitle.setFillColor(waitingSubtitleColor);
		{
			sf::FloatRect bounds = subtitle.getLocalBounds();
			subtitle.setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				panel.getPosition().y + 78.f
				});
		}
		objects.push_back(new ObjectText(subtitle));

		sf::Text roomCodeLabel(*font, "Room Code");
		roomCodeLabel.setCharacterSize(waitingRoomCodeLabelSize);
		roomCodeLabel.setFillColor(waitingLabelColor);
		roomCodeLabel.setPosition({
			WINDOW_WIDTH / 2.f - waitingRoomCodeBoxSize.x / 2.f,
			WINDOW_HEIGHT * 0.34f
			});
		objects.push_back(new ObjectText(roomCodeLabel));

		sf::RectangleShape roomCodeBox;
		roomCodeBox.setSize(waitingRoomCodeBoxSize);
		roomCodeBox.setPosition({
			WINDOW_WIDTH / 2.f - waitingRoomCodeBoxSize.x / 2.f,
			WINDOW_HEIGHT * 0.38f
			});
		roomCodeBox.setFillColor(waitingRoomCodeBoxColor);
		roomCodeBox.setOutlineThickness(2.f);
		roomCodeBox.setOutlineColor(waitingRoomCodeOutlineColor);
		objects.push_back(new ObjectShape(roomCodeBox));

		roomCodeValue = new sf::Text(*font, id);
		roomCodeValue->setCharacterSize(waitingRoomCodeSize);
		roomCodeValue->setFillColor(waitingValueColor);
		{
			sf::FloatRect bounds = roomCodeValue->getLocalBounds();
			roomCodeValue->setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				roomCodeBox.getPosition().y + (roomCodeBox.getSize().y - bounds.size.y) / 2.f - bounds.position.y - 2.f
				});
		}
		objects.push_back(new DynamicTextObject(roomCodeValue));

		sf::Text playerCountLabel(*font, "Players");
		playerCountLabel.setCharacterSize(waitingPlayerCountLabelSize);
		playerCountLabel.setFillColor(waitingLabelColor);
		playerCountLabel.setPosition({
			WINDOW_WIDTH / 2.f - waitingPlayerCountBoxSize.x / 2.f,
			WINDOW_HEIGHT * 0.54f
			});
		objects.push_back(new ObjectText(playerCountLabel));

		sf::RectangleShape playerCountBox;
		playerCountBox.setSize(waitingPlayerCountBoxSize);
		playerCountBox.setPosition({
			WINDOW_WIDTH / 2.f - waitingPlayerCountBoxSize.x / 2.f,
			WINDOW_HEIGHT * 0.58f
			});
		playerCountBox.setFillColor(waitingPlayerCountBoxColor);
		playerCountBox.setOutlineThickness(2.f);
		playerCountBox.setOutlineColor(waitingPlayerCountOutlineColor);
		objects.push_back(new ObjectShape(playerCountBox));

		playerCountValue = new sf::Text(*font, std::to_string(playerCount) + "/4");
		playerCountValue->setCharacterSize(waitingPlayerCountSize);
		playerCountValue->setFillColor(waitingValueColor);
		{
			sf::FloatRect bounds = playerCountValue->getLocalBounds();
			playerCountValue->setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				playerCountBox.getPosition().y + (playerCountBox.getSize().y - bounds.size.y) / 2.f - bounds.position.y - 4.f
				});
		}
		objects.push_back(new DynamicTextObject(playerCountValue));

		sf::Text footer(*font, waitingFooterText);
		footer.setCharacterSize(waitingFooterSize);
		footer.setFillColor(waitingFooterColor);
		{
			sf::FloatRect bounds = footer.getLocalBounds();
			footer.setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				WINDOW_HEIGHT * 0.80f
				});
		}
		objects.push_back(new ObjectText(footer));
	}

	bool update(sf::RenderWindow& window) override
	{
		playerCount = LM->GetUpdatedPlayerCount();

		playerCountValue->setString(std::to_string(playerCount) + "/4");

		if (playerCount >= PLAYER_COUNT) {

			NT->ConnectP2P();

			if (NT->GetConnectedClientCount() >= PLAYER_COUNT - 1) {
				nextScene = SceneOption::GAME;
				return Scene::update(window);
			}

			startGameWaitTime += TIME->dt;
		}

		if (startGameWaitTime >= START_GAME_MAX_WAIT_TIME) {
			std::cerr << "P2P connection timeout";
			return false;
		}

		return Scene::update(window);
	}
};