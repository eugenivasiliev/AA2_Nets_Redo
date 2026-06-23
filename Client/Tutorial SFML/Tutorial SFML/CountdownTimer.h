#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "PlayerManager.h"
#include "NetworkManager.h"

class CountdownTimer : public Object
{
private:
	PlayerManager* playerManager;
	bool countdownActive = false;
	sf::Clock countdownClock;
	sf::Text* timerText;

public:

	CountdownTimer(PlayerManager* _playerManager, sf::Text* _timerText) : playerManager(_playerManager), timerText(_timerText) { countdownClock.reset(); }

	void update() override {
		timerText->setString(std::to_string((int)std::floor(countdownClock.getElapsedTime().asSeconds())));

		if (playerManager->currentPlayer != NT->GetPlayerIndex()) { 
			countdownClock.reset(); 
			countdownActive = false; 
			return; 
		}

		if (!countdownActive) {
			countdownActive = true;
			countdownClock.start();
		}

		if (countdownClock.getElapsedTime().asSeconds() < PLAYER_COUNTDOWN) return;

		playerManager->SkipPlayer();
		countdownActive = false;
		countdownClock.reset();
	}

	void render(sf::RenderWindow& window) override { window.draw(*timerText); }
};

