#pragma once
#include "Scene.h"
#include "config.h"
#include "InputField.h"
#include "Button.h"
#include "NetworkManager.h"
#include "LobbyManager.h"
#include "LobbyStyle.h"
#include "ObjectShape.h"
#include "ObjectText.h"
#include "ObjectCircle.h"
#include "PlaceholderText.h"
#include "StatusText.h"

class LobbyScene : public Scene
{
	sf::Font* font = nullptr;
	InputField* lobbyField = nullptr;

	std::string statusMessage = "Enter a room code or create a new lobby";
	sf::Color statusColor = lobbyStatusNeutralColor;

public:
	void attemptCreateLobby() {
		if (!lobbyField)
			return;

		if (lobbyField->value.empty()) {
			statusMessage = "Enter a room code first";
			statusColor = lobbyStatusErrorColor;
			return;
		}

		NT->SendLobbyCreate(lobbyField->value);
		statusMessage = "Creating lobby...";
		statusColor = lobbyStatusNeutralColor;
	}

	void attemptJoinLobby() {
		if (!lobbyField)
			return;

		if (lobbyField->value.empty()) {
			statusMessage = "Enter a room code first";
			statusColor = lobbyStatusErrorColor;
			return;
		}

		NT->SendLobbyJoin(lobbyField->value);
		statusMessage = "Joining lobby...";
		statusColor = lobbyStatusNeutralColor;
	}

	void enter(SharedMemory* _sharedMemory) override {
		sharedMemory = _sharedMemory;
		font = new sf::Font(FONT_PATH);

		// =========================
		// BACKGROUND
		// =========================
		sf::RectangleShape background;
		background.setSize({ WINDOW_WIDTH, WINDOW_HEIGHT });
		background.setPosition({ 0.f, 0.f });
		background.setFillColor(lobbyBackgroundColor);
		objects.push_back(new ObjectShape(background));

		// =========================
		// SUBTLE GRID (3x3)
		// =========================
		float gridWidth = WINDOW_WIDTH * 0.5f;
		float gridHeight = WINDOW_HEIGHT * 0.5f;

		float startX = WINDOW_WIDTH * 0.5f - gridWidth / 2.f;
		float startY = WINDOW_HEIGHT * 0.5f - gridHeight / 2.f;

		for (int i = 1; i <= 2; i++) {
			sf::RectangleShape line;
			line.setSize({ 2.f, gridHeight });
			line.setPosition({ startX + (gridWidth / 3.f) * i, startY });
			line.setFillColor(lobbyGridColor);
			objects.push_back(new ObjectShape(line));
		}

		for (int i = 1; i <= 2; i++) {
			sf::RectangleShape line;
			line.setSize({ gridWidth, 2.f });
			line.setPosition({ startX, startY + (gridHeight / 3.f) * i });
			line.setFillColor(lobbyGridColor);
			objects.push_back(new ObjectShape(line));
		}

		// =========================
		// DECORATIVE X
		// =========================
		sf::RectangleShape xLine1;
		xLine1.setSize({ 120.f, 4.f });
		xLine1.setPosition({ startX + 40.f, startY + 40.f });
		xLine1.setFillColor(lobbyDecorativeXColor);
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
		oShape.setOutlineColor(lobbyDecorativeOColor);
		objects.push_back(new ObjectCircle(oShape));

		// =========================
		// PANEL SHADOW
		// =========================
		sf::RectangleShape panelShadow;
		panelShadow.setSize(lobbyPanelSize);
		panelShadow.setPosition({
			WINDOW_WIDTH / 2.f - lobbyPanelSize.x / 2.f + 8.f,
			WINDOW_HEIGHT / 2.f - lobbyPanelSize.y / 2.f + 10.f
			});
		panelShadow.setFillColor(sf::Color(0, 0, 0, 100));
		objects.push_back(new ObjectShape(panelShadow));

		// =========================
		// PANEL
		// =========================
		sf::RectangleShape panel;
		panel.setSize(lobbyPanelSize);
		panel.setPosition({
			WINDOW_WIDTH / 2.f - lobbyPanelSize.x / 2.f,
			WINDOW_HEIGHT / 2.f - lobbyPanelSize.y / 2.f
			});
		panel.setFillColor(lobbyPanelColor);
		panel.setOutlineThickness(lobbyPanelOutlineThickness);
		panel.setOutlineColor(lobbyPanelOutlineColor);
		objects.push_back(new ObjectShape(panel));

		// =========================
		// ACCENT LINE
		// =========================
		sf::RectangleShape accentLine;
		accentLine.setSize({ lobbyPanelSize.x, 6.f });
		accentLine.setPosition(panel.getPosition());
		accentLine.setFillColor(lobbyPanelAccentColor);
		objects.push_back(new ObjectShape(accentLine));

		// =========================
		// TITLE
		// =========================
		sf::Text title(*font, lobbyTitleText);
		title.setCharacterSize(lobbyTitleSize);
		title.setFillColor(lobbyTitleColor);
		{
			sf::FloatRect bounds = title.getLocalBounds();
			title.setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				WINDOW_HEIGHT * 0.14f
				});
		}
		objects.push_back(new ObjectText(title));

		// =========================
		// SUBTITLE
		// =========================
		sf::Text subtitle(*font, lobbySubtitleText);
		subtitle.setCharacterSize(lobbySubtitleSize);
		subtitle.setFillColor(lobbySubtitleColor);
		{
			sf::FloatRect bounds = subtitle.getLocalBounds();
			subtitle.setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				WINDOW_HEIGHT * 0.205f
				});
		}
		objects.push_back(new ObjectText(subtitle));

		// =========================
		// ROOM CODE INPUT
		// =========================
		sf::RectangleShape lobbyRect;
		lobbyRect.setSize(lobbyRectSize);
		lobbyRect.setPosition({
			WINDOW_WIDTH * lobbyRectAnchorX - lobbyRect.getSize().x / 2.f,
			WINDOW_HEIGHT * lobbyRectAnchorY
			});
		lobbyRect.setFillColor(lobbyRectColour);
		lobbyRect.setOutlineThickness(lobbyInputOutlineThickness);
		lobbyRect.setOutlineColor(lobbyInputIdleOutlineColor);

		sf::Text lobbyLabel(*font, "Room Code");
		lobbyLabel.setCharacterSize(lobbyInputLabelSize);
		lobbyLabel.setFillColor(lobbyInputLabelColor);
		lobbyLabel.setPosition({
			lobbyRect.getPosition().x,
			lobbyRect.getPosition().y - 28.f
			});
		objects.push_back(new ObjectText(lobbyLabel));

		lobbyField = new InputField(lobbyRect, sf::Text(*font), lobbyMaxChars);
		lobbyField->setTextCharacterSize(lobbyInputTextSize);
		lobbyField->setTextColor(lobbyInputTextColor);
		lobbyField->setIdleOutlineColor(lobbyInputIdleOutlineColor);
		lobbyField->setFocusOutlineColor(lobbyInputFocusOutlineColor);
		objects.push_back(lobbyField);

		sf::Text lobbyPlaceholder(*font, "Enter room code");
		lobbyPlaceholder.setCharacterSize(lobbyInputPlaceholderSize);
		lobbyPlaceholder.setFillColor(lobbyInputPlaceholderColor);
		lobbyPlaceholder.setPosition({
			lobbyRect.getPosition().x + 14.f,
			lobbyRect.getPosition().y + lobbyRect.getSize().y / 2.f - 15.f
			});
		objects.push_back(new PlaceholderText(lobbyPlaceholder, lobbyField));

		// =========================
		// CREATE BUTTON
		// =========================
		sf::RectangleShape createRect;
		createRect.setSize(createRectSize);
		createRect.setPosition({
			WINDOW_WIDTH * createRectAnchorX - createRect.getSize().x / 2.f,
			WINDOW_HEIGHT * createRectAnchorY
			});
		createRect.setFillColor(createRectColour);

		sf::Text createText(*font, createButtonLabel);
		createText.setCharacterSize(lobbyButtonTextSize);
		createText.setFillColor(sf::Color::White);

		Button* createButton = new Button(createRect, createText, [this]() {
			attemptCreateLobby();
			});
		objects.push_back(createButton);

		// =========================
		// JOIN BUTTON
		// =========================
		sf::RectangleShape joinRect;
		joinRect.setSize(joinRectSize);
		joinRect.setPosition({
			WINDOW_WIDTH * joinRectAnchorX - joinRect.getSize().x / 2.f,
			WINDOW_HEIGHT * joinRectAnchorY
			});
		joinRect.setFillColor(joinRectColour);

		sf::Text joinText(*font, joinButtonLabel);
		joinText.setCharacterSize(lobbyButtonTextSize);
		joinText.setFillColor(sf::Color::White);

		Button* joinButton = new Button(joinRect, joinText, [this]() {
			attemptJoinLobby();
			});
		objects.push_back(joinButton);

		// =========================
		// STATUS TEXT
		// =========================
		sf::Text statusBase(*font, "");
		statusBase.setCharacterSize(lobbyStatusTextSize);
		statusBase.setFillColor(lobbyStatusNeutralColor);
		statusBase.setPosition({ 0.f, WINDOW_HEIGHT * 0.84f });
		objects.push_back(new StatusText(statusBase, &statusMessage, &statusColor));

		// =========================
		// FOOTER
		// =========================
		sf::Text footer(*font, lobbyFooterText);
		footer.setCharacterSize(lobbyFooterSize);
		footer.setFillColor(lobbyFooterColor);
		{
			sf::FloatRect bounds = footer.getLocalBounds();
			footer.setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				WINDOW_HEIGHT * 0.89f
				});
		}
		objects.push_back(new ObjectText(footer));
	}

	bool update(sf::RenderWindow& window) override
	{
		if (LM->GetRoomJoined()) {
			statusMessage = "Lobby joined successfully";
			statusColor = lobbyStatusSuccessColor;
			nextScene = SceneOption::WAITING_ROOM;
		}

		return Scene::update(window);
	}

	void handleEvent(const sf::Event& event)
	{

		if (const sf::Event::KeyPressed* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->code == sf::Keyboard::Key::Enter) {
				attemptJoinLobby();
			}
		}
	}
};