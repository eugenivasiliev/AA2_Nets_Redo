#pragma once
#include <SFML/Window.hpp>

#define TIME TimeManager::Instance()

class TimeManager
{
private:
	sf::Clock clock;

public:
	static TimeManager* Instance() {
		static TimeManager tm;
		return &tm;
	}

	float dt = 0;
	float elapsedTime = 0;

	void Update() {
		dt = clock.getElapsedTime().asSeconds() - elapsedTime;
		elapsedTime = clock.getElapsedTime().asSeconds();
	}
};

