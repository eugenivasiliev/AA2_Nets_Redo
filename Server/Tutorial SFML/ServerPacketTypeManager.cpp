//#include "ServerPacketTypeManager.h"
//#include "MatchmakingManager.h"
//#include <iostream>
//
//sf::Packet& operator>>(sf::Packet& packet, PacketTypes& tipo) {
//	int temp;
//	packet >> temp;
//	tipo = static_cast<PacketTypes>(temp);
//
//	return packet;
//}
//
//sf::Packet& operator<<(sf::Packet& packet, PacketTypes& tipo) {
//	int temp;
//	temp = static_cast<int>(tipo);
//	packet << temp;
//
//	return packet;
//}
//
//sf::Packet& operator>>(sf::Packet& packet, Player& player) {
//	std::string ipRemoteAdrress;
//	packet >> ipRemoteAdrress;
//	std::optional<sf::IpAddress> ipAddress = sf::IpAddress::resolve(ipRemoteAdrress);
//
//	unsigned short remotePort;
//	packet >> remotePort;
//
//	packet >> player.name;
//	packet >> player.points;
//
//	return packet;
//}
//
//sf::Packet& operator<<(sf::Packet& packet, Player& player) {
//	std::string ipRemoteAdrress = player.client->getRemoteAddress()->toString();
//	packet << ipRemoteAdrress;
//	packet << player.client->getRemotePort();
//	packet << player.name;
//	packet << player.points;
//
//	std::cout << "Cliente: " << player.name << " con IP: " << ipRemoteAdrress << " y puerto: " << player.client->getRemotePort() << std::endl;
//	return packet;
//}
//
//void ServerPacketTypesManager::ReceivePacket(sf::Packet packet, sf::TcpSocket& client)
//{
//	PacketTypes packetType;
//
//	packet >> packetType;
//
//	std::cout << client.getRemoteAddress()->toString() << std::endl;
//
//	switch (packetType)
//	{
//	case PacketTypes::HANDSHAKE:
//		ReceiveHandshakePacket(packet);
//		break;
//	case PacketTypes::LOGIN:
//		ReceiveLoginPacket(packet, client);
//		break;
//	case PacketTypes::REGISTER:
//		ReceiveRegisterPacket(packet, client);
//		break;
//	case PacketTypes::LOBBY_CREATE:
//		ReceiveLobbyCreatePacket(packet, client);
//		break;
//	case PacketTypes::LOBBY_JOIN:
//		ReceiveLobbyJoinPacket(packet, client);
//		break;
//	case PacketTypes::RANKING:
//		ReceiveRankingPacket(packet, client);
//		break;
//	case PacketTypes::START_GAME:
//		ReceiveStartGamePacket(packet, client);
//		break;
//	case PacketTypes::END_GAME:
//		ReceiveEndGamePacket(packet);
//		break;
//	default:
//		std::cout << "No se ha identificado el tipo de packete" << std::endl;
//		break;
//	}
//
//	packet.clear();
//}
//
//void ServerPacketTypesManager::SendData(sf::TcpSocket& client, sf::Packet& packet)
//{
//	if (client.send(packet) == sf::Socket::Status::Done) {
//		std::cout << "Enviando paquete..." << std::endl;
//	}
//	else {
//		std::cerr << "Error al enviar el paquete" << std::endl;
//	}
//}
//
//void ServerPacketTypesManager::SendHandshake(sf::TcpSocket& client)
//{
//	sf::Packet packet;
//	packet << PacketTypes::HANDSHAKE << handshakeMessage;
//	SendData(client, packet);
//}
//
//void ServerPacketTypesManager::SendUpdatedPlayerCount(sf::TcpSocket& client, int playerCount)
//{
//	sf::Packet packet;
//	packet << PacketTypes::WAITING_ROOM_PLAYERS;
//	packet << playerCount;
//
//	SendData(client, packet);
//}
//
//void ServerPacketTypesManager::SendLoginResponse(sf::TcpSocket& client, bool success, const std::string& username)
//{
//	sf::Packet packet;
//	packet << PacketTypes::LOGIN;
//	packet << success;
//	packet << username;
//
//	SendData(client, packet);
//
//	std::cout << "Respuesta de login enviada" << std::endl;
//}
//
//void ServerPacketTypesManager::SendRegisterResponse(sf::TcpSocket& client, bool success, const std::string& username)
//{
//	sf::Packet packet;
//	packet << PacketTypes::REGISTER;
//	packet << success;
//	packet << username;
//
//	SendData(client, packet);
//
//	std::cout << "Respuesta de registro enviada" << std::endl;
//}
//
//void ServerPacketTypesManager::SendLobbyCreateResponse(sf::TcpSocket& client, bool success)
//{
//	sf::Packet packet;
//	packet << PacketTypes::LOBBY_CREATE;
//	packet << success;
//
//	SendData(client, packet);
//
//	std::cout << "Respuesta de creacion de lobby enviada" << std::endl;
//}
//
//void ServerPacketTypesManager::SendLobbyJoinResponse(sf::TcpSocket& client, bool success)
//{
//	sf::Packet packet;
//	packet << PacketTypes::LOBBY_JOIN;
//	packet << success;
//
//	SendData(client, packet);
//
//	std::cout << "Respuesta de join a lobby enviada" << std::endl;
//}
//
//void ServerPacketTypesManager::SendRankingPacket(sf::TcpSocket& client, std::vector<Database::RankingEntry>& rankings)
//{
//	sf::Packet packet;
//	packet << PacketTypes::RANKING;
//
//	packet << static_cast<int>(rankings.size());
//
//	for (const auto& entry : rankings) {
//		packet << entry.position;
//		packet << entry.userId;
//		packet << entry.username;
//		packet << entry.points;
//
//		
//		std::cout << entry.position << ". " << entry.userId << " - "
//			<< entry.username << " - "
//			<< entry.points << " puntos" << std::endl;
//	}
//
//	SendData(client, packet);
//	std::cout << "Ranking packet enviado con " << rankings.size() << " entradas." << std::endl;
//}
//
//void ServerPacketTypesManager::SendStartGamePacket(sf::TcpSocket& client, std::string lobbyId)
//{
//	sf::Packet packet;
//
//	packet << PacketTypes::START_GAME;
//
//	GameRoom* gameRoom = MM->GetGameInfo(lobbyId);
//
//	packet << gameRoom->GetPlayerAmount();
//
//	for (int i = 0; i < gameRoom->GetPlayerAmount(); i++)
//	{
//		Player player = *gameRoom->GetPlayer(i);
//
//		packet << player;
//	}
//
//	SendData(client, packet);
//}
//
//void ServerPacketTypesManager::ReceiveHandshakePacket(sf::Packet data)
//{
//	std::string receiveMesage;
//	data >> receiveMesage;
//
//	std::cout << "Mensaje enviado del cliente: " << receiveMesage << std::endl;
//}
//
//void ServerPacketTypesManager::ReceiveLoginPacket(sf::Packet data, sf::TcpSocket& client)
//{
//	std::string loginUsername;
//	std::string loginPassword;
//
//	data >> loginUsername;
//	data >> loginPassword;
//
//	int userId = 0;
//
//	bool correctLogin = DB->LoginUser(loginUsername, loginPassword, userId);
//
//	bool userAlreadyConnected = false;
//
//	if(correctLogin)
//		userAlreadyConnected = MM->CheckIfConnectedPlayerExists(&client, loginUsername, 15);
//
//	correctLogin = correctLogin && !userAlreadyConnected;
//
//	SendLoginResponse(client, correctLogin, loginUsername);
//	
//	if (correctLogin) {
//		MM->AddConnectedPlayer(&client, loginUsername, 15);
//	}
//}
//
//void ServerPacketTypesManager::ReceiveRegisterPacket(sf::Packet data, sf::TcpSocket& client)
//{
//	std::string registerUsername;
//	std::string registerPassword;
//
//	data >> registerUsername;
//	data >> registerPassword;
//
//	std::string passwordHash = bcrypt::generateHash(registerPassword);
//	
//	bool correctRegister = DB->RegisterUser(registerUsername, passwordHash);
//
//	SendRegisterResponse(client, correctRegister, registerUsername);
//
//	if (correctRegister) {
//		MM->AddConnectedPlayer(&client, registerUsername, 15);
//	}
//}
//
//void ServerPacketTypesManager::ReceiveLobbyCreatePacket(sf::Packet data, sf::TcpSocket& client)
//{
//	std::string lobbyID;
//
//	data >> lobbyID;
//
//	bool successfulLobbyCreation = MM->CreateWaitingRoom(lobbyID, &client);
//	 
//	if (successfulLobbyCreation) {
//		std::cout << "Lobby " << lobbyID << "creado exitosamente, pasando jugador a la sala de espera" << std::endl;
//	}
//	else {
//		std::cout << "El ID " << lobbyID << " ya esta en uso" << std::endl;
//	}
//
//	SendLobbyCreateResponse(client, successfulLobbyCreation);
//}
//
//void ServerPacketTypesManager::ReceiveLobbyJoinPacket(sf::Packet data, sf::TcpSocket& client)
//{
//	std::string lobbyID;
//
//	data >> lobbyID;
//
//	std::cout << "Servidor recibe join packet" << std::endl;
//	bool successfulLobbyJoin = MM->JoinWaitingRoom(lobbyID, &client);
//
//	if (successfulLobbyJoin) {
//		std::cout << "Jugador se ha unido a lobby con ID: " << lobbyID << std::endl;
//	}
//	else {
//		std::cout << "El lobby " << lobbyID << " esta lleno o no existe" << std::endl;
//	}
//
//	SendLobbyJoinResponse(client, successfulLobbyJoin);
//}
//
//void ServerPacketTypesManager::ReceiveRankingPacket(sf::Packet data, sf::TcpSocket& client)
//{
//	std::cout << "Recibida peticion de ranking" << std::endl;
//
//	std::vector<Database::RankingEntry> topRankings = DB->GetTop10Rankings();
//
//	SendRankingPacket(client, topRankings);
//}
//
//void ServerPacketTypesManager::ReceiveStartGamePacket(sf::Packet data, sf::TcpSocket& client)
//{
//	std::string lobbyID;
//
//	data >> lobbyID;
//
//	std::cout << "Recibida peticion para iniciar la partida de la sala: " << lobbyID << std::endl;
//
//	SendStartGamePacket(client, lobbyID);
//}
//
//void ServerPacketTypesManager::ReceiveEndGamePacket(sf::Packet data)
//{
//}
