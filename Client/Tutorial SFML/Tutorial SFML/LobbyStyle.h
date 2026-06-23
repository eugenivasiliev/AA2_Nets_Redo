#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "config.h"

// =========================
// BACKGROUND
// =========================
const sf::Color lobbyBackgroundColor = sf::Color(16, 18, 28);

// =========================
// PANEL
// =========================
const sf::Vector2f lobbyPanelSize = { WINDOW_WIDTH * 0.48f, WINDOW_HEIGHT * 0.74f };
const sf::Color lobbyPanelColor = sf::Color(24, 28, 40, 245);
const sf::Color lobbyPanelOutlineColor = sf::Color(255, 255, 255, 20);
const float lobbyPanelOutlineThickness = 2.0f;

// =========================
// X / O ACCENTS
// =========================
const sf::Color lobbyAccentXColor = sf::Color(80, 170, 255);
const sf::Color lobbyAccentOColor = sf::Color(255, 120, 170);

// =========================
// TEXTS
// =========================
const std::string lobbyTitleText = "Lobby";
const std::string lobbySubtitleText = "Create a room or join an existing one";
const std::string lobbyFooterText = "Invite your friends and start the match";

const unsigned int lobbyTitleSize = 42;
const unsigned int lobbySubtitleSize = 20;
const unsigned int lobbyFooterSize = 16;

const sf::Color lobbyTitleColor = sf::Color(240, 245, 255);
const sf::Color lobbySubtitleColor = sf::Color(150, 160, 180);
const sf::Color lobbyFooterColor = sf::Color(120, 130, 150);

// =========================
// ROOM INPUT
// =========================
const sf::Vector2f lobbyRectSize = { WINDOW_WIDTH * 0.30f, WINDOW_HEIGHT * 0.085f };
const float lobbyRectAnchorX = 0.5f;
const float lobbyRectAnchorY = 0.36f;
const sf::Color lobbyRectColour = sf::Color(34, 37, 49);
const int lobbyMaxChars = 10;

const sf::Color lobbyInputIdleOutlineColor = sf::Color(255, 255, 255, 25);
const sf::Color lobbyInputFocusOutlineColor = lobbyAccentOColor;
const sf::Color lobbyInputLabelColor = sf::Color(190, 196, 214);
const sf::Color lobbyInputTextColor = sf::Color(236, 240, 255);
const sf::Color lobbyInputPlaceholderColor = sf::Color(115, 120, 140);

const unsigned int lobbyInputLabelSize = 18;
const unsigned int lobbyInputTextSize = 26;
const unsigned int lobbyInputPlaceholderSize = 22;
const float lobbyInputOutlineThickness = 2.0f;

// =========================
// BUTTONS
// =========================
const sf::Vector2f createRectSize = { WINDOW_WIDTH * 0.30f, WINDOW_HEIGHT * 0.085f };
const float createRectAnchorX = 0.5f;
const float createRectAnchorY = 0.54f;
const sf::Color createRectColour = lobbyAccentXColor;
const std::string createButtonLabel = "Create Lobby";

const sf::Vector2f joinRectSize = { WINDOW_WIDTH * 0.30f, WINDOW_HEIGHT * 0.085f };
const float joinRectAnchorX = 0.5f;
const float joinRectAnchorY = 0.67f;
const sf::Color joinRectColour = sf::Color(52, 58, 78);
const std::string joinButtonLabel = "Join Lobby";

const unsigned int lobbyButtonTextSize = 28;

// =========================
// STATUS
// =========================
const sf::Color lobbyStatusSuccessColor = sf::Color(90, 200, 140);
const sf::Color lobbyStatusErrorColor = sf::Color(220, 90, 90);
const sf::Color lobbyStatusNeutralColor = sf::Color(160, 168, 190);
const unsigned int lobbyStatusTextSize = 18;

const sf::Color lobbyPanelAccentColor = lobbyAccentXColor;

// =========================
// DECORATION
// =========================
const sf::Color lobbyGridColor = sf::Color(255, 255, 255, 15);
const sf::Color lobbyDecorativeXColor = sf::Color(lobbyAccentXColor.r, lobbyAccentXColor.g, lobbyAccentXColor.b, 40);
const sf::Color lobbyDecorativeOColor = sf::Color(lobbyAccentOColor.r, lobbyAccentOColor.g, lobbyAccentOColor.b, 40);