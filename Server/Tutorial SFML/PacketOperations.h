#include "config.h"
#include "User.h"
#include "Move.h"
#include "Player.h"
#include "Database.h"

sf::Packet& operator>> (sf::Packet& packet, PacketType& type);

sf::Packet& operator<< (sf::Packet& packet, PacketType& type);

sf::Packet& operator>> (sf::Packet& packet, User& user);

sf::Packet& operator<< (sf::Packet& packet, User& user);

sf::Packet& operator>> (sf::Packet& packet, Move& move);

sf::Packet& operator<< (sf::Packet& packet, Move& move);

sf::Packet& operator<< (sf::Packet& packet, Player& player);

sf::Packet& operator<< (sf::Packet& packet, Database::RankingEntry& entry);