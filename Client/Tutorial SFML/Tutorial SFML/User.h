#pragma once
#include <string>
#include <SFML/Network.hpp>
struct User
{
	std::string nickname;
	int score;
	int position;

	sf::IpAddress address = sf::IpAddress(0, 0, 0, 0);
	unsigned short port;

	int userIndex;
};

