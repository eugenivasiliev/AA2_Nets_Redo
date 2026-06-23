#include "MatchMakingManager.h"
#include "NetworkManager.h"
#include <iostream>

bool MatchMakingManager::IsAvailableRoomId(std::string roomId)
{
	for (int i = 0; i < waitingRooms.size(); i++)
	{
		if (roomId == waitingRooms[i].GetId())
			return false;
	}
	return true;
}

Player* MatchMakingManager::GetPlayer(sf::TcpSocket* playerClient)
{
	for (int i = 0; i < connectedPlayers.size(); i++)
	{
		if (playerClient == connectedPlayers[i].client)
			return &connectedPlayers[i];
	}

	return nullptr;
}

GameRoom* MatchMakingManager::GetGameInfo(std::string roomId)
{
	for (int i = 0; i < waitingRooms.size(); i++)
	{
		if (roomId == waitingRooms[i].GetId())
			return &waitingRooms[i];
	}

	return nullptr;
}

void MatchMakingManager::AddConnectedPlayer(sf::TcpSocket* playerClient, std::string username, int playerPoints)
{
	connectedPlayers.push_back(Player(playerClient, username, playerPoints));
}

bool MatchMakingManager::CheckIfConnectedPlayerExists(sf::TcpSocket* playerClient, std::string username, int playerPoints)
{
	bool playerExists = false;

	for (int i = 0; i < connectedPlayers.size(); i++)
	{
		if (username == connectedPlayers[i].name)
		{
			playerExists = true;
			std::cout << "El jugador " << username << " ya ha iniciado sesion." << std::endl;
			continue;
		}
	}

	return playerExists;
}

void MatchMakingManager::RemoveConnectedPlayer(sf::TcpSocket* playerClient)
{
	for(int i = 0; i < connectedPlayers.size(); i++)
	{
		if (playerClient == connectedPlayers[i].client)
		{
			std::cout << "El jugador " << connectedPlayers[i].name << " se ha desconectado." << std::endl;
			connectedPlayers.erase(connectedPlayers.begin() + i);
			return;
		}
	}
}

bool MatchMakingManager::CreateWaitingRoom(std::string roomId, sf::TcpSocket* playerClient)
{		
	if (!IsAvailableRoomId(roomId))
		return false;
	
	Player* player = GetPlayer(playerClient);

	if (player == nullptr)
	{
		std::cout << "Error: Jugador no encontrado";
		return false;
	}

	GameRoom room(roomId);
	room.AddPlayer(*player);

	waitingRooms.push_back(room);

	return true;
}

bool MatchMakingManager::JoinWaitingRoom(std::string roomId, sf::TcpSocket* playerClient)
{
	for (int i = 0; i < waitingRooms.size(); i++)
	{
		if (roomId == waitingRooms[i].GetId())
		{
			if (waitingRooms[i].IsFull()) return false;
			std::cout << "Es waiting room " << i << std::endl;
			waitingRooms[i].AddPlayer(*GetPlayer(playerClient));

			return true;
		}
	}
	return false;
}

bool MatchMakingManager::RemoveWaitingRoom(std::string roomId)
{
	for (int i = 0; i < waitingRooms.size(); i++)
	{
		if (roomId == waitingRooms[i].GetId())
		{
			waitingRooms.erase(waitingRooms.begin() + i);
			return true;
		}
	}
	return false;
}
