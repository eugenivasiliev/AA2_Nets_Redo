#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "config.h"

// =========================
// BACKGROUND
// =========================
const sf::Color waitingBackgroundColor = sf::Color(16, 18, 28);

// =========================
// PANEL
// =========================
const sf::Vector2f waitingPanelSize = { WINDOW_WIDTH * 0.50f, WINDOW_HEIGHT * 0.58f };
const sf::Color waitingPanelColor = sf::Color(24, 28, 40, 245);
const sf::Color waitingPanelOutlineColor = sf::Color(255, 255, 255, 20);
const float waitingPanelOutlineThickness = 2.0f;

// =========================
// X / O COLORS
// =========================
const sf::Color waitingAccentXColor = sf::Color(80, 170, 255);
const sf::Color waitingAccentOColor = sf::Color(255, 120, 170);

// =========================
// TEXT
// =========================
const std::string waitingTitleText = "Waiting Room";
const std::string waitingSubtitleText = "Waiting for players to join";

const unsigned int waitingTitleSize = 42;
const unsigned int waitingSubtitleSize = 20;
const unsigned int waitingRoomCodeLabelSize = 18;
const unsigned int waitingRoomCodeSize = 30;
const unsigned int waitingPlayerCountLabelSize = 18;
const unsigned int waitingPlayerCountSize = 54;
const unsigned int waitingFooterSize = 18;

const sf::Color waitingTitleColor = sf::Color(240, 245, 255);
const sf::Color waitingSubtitleColor = sf::Color(150, 160, 180);
const sf::Color waitingLabelColor = sf::Color(190, 196, 214);
const sf::Color waitingValueColor = sf::Color(236, 240, 255);
const sf::Color waitingFooterColor = sf::Color(120, 130, 150);

// =========================
// ROOM CODE BOX
// =========================
const sf::Vector2f waitingRoomCodeBoxSize = { WINDOW_WIDTH * 0.28f, WINDOW_HEIGHT * 0.10f };
const sf::Color waitingRoomCodeBoxColor = sf::Color(34, 37, 49);
const sf::Color waitingRoomCodeOutlineColor = waitingAccentXColor;

// =========================
// PLAYER COUNT BOX
// =========================
const sf::Vector2f waitingPlayerCountBoxSize = { WINDOW_WIDTH * 0.20f, WINDOW_HEIGHT * 0.13f };
const sf::Color waitingPlayerCountBoxColor = sf::Color(34, 37, 49);
const sf::Color waitingPlayerCountOutlineColor = waitingAccentOColor;

// =========================
// STATUS
// =========================
const std::string waitingFooterText = "The match will start automatically when everyone is ready";

// =========================
// DECORATION
// =========================
const sf::Color waitingGridColor = sf::Color(255, 255, 255, 15);
const sf::Color waitingDecorativeXColor = sf::Color(waitingAccentXColor.r, waitingAccentXColor.g, waitingAccentXColor.b, 40);
const sf::Color waitingDecorativeOColor = sf::Color(waitingAccentOColor.r, waitingAccentOColor.g, waitingAccentOColor.b, 40);