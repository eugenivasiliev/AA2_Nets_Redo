#include "config.h"
#include "User.h"
#include "Move.h"
#include "Player.h"

sf::Packet& operator>> (sf::Packet& packet, PacketType& type);

sf::Packet& operator<< (sf::Packet& packet, PacketType& type);

sf::Packet& operator>> (sf::Packet& packet, User& user);

sf::Packet& operator<< (sf::Packet& packet, User& user);

sf::Packet& operator>> (sf::Packet& packet, Move& move);

sf::Packet& operator<< (sf::Packet& packet, Move& move);

sf::Packet& operator>> (sf::Packet& packet, Player& move);

sf::Packet& operator<< (sf::Packet& packet, Player& move);