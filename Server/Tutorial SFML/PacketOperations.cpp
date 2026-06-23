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

    packet >> user.nickname;
    packet >> user.score;
    packet >> user.position;

    packet >> stringAddress;
    user.address = sf::IpAddress::resolve(stringAddress).value();
    packet >> user.port;

    packet >> user.userIndex;
    return packet;
}

sf::Packet& operator<< (sf::Packet& packet, User& user) {
    std::string stringAddress = user.address.toString();

    packet << user.nickname;
    packet << user.score;
    packet << user.position;

    packet << stringAddress;
    packet << user.port;

    packet << user.userIndex;
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

sf::Packet& operator<< (sf::Packet& packet, Database::RankingEntry& entry) {
    User user;
    user.nickname = entry.username;
    user.score = entry.points;
    user.userIndex = entry.userId;
    user.position = entry.position;

    packet << user;
    return packet;
}