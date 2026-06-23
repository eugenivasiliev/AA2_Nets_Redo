#pragma once
#include <SFML/Graphics.hpp>
#include "config.h"

const sf::Font emptyFont = sf::Font::Font();

// =========================
// BACKGROUND
// =========================
const sf::Color rankingBackgroundColor = sf::Color(16, 18, 28);

// =========================
// PANEL
// =========================
const sf::Vector2f rankingPanelSize = { WINDOW_WIDTH * 0.52f, WINDOW_HEIGHT * 0.78f };
const sf::Color rankingPanelColor = sf::Color(24, 28, 40, 245);
const sf::Color rankingPanelOutlineColor = sf::Color(255, 255, 255, 20);
const float rankingPanelOutlineThickness = 2.0f;

// =========================
// X / O COLORS
// =========================
const sf::Color rankingAccentXColor = sf::Color(80, 170, 255);
const sf::Color rankingAccentOColor = sf::Color(255, 120, 170);

// =========================
// TEXT
// =========================
const std::string rankingTitleText = "Ranking";
const std::string rankingSubtitleText = "Top players";
const std::string rankingFooterText = "Compete, climb and stay at the top";

const unsigned int rankingTitleSize = 42;
const unsigned int rankingSubtitleSize = 20;
const unsigned int rankingFooterSize = 16;

const sf::Color rankingTitleColor = sf::Color(240, 245, 255);
const sf::Color rankingSubtitleColor = sf::Color(150, 160, 180);
const sf::Color rankingFooterColor = sf::Color(120, 130, 150);

// =========================
// LIST
// =========================
const float rankingsWidth = WINDOW_WIDTH * 0.36f;
const float rankingsHeight = WINDOW_HEIGHT * 0.07f;

const float rankingsAnchorX = 0.32f;
const float rankingsAnchorY = 0.25f;

const float rankingsSeparation = WINDOW_HEIGHT * 0.018f;

const sf::Color rankingsBackground = sf::Color(34, 37, 49);
const sf::Color rankingsTopThreeBackground = sf::Color(45, 52, 72);
const sf::Color rankingsOutlineColor = sf::Color(255, 255, 255, 18);

const int rankingsCharacterSize = 30;
const sf::Text::Style rankingsTextStyle = sf::Text::Style::Bold;

const sf::Color rankingsTextColor = sf::Color(236, 240, 255);
const sf::Color rankingsIndexColor = sf::Color(190, 196, 214);

// =========================
// DECORATION
// =========================
const sf::Color rankingGridColor = sf::Color(255, 255, 255, 15);
const sf::Color rankingDecorativeXColor = sf::Color(rankingAccentXColor.r, rankingAccentXColor.g, rankingAccentXColor.b, 40);
const sf::Color rankingDecorativeOColor = sf::Color(rankingAccentOColor.r, rankingAccentOColor.g, rankingAccentOColor.b, 40);