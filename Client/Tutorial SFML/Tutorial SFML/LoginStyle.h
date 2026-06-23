#pragma once
#include <SFML/Graphics.hpp>
#include "config.h"

// BACKGROUND
const sf::Color loginBackgroundColor = sf::Color(12, 14, 22);
const sf::Color ambientGlowColor = sf::Color(88, 101, 242, 35);

// MAIN PANEL
const sf::Vector2f loginPanelSize = { WINDOW_WIDTH * 0.44f, WINDOW_HEIGHT * 0.76f };
const sf::Color loginPanelColor = sf::Color(22, 24, 34, 245);
const sf::Color loginPanelOutlineColor = sf::Color(255, 255, 255, 28);
const float loginPanelOutlineThickness = 2.0f;

// TEXTS
const std::string loginTitleText = "Welcome Back";
const std::string loginSubtitleText = "Login to continue";
const std::string loginFooterText = "Access your account or create a new one";

const unsigned int loginTitleSize = 42;
const unsigned int loginSubtitleSize = 20;
const unsigned int loginFooterSize = 16;

const sf::Color loginTitleColor = sf::Color(245, 247, 255);
const sf::Color loginSubtitleColor = sf::Color(170, 176, 196);
const sf::Color loginFooterColor = sf::Color(130, 136, 156);

// INPUT FIELDS
const sf::Vector2f nicknameRectSize = { WINDOW_WIDTH * 0.28f, WINDOW_HEIGHT * 0.085f };
const float nicknameRectAnchorX = 0.5f;
const float nicknameRectAnchorY = 0.32f;
const sf::Color nicknameRectColour = sf::Color(34, 37, 49);
const int nicknameMaxChars = 10;

const sf::Vector2f passwordRectSize = { WINDOW_WIDTH * 0.28f, WINDOW_HEIGHT * 0.085f };
const float passwordRectAnchorX = 0.5f;
const float passwordRectAnchorY = 0.47f;
const sf::Color passwordRectColour = sf::Color(34, 37, 49);
const int passwordMaxChars = 10;

const sf::Color inputOutlineColor = sf::Color(255, 255, 255, 30);
const sf::Color inputLabelColor = sf::Color(190, 196, 214);
const sf::Color inputTextColor = sf::Color(236, 240, 255);
const sf::Color inputPlaceholderColor = sf::Color(115, 120, 140);

const unsigned int inputLabelSize = 18;
const unsigned int inputTextSize = 26;
const unsigned int inputPlaceholderSize = 22;
const float inputOutlineThickness = 2.0f;

// BUTTONS
const sf::Vector2f loginRectSize = { WINDOW_WIDTH * 0.28f, WINDOW_HEIGHT * 0.085f };
const float loginRectAnchorX = 0.5f;
const float loginRectAnchorY = 0.62f;
const sf::Color loginRectColour = sf::Color(88, 101, 242);
const std::string loginButtonLabel = "Login";

const sf::Vector2f registerRectSize = { WINDOW_WIDTH * 0.28f, WINDOW_HEIGHT * 0.085f };
const float registerRectAnchorX = 0.5f;
const float registerRectAnchorY = 0.74f;
const sf::Color registerRectColour = sf::Color(46, 50, 66);
const std::string registerButtonLabel = "Register";

const unsigned int buttonTextSize = 28;

// DECORATIVE GLOW
const float topGlowRadius = 180.f;
const float bottomGlowRadius = 140.f;

// FOCUS / STATUS
const sf::Color inputFocusOutlineColor = sf::Color(88, 101, 242, 220);
const sf::Color inputIdleOutlineColor = sf::Color(255, 255, 255, 30);

const sf::Color statusSuccessColor = sf::Color(90, 200, 140);
const sf::Color statusErrorColor = sf::Color(220, 90, 90);
const sf::Color statusNeutralColor = sf::Color(160, 168, 190);

const unsigned int statusTextSize = 18;

const sf::Color panelAccentColor = sf::Color(88, 101, 242, 180);