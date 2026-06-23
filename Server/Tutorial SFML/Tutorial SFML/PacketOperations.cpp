#include "PacketOperations.h"
sf::Packet& operator>> (sf::Packet& packet, PacketType& type) {
    int uncasted;
    packet >> uncasted;
    type = static_cast<PacketType>(uncasted);
    return packet;
}

sf::Packet& operator<< (sf::Packet& packet, PacketType& type) {
    packet << static_cast<int>(type);
    return packet;
}

sf::Packet& operator>> (sf::Packet& packet, User& user) {
    std::string stringAddress;
    sf::IpAddress address = sf::IpAddress(0, 0, 0, 0);
    unsigned short port;

    packet >> user.nickname;
    packet >> user.score;
    packet >> user.userIndex;
    packet >> user.position;

    packet >> stringAddress;
    user.address = sf::IpAddress::resolve(stringAddress).value();
    packet >> user.port;
    return packet;
}

sf::Packet& operator<< (sf::Packet& packet, User& user) {
    std::string stringAddress = user.address.toString();

    packet << user.nickname;
    packet << user.score;
    packet << user.userIndex;
    packet << user.position;

    packet << stringAddress;
    packet << user.port;
    return packet;
}

sf::Packet& operator>> (sf::Packet& packet, Move& move) {
    packet >> move.row;
    packet >> move.column;
    packet >> move.playerIndex;
    return packet;
}

sf::Packet& operator<< (sf::Packet& packet, Move& move) {
    packet << move.row;
    packet << move.column;
    packet << move.playerIndex;
    return packet;
}

sf::Packet& operator<< (sf::Packet& packet, Player& player) {
    packet << player.client->getRemoteAddress().value().toString();
    packet << player.client->getRemotePort();
    packet << player.name;
    packet << player.points;
}