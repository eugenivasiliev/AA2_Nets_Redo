#pragma once
#include <SFML/Network.hpp>
#include "config.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include "PacketOperations.h"
#include <iostream>
#include "Database.h"
#include "MatchMakingManager.h"

#define NT NetworkManager::Instance()

class NetworkManager
{
private:
    const std::unordered_map<PacketType, std::function<void(sf::Packet&, sf::TcpSocket*)>> packetFunctions{
        { PacketType::HANDSHAKE, [&](sf::Packet& packet, sf::TcpSocket* client) { ReceiveHandshake(); } },
        { PacketType::LOGIN, [&](sf::Packet& packet, sf::TcpSocket* client) { ReceiveLogin(packet, client); } },
        { PacketType::REGISTER, [&](sf::Packet& packet, sf::TcpSocket* client) { ReceiveRegister(packet, client); } },
        { PacketType::LOBBY_JOIN, [&](sf::Packet& packet, sf::TcpSocket* client) { ReceiveLobbyJoin(packet, client); } },
        { PacketType::LOBBY_CREATE, [&](sf::Packet& packet, sf::TcpSocket* client) { ReceiveLobbyCreate(packet, client); } },
        { PacketType::END_GAME, [&](sf::Packet& packet, sf::TcpSocket* client) { ReceiveGameEnd(packet, client); } },
        { PacketType::RANKING, [&](sf::Packet& packet, sf::TcpSocket* client) { ReceiveRanking(packet, client); } },
    };

    sf::SocketSelector selector;
    sf::TcpListener listener;
    std::vector<sf::TcpSocket*> clients{};
    unsigned short localPort;
    bool connected = false;

    bool login = false;

    int playerIndex;

    std::vector<Move> pendingMoves;

    std::vector<User> rankingUsers{};

public:
    static NetworkManager* Instance() {
        static NetworkManager nm;
        return &nm;
    }

    bool Init() {
        if (listener.listen(SERVER_PORT) != sf::Socket::Status::Done)
            return false;

        selector.add(listener);
        std::cout << "Server open" << std::endl;
        connected = true;
        return true;
    }

    bool Update() {
        if (!selector.wait() && !connected) return connected;

        AcceptConnections();

        if (selector.isReady(listener)) return connected;

        for (int i = 0; i < clients.size(); i++) {
            if (selector.isReady(*clients[i])) {
                sf::Packet packet;
                sf::Socket::Status status = clients[i]->receive(packet);

                if (status == sf::Socket::Status::Done) {
                    PacketType type;
                    packet >> type;
                    packetFunctions.at(type)(packet, clients[i]);
                    packet.clear();
                }

                if (status == sf::Socket::Status::Disconnected) {
                    MM->RemoveConnectedPlayer(clients[i]);

                    selector.remove(*clients[i]);

                    delete clients[i];
                    clients.erase(clients.begin() + i);
                    i--;

                    std::cout << "Disconnected" << std::endl;
                }
            }
        }

        return connected;
    }

    void AcceptConnections()
    {
        if (!selector.isReady(listener)) return;

        sf::TcpSocket* newClient = new sf::TcpSocket();

        if (listener.accept(*newClient) != sf::Socket::Status::Done) return;

        newClient->setBlocking(false);

        SendHandshake(newClient);
        selector.add(*newClient);

        clients.push_back(newClient);

        std::cout << "Connected new client" << std::endl;
    }

    int GetConnectedClientCount() { return clients.size(); }
    int GetPlayerIndex() { return playerIndex; }

    std::vector<User> GetRankingUsers() { return rankingUsers; }

public:
    void SendHandshake(sf::TcpSocket* client) {
        sf::Packet packet;
        packet << PacketType::HANDSHAKE;
        client->send(packet);
    }

    void SendLogin(sf::TcpSocket* client, bool success) {
        sf::Packet packet;
        packet << PacketType::LOGIN;
        packet << success;
        client->send(packet);
    }

    void SendRegister(sf::TcpSocket* client, bool success) {
        sf::Packet packet;
        packet << PacketType::REGISTER;
        packet << success;
        client->send(packet);
    }

