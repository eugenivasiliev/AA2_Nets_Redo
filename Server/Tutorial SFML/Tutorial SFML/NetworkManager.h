#pragma once
#include <SFML/Network.hpp>
#include "config.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include "PacketOperations.h"
#include <iostream>
#include "Move.h"
#include "User.h"
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
        { PacketType::ROOM_PLAYERS, [&](sf::Packet& packet, sf::TcpSocket* client) { std::cerr << "Received unexpected packet"; }},
        { PacketType::RANKING, [&](sf::Packet& packet, sf::TcpSocket* client) { ReceiveRanking(packet); } },
    };

    sf::TcpListener* listener;
    sf::SocketSelector selector;
    std::vector<sf::TcpSocket*> connectedClients{};
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
        listener = new sf::TcpListener();

        if (listener->listen(SERVER_PORT) != sf::Socket::Status::Done) {
            std::cerr << "Could not connect" << std::endl;
            return false;
        }

        connected = true;
        selector.add(*listener);
        std::cout << "Connected" << std::endl;
        return true;
    }

    bool Update()
    {
        if (!connected || !selector.wait()) return connected;
        AcceptNewConnections();
        ReceivePackets();
        return connected;
    }

    void AcceptNewConnections() {
        if (selector.isReady(*listener)) {
            sf::TcpSocket* newClient = new sf::TcpSocket();

            if (listener->accept(*newClient) == sf::Socket::Status::Done) {
                newClient->setBlocking(false);

                SendHandshake(*newClient);
                selector.add(*newClient);

                connectedClients.push_back(newClient);

                std::cout << "Nueva conexion establecida" << std::endl;
            }
        }
    }

    bool ReceivePackets() {

        if (!selector.isReady(*listener)) {
            for (int i = 0; i < connectedClients.size(); i++) {
                if (!selector.isReady(*connectedClients[i])) continue;

                sf::Packet packet;
                sf::Socket::Status status = connectedClients[i]->receive(packet);
                while (status == sf::Socket::Status::Done)
                {
                    PacketType type;
                    packet >> type;
                    packetFunctions.at(type)(packet, connectedClients[i]);
                    packet.clear();
                }
                if (status == sf::Socket::Status::Disconnected) {

                    MM->RemoveConnectedPlayer(connectedClients[i]);

                    selector.remove(*connectedClients[i]);

                    delete connectedClients[i];
                    connectedClients.erase(connectedClients.begin() + i);
                    i--;

                    std::cout << "Disconnection" << std::endl;
                }
            }
        }

        return connected;
    }

    int GetConnectedClientCount() { return connectedClients.size(); }
    int GetPlayerIndex() { return playerIndex; }

    Move GetPendingMove() {
        if (pendingMoves.empty()) return nullMove;

        Move pending = pendingMoves.back();
        pendingMoves.pop_back();
        return pending;
    }

    std::vector<User> GetRankingUsers() { return rankingUsers; }

private:
    void SendHandshake(sf::TcpSocket& client) {
        sf::Packet packet;
        packet << PacketType::HANDSHAKE;
        client.send(packet);
    }

    void SendLogin(sf::TcpSocket& client, bool success) {
        sf::Packet packet;
        packet << PacketType::LOGIN;
        packet << success;
        client.send(packet);
    }

    void SendRegister(sf::TcpSocket& client, bool success) {
        sf::Packet packet;
        packet << PacketType::REGISTER;
        packet << success;
        client.send(packet);
    }

    void SendLobbyCreate(sf::TcpSocket& client, bool success) {
        sf::Packet packet;
        packet << PacketType::LOBBY_CREATE;
        packet << success;
        client.send(packet);
    }

    void SendLobbyJoin(sf::TcpSocket& client, bool success) {
        sf::Packet packet;
        packet << PacketType::LOBBY_JOIN;
        packet << success;
        client.send(packet);
    }

    void SendRoomPlayers(sf::TcpSocket* client, std::string roomID) {
        sf::Packet packet;
        packet << PacketType::ROOM_PLAYERS;
        GameRoom* room = MM->GetGameInfo(roomID);
        for (int i = 0; i < room->GetPlayerAmount(); i++) {
            packet << *room->GetPlayer(i);
        }
        for (int i = 0; i < room->GetPlayerAmount(); i++) {
            room->GetPlayer(i)->client->send(packet);
        }
    }

    //void SendRanking() {
    //    sf::Packet packet;
    //    packet << PacketType::RANKING;
    //    socket.send(packet);
    //}

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
            userAlreadyConnected = MM->CheckIfConnectedPlayerExists(loginUsername);

        correctLogin = correctLogin && !userAlreadyConnected;

        SendLogin(*client, correctLogin);

        if (correctLogin) {
            MM->AddConnectedPlayer(client, loginUsername, 15);
        }
    }
    void ReceiveRegister(sf::Packet& packet, sf::TcpSocket* client) { 
        std::string registerUsername;
        std::string registerPassword;

        packet >> registerUsername;
        packet >> registerPassword;

        bool correctRegister = DB->RegisterUser(registerUsername, registerPassword);

        SendRegister(*client, correctRegister);

        if (correctRegister) {
            MM->AddConnectedPlayer(client, registerUsername, 15);
        }
    }

    void ReceiveLobbyJoin(sf::Packet& packet, sf::TcpSocket* client) {
        std::string lobbyID;

        packet >> lobbyID;

        bool successfulLobbyJoin = MM->JoinWaitingRoom(lobbyID, client);

        SendLobbyJoin(*client, successfulLobbyJoin);
        SendRoomPlayers(client, lobbyID);
    }

    void ReceiveLobbyCreate(sf::Packet& packet, sf::TcpSocket* client) {
        std::string lobbyID;

        packet >> lobbyID;

        bool successfulLobbyCreation = MM->CreateWaitingRoom(lobbyID, client);

        SendLobbyCreate(*client, successfulLobbyCreation);
    }

    void ReceiveRanking(sf::Packet& packet) {
        rankingUsers.clear();
        int rankingPlayerCount;
        packet >> rankingPlayerCount;
        for (int i = 0; i < rankingPlayerCount; i++) {
            User user;
            packet >> user;
            rankingUsers.push_back(user);
        }
    }

public:
    bool GetLoginStatus() { return login; }
};

