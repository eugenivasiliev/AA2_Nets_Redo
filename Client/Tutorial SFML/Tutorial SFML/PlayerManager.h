#pragma once
#include "Player.h"
#include "User.h"
#include "Object.h"
#include "Move.h"
#include "NetworkManager.h"

class PlayerManager : public Object
{
public:
	int currentPlayer = 0;
	PlayerRect players[PLAYER_COUNT];
	User user;
	std::vector<bool> winners;

public:
	void GetNextPlayer() {
		players[currentPlayer].isCurrent = false;

		do {
			currentPlayer++;
			currentPlayer %= PLAYER_COUNT;
		} while (winners[currentPlayer]);

		players[currentPlayer].isCurrent = true;
	}

	void SkipPlayer() {
		NT->SendMove(noMove);
		GetNextPlayer();
	}

	int WinnerCount() {
		int sum = 0;
		for (int i = 0; i < PLAYER_COUNT; i++) sum += winners[i];
		return sum;
	}

	void FinishGame() {
		NT->SendGameEnd(winners);
	}

public:
	PlayerManager() {}

	PlayerManager(User _user, std::vector<User> _otherUsers, sf::Font& font) : user(_user) {

		for (int i = 0; i < PLAYER_COUNT; ++i)
			winners.push_back(false);

		for (int index = 0; index < PLAYER_COUNT; ++index)
			if (_user.userIndex == index)
				players[index] = PlayerRect(_user, font);
			else for (int othersIndex = 0; othersIndex < PLAYER_COUNT - 1; ++othersIndex) 
					if (_otherUsers[othersIndex].userIndex == index) 
						players[index] = PlayerRect(_otherUsers[othersIndex], font);

		players[currentPlayer].isCurrent = true;
	}

	void render(sf::RenderWindow& window) override {
		for (PlayerRect player : players)
			player.render(window);
	}
};

