#pragma once
#include "config.h"
#include <SFML/Graphics.hpp>

const sf::Font nullFont = sf::Font::Font();

#pragma region GLOBAL

// Background
const sf::Color gameBackgroundColor = sf::Color(16, 18, 28);

// X / O colors
const sf::Color gameAccentXColor = sf::Color(80, 170, 255);
const sf::Color gameAccentOColor = sf::Color(255, 120, 170);

// Panels
const sf::Color gamePanelColor = sf::Color(24, 28, 40, 245);
const sf::Color gamePanelOutlineColor = sf::Color(255, 255, 255, 20);
const float gamePanelOutlineThickness = 2.0f;

// Decorations
const sf::Color gameGridColor = sf::Color(255, 255, 255, 15);
const sf::Color gameDecorativeXColor = sf::Color(gameAccentXColor.r, gameAccentXColor.g, gameAccentXColor.b, 40);
const sf::Color gameDecorativeOColor = sf::Color(gameAccentOColor.r, gameAccentOColor.g, gameAccentOColor.b, 40);

// Title
const std::string gameTitleText = "Match";
const std::string gameSubtitleText = "Tic Tac Toe Online";

const unsigned int gameTitleSize = 40;
const unsigned int gameSubtitleSize = 18;

const sf::Color gameTitleColor = sf::Color(240, 245, 255);
const sf::Color gameSubtitleColor = sf::Color(150, 160, 180);

#pragma endregion


#pragma region BOARD

// Anchor 
const float boardAnchorX = 0.22f;
const float boardAnchorY = 0.22f;

// Size
const float boardSideLength = WINDOW_WIDTH * 0.35f;

// Panel 
const sf::Vector2f boardPanelSize = {
	boardSideLength + 40.f,
	boardSideLength + 40.f
};

// Cells
const float cellPadding = 0.10f;

// Player colors (used for X/O)
const sf::Color colours[PLAYER_COUNT + 1] = {
	sf::Color(0, 0, 0),        // Default
	sf::Color(80, 170, 255),
	sf::Color(255, 120, 170),
	sf::Color(130, 220, 140),
	sf::Color(255, 210, 90)
};

#pragma endregion


#pragma region PLAYERS

// Anchor 
const float playersAnchorX = 0.67f;
const float playersAnchorY = 0.28f;

// Card size
const float playersWidth = WINDOW_WIDTH * 0.18f;
const float playersHeight = WINDOW_HEIGHT * 0.09f;

// Spacing between players
const float playersSeparation = WINDOW_HEIGHT * 0.03f;

// Panel size 
const sf::Vector2f playersPanelSize = {
	WINDOW_WIDTH * 0.22f,
	WINDOW_HEIGHT * 0.36f
};

// Colors
const sf::Color playersBackground = sf::Color(34, 37, 49);
const sf::Color activePlayersBackground = sf::Color(60, 120, 200);

// Text
const int playersCharacterSize = 32;
const sf::Text::Style playersTextStyle = sf::Text::Style::Bold;

#pragma endregion


#pragma region TIMER

// Anchor 
const float timerPanelAnchorX = 0.67f;
const float timerPanelAnchorY = 0.72f;

// Size
const sf::Vector2f timerPanelSize = {
	WINDOW_WIDTH * 0.22f,
	WINDOW_HEIGHT * 0.14f
};

// Colors
const sf::Color timerPanelColor = sf::Color(24, 28, 40, 245);
const sf::Color timerPanelOutlineColor = sf::Color(255, 255, 255, 20);

#pragma endregion