#pragma once
#include <SFML/Network.hpp>
#include "config.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "LobbyManager.h"
#include "PacketOperations.h"
#include <iostream>

#define NT NetworkManager::Instance()

class NetworkManager
{
private:
    const std::unordered_map<PacketType, std::function<void(sf::Packet&)>> packetFunctions {
        { PacketType::HANDSHAKE, [&](sf::Packet& packet) { ReceiveHandshake(); } },
        { PacketType::LOGIN, [&](sf::Packet& packet) { ReceiveLogin(packet); } },
        { PacketType::REGISTER, [&](sf::Packet& packet) { ReceiveRegister(packet); } },
        { PacketType::LOBBY_JOIN, [&](sf::Packet& packet) { ReceiveLobbyJoin(packet); } },
        { PacketType::LOBBY_CREATE, [&](sf::Packet& packet) { ReceiveLobbyCreate(packet); } },
        { PacketType::ROOM_PLAYERS, [&](sf::Packet& packet) { ReceiveRoomPlayers(packet); } },
        { PacketType::RANKING, [&](sf::Packet& packet) { ReceiveRanking(packet); } },
        { PacketType::MOVE, [&](sf::Packet& packet) { ReceiveMove(packet); } },
    };

    sf::TcpSocket socket;
    sf::TcpListener* listener;
    std::vector<sf::TcpSocket*> connectedClients{};
    unsigned short localPort;
    bool connected = false;

    bool login = false;
    std::string nickname;

    int playerIndex;

    std::vector<Move> pendingMoves;

    std::vector<User> rankingUsers{};

    unsigned short p2pPort;

    bool p2pStarted = false;                 
    std::vector<int> connectedPeerIndices;   

public:
	static NetworkManager* Instance() {
		static NetworkManager nm;
		return &nm;
	}

	bool Init() {
        if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done) {
            std::cerr << "Could not connect" << std::endl;
            return false;
        }

        socket.setBlocking(false);
        connected = true;
        localPort = socket.getLocalPort();
        SendHandshake();
        std::cout << "Connected" << std::endl;

        return true;
	}

    void Update() {
        if (!connected) return;

        sf::Packet packet;
        sf::Socket::Status status = socket.receive(packet);
        if (status == sf::Socket::Status::Done)
        {
            PacketType type;
            packet >> type;
            packetFunctions.at(type)(packet);
            packet.clear();
        }

        if (status == sf::Socket::Status::Disconnected)
            connected = false;
    }

    void UpdateP2P() {
        if (!connected) return;

        for (int i = 0; i < connectedClients.size(); i++) {
            sf::Packet packet;
            sf::Socket::Status status = connectedClients[i]->receive(packet);
            if (status == sf::Socket::Status::Done)
            {
                std::cout << "Received" << std::endl;
                PacketType type;
                packet >> type;
                packetFunctions.at(type)(packet);
                packet.clear();
            }

            if (status == sf::Socket::Status::Disconnected)
                connected = false;
        }
    }

    void ConnectP2P() {
        //Listen
        if (listener == nullptr) return;

        sf::TcpSocket* p2pSocket = new sf::TcpSocket();

        if (listener->accept(*p2pSocket) == sf::Socket::Status::Done)
        {
            p2pSocket->setBlocking(false);
            connectedClients.push_back(p2pSocket);
            std::cout << "Accepted p2p connection (" << connectedClients.size() << ")" << std::endl;
        }
        else
        {
            delete p2pSocket; 
        }

        //Connect
        for (User player : LM->GetPlayers())
        {
            if (player.userIndex <= playerIndex) continue;
            if (std::find(connectedPeerIndices.begin(), connectedPeerIndices.end(),
                player.userIndex) != connectedPeerIndices.end()) continue;

            p2pSocket = new sf::TcpSocket();

            if (p2pSocket->connect(player.address, player.port, sf::seconds(1)) != sf::Socket::Status::Done)
            {
                delete p2pSocket;
                continue;
            }

            p2pSocket->setBlocking(false);
            connectedClients.push_back(p2pSocket);
            connectedPeerIndices.push_back(player.userIndex);

            std::cout << "Connected to peer " << player.userIndex << " at "
                << player.address.toString() << ':' << player.port << std::endl;
        }
    }

    void RemoveP2P() {
        for (sf::TcpSocket* client : connectedClients) delete client;
        connectedClients.clear();
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

    void SetNickname(std::string nick) { nickname = nick; }
    std::string GetNickname() { return nickname; }

public:
    void SendHandshake() {
        sf::Packet packet;
        packet << PacketType::HANDSHAKE;
        socket.send(packet);
    }

    void SendLogin(std::string username, std::string password) {
        sf::Packet packet;
        packet << PacketType::LOGIN;
        packet << username;
        packet << password;
        socket.send(packet);
    }

    void SendRegister(std::string username, std::string password) {
        sf::Packet packet;
        packet << PacketType::REGISTER;
        packet << username;
        packet << password;
        socket.send(packet);
    }

    void SendLobbyCreate(std::string id) {
        sf::Packet packet;
        packet << PacketType::LOBBY_CREATE;
        packet << id;
        socket.send(packet);
    }

    void SendLobbyJoin(std::string id) {
        sf::Packet packet;
        packet << PacketType::LOBBY_JOIN;
        packet << id;
        socket.send(packet);
    }

    void SendMove(Move move) {
        sf::Packet packet;
        packet << PacketType::MOVE;
        packet << move;
        for (int i = 0; i < connectedClients.size(); i++) {
            std::cout << "Sent move to " << i << std::endl;
            while(connectedClients[i]->send(packet) != sf::Socket::Status::Done);
        }
    }

    void SendGameEnd(std::vector<bool> winners) {
        sf::Packet packet;
        packet << PacketType::END_GAME;
        packet << LM->GetRoomId();
        packet << nickname;
        packet << winners[playerIndex];
        socket.send(packet);
    }

    void SendRanking() {
        sf::Packet packet;
        packet << PacketType::RANKING;
        socket.send(packet);
    }

private:
    
    void ReceiveHandshake() {}

    void ReceiveLogin(sf::Packet& packet) { packet >> login; }
    void ReceiveRegister(sf::Packet& packet) { packet >> login; }

    void ReceiveLobbyJoin(sf::Packet& packet) { 
        bool success;
        packet >> success;

        if (!success) return;

        std::string id;
        packet >> id;

        LM->JoinRoom(id);

        packet >> playerIndex;
    }
    void ReceiveLobbyCreate(sf::Packet& packet) {
        bool success;
        packet >> success;

        if (!success) return;

        std::string id;
        packet >> id;

        LM->JoinRoom(id);

        playerIndex = 0;
    }

    void ReceiveRoomPlayers(sf::Packet& packet) {
        LM->ClearPlayers();
        int newPlayerCount;
        packet >> newPlayerCount;
        for (int i = 0; i < newPlayerCount; i++) {
            User user;
            packet >> user;
            LM->AddPlayer(user);

            if (user.nickname == nickname) {
                playerIndex = user.userIndex;
                p2pPort = user.port;
                BeginP2P();
            }
        }
    }

    void ReceiveRanking(sf::Packet& packet) {
        rankingUsers.clear();
        size_t rankingPlayerCount;
        packet >> rankingPlayerCount;
        for (int i = 0; i < rankingPlayerCount; i++) {
            User user;
            packet >> user;
            rankingUsers.push_back(user);
        }
    }

    void ReceiveMove(sf::Packet& packet) {
        std::cout << "Move" << std::endl;
        Move move = nullMove;
        packet >> move;
        pendingMoves.push_back(move);
    }

public:
    bool GetLoginStatus() { return login; }

private:
    void BeginP2P() {
        if (p2pStarted) return;
        p2pStarted = true;

        listener = new sf::TcpListener();
        listener->setBlocking(false);
        if (listener->listen(p2pPort) != sf::Socket::Status::Done) {
            std::cerr << "Could not create p2p port " << p2pPort << std::endl;
            return;
        }
        std::cout << "Listening for p2p on port " << p2pPort << std::endl;
    }
};