    void SendLobbyCreate(sf::TcpSocket* client, bool success, std::string lobbyID) {
        sf::Packet packet;
        packet << PacketType::LOBBY_CREATE;
        packet << success;
        packet << lobbyID;
        client->send(packet);
    }

    void SendLobbyJoin(sf::TcpSocket* client, bool success, std::string lobbyID) {
        sf::Packet packet;
        packet << PacketType::LOBBY_JOIN;
        packet << success;
        packet << lobbyID;
        client->send(packet);
    }

    void SendRanking(sf::TcpSocket* client) {
        sf::Packet packet;
        packet << PacketType::RANKING;
        std::vector<User> entries = DB->GetTop10Rankings();
        packet << entries.size();
        std::cout << entries.size() << std::endl;
        for (User entry : entries)
            packet << entry;
        client->send(packet);
    }

    void SendRoomPlayers(GameRoom* room) {
        sf::Packet packet;
        packet << PacketType::ROOM_PLAYERS;
        int count = room->GetPlayerAmount();
        packet << count;
        for (int i = 0; i < count; i++) {
            User user;
            Player* player = room->GetPlayer(i);
            user.nickname = player->name;
            user.score = player->points;
            user.address = player->client->getRemoteAddress().value();
            user.port = static_cast<unsigned short>(55001 + i);
            user.userIndex = i;
            packet << user;
        }
        for (int i = 0; i < count; i++) 
            room->GetPlayer(i)->client->send(packet);
    }

private:

    void ReceiveHandshake() {}

    void ReceiveLogin(sf::Packet& packet, sf::TcpSocket* client) {
        std::string loginUsername;
        std::string loginPassword;

        packet >> loginUsername;
        packet >> loginPassword;

        int userId = 0;

        bool correctLogin = DB->LoginUser(loginUsername, loginPassword, userId);

        bool userAlreadyConnected = false;

        if (correctLogin)
            userAlreadyConnected = MM->CheckIfConnectedPlayerExists(client, loginUsername, 15);

        correctLogin = correctLogin && !userAlreadyConnected;

        SendLogin(client, correctLogin);

        if (correctLogin) {
            MM->AddConnectedPlayer(client, loginUsername, 15);
        }
    }
    void ReceiveRegister(sf::Packet& packet, sf::TcpSocket* client) {
        std::string registerUsername;
        std::string registerPassword;

        packet >> registerUsername;
        packet >> registerPassword;

        std::string passwordHash = bcrypt::generateHash(registerPassword);

        bool correctRegister = DB->RegisterUser(registerUsername, passwordHash);

        SendRegister(client, correctRegister);

        if (correctRegister) {
            MM->AddConnectedPlayer(client, registerUsername, 15);
        }
    }

    void ReceiveLobbyJoin(sf::Packet& packet, sf::TcpSocket* client) {
        std::string lobbyID;

        packet >> lobbyID;

        bool successfulLobbyJoin = MM->JoinWaitingRoom(lobbyID, client);

        SendLobbyJoin(client, successfulLobbyJoin, lobbyID);

        if (!successfulLobbyJoin) return;

        GameRoom* room = MM->GetGameInfo(lobbyID);
        SendRoomPlayers(room);
    }

    void ReceiveLobbyCreate(sf::Packet& packet, sf::TcpSocket* client) {
        std::string lobbyID;

        packet >> lobbyID;

        bool successfulLobbyCreation = MM->CreateWaitingRoom(lobbyID, client);

        SendLobbyCreate(client, successfulLobbyCreation, lobbyID);
    }

    void ReceiveGameEnd(sf::Packet& packet, sf::TcpSocket* client) {
        std::string lobbyID;
        packet >> lobbyID;
        MM->RemoveWaitingRoom(lobbyID);

        std::string nickname;
        packet >> nickname;
        bool won;
        packet >> won;

        if (won)
            DB->ModifyScore(nickname, WIN_SCORE);
    }

    void ReceiveRanking(sf::Packet& packet, sf::TcpSocket* client) {
        SendRanking(client);
    }

public:
    bool GetLoginStatus() { return login; }
};

