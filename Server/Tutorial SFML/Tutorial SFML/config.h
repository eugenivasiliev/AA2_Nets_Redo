#pragma once
#include <SFML/Network.hpp>

#define SERVER_IP sf::IpAddress(127, 0, 0, 1)
#define SERVER_PORT 5500

#define P2P_PORT 5600

#define WINDOW_NAME "AA1 Nets"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define BOARD_WIDTH 6
#define BOARD_HEIGHT 6
#define PLAYER_COUNT 4

#define PLAYER_COUNTDOWN 20.0f //In seconds

#define START_GAME_MAX_WAIT_TIME 5.0f

enum class SceneOption {
	NONE,
	LOGIN,
	LOBBY,
	WAITING_ROOM,
	GAME,
	RANKING
};

#define FONT_PATH "arial.ttf"

enum PacketType { HANDSHAKE, LOGIN, REGISTER, LOBBY_CREATE, LOBBY_JOIN, ROOM_PLAYERS, RANKING, START_GAME, END_GAME, MOVE };