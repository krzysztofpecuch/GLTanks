#include "server.h"
#include "game.h"

#include <iostream>
#include <chrono>

#include "accesslib/Bullet.h"
#include "accesslib/operators.h"


const int PORT = 55000;

Server::Server(Game &game) :
    m_game(game)
{
    std::cout << sf::IpAddress::getLocalAddress().toString() << std::endl;
}

Server::~Server()
{
    end();

    if (m_listeningThread)
        delete m_listeningThread;

    for (auto& client : m_clients)
    {
        delete client;
    }
}

void Server::run()
{
    m_listeningThread = new std::thread(&Server::manageConnections, this);
}

void Server::sendData(const std::map<int, Tank>& tanks)
{
	sf::Packet packetType;
	sf::Packet playersMapPacket;
	sf::Packet mapSizePacket;
	std::array<Player*, 4> players;

	const int* map = m_game.getMap().getTileMap();
	std::string textMap;
	
	for (int i = 0; i < m_game.getMap().getTileCount(); i++) {
		textMap += std::to_string(map[i]);
		if (i != 0 && (i%m_game.getMap().getSizeY() == 0)) {
			textMap += '\n';
		}
	}
	
	playersMapPacket << textMap;

	int index = 0;
	for (const auto& tank : tanks)
	{
		Player* player = new Player;
		player->ID = tank.first;
		player->x = tank.second.getPosition().x;
		player->y = tank.second.getPosition().y;
		player->turn = tank.second.getCurrentDir();
		players[index] = player;
		index++;
	}

	packetType << PACKET_TYPE::TYPE_MAP_PLAYERS;
	for (int i = 0; i < MAX_PLAYER_NUMBER; i++)
	{
		playersMapPacket << *players[i];
	}

	for (const auto& client : m_clients)
	{
		if (client->socket().send(packetType) == sf::Socket::Done) {
			if (client->socket().send(playersMapPacket) == sf::Socket::Done) {
				std::cout << "Wyslano pakiet z graczami do klienta " << client->id() << std::endl;
			}
		}
	}

	packetType.clear();
	packetType << PACKET_TYPE::TYPE_MAP_CREATOR;
	mapSizePacket << m_game.getMap().getSizeX() << m_game.getMap().getSizeY();

	for (const auto& client : m_clients) {
		if (client->socket().send(packetType) == sf::Socket::Done) {
			if (client->socket().send(mapSizePacket) == sf::Socket::Done) {
				std::cout << "Wyslano pakiet z rozmiarami mapy do klienta " << client->id() << std::endl;
			}
		}
	}

	mapSizePacket.clear();
	packetType.clear();
	playersMapPacket.clear();
}

int Server::connectedClientsCount() const
{
    return Client::connectedClients();
}

void Server::end()
{
    m_running = false;
    m_listeningThread->join();
    std::cout << "Thread joined!" << std::endl;
}

void Server::manageConnections()
{
    m_listener.listen(PORT);
    m_listener.setBlocking(false);

    m_running = true;

    while (m_running)
    {
		if (m_game.state == gameState::WAITING) {
			acceptNewClients();
		}
		else {
			receiveData();
		}
        deleteDisconnectedClients();
    }
}

void Server::acceptNewClients()
{
    Client* client = new Client();
    if (m_listener.accept(client->socket()) == sf::Socket::Done)
    {
        client->markAsConnected();

        std::cout << "Client with id " << client->id() << " connected to server" << std::endl;
        client->socket().setBlocking(false);

        sf::Packet packet;
        packet << client->id();

        client->socket().send(packet);

        m_game.addTank(client->id());

        m_clients.push_back(client);
		
		if (m_clients.size() == MAX_PLAYER_NUMBER) {
			m_game.state = gameState::RUNNING;
		}
    }
    else
    {
        delete client;
    }
}

void Server::receiveData()
{
    for (const auto& client : m_clients)
    {
        m_packet.clear();
        if (client->socket().receive(m_packet) == sf::Socket::Done)
        {
            int data;
            m_packet >> data;
            std::cout << "Data " << data << " received" << std::endl;
            m_game.moveTank(client->id(), data);
        }
    }
}

void Server::deleteDisconnectedClients()
{
    for (unsigned i = 0; i < m_clients.size(); ++i)
    {
        sf::Packet dummy;
        if (m_clients[i]->socket().receive(dummy) == sf::Socket::Disconnected)
        {
            std::cout << "Client with id " << m_clients[i]->id() << " disconnected from server" << std::endl;
            m_game.setMessageText("Waiting for " + std::to_string((4-Client::connectedClients())) + " more players");
            m_clients[i]->markAsDisconnected();
            m_game.deleteTank(m_clients[i]->id());

            m_clients.erase(m_clients.begin() + i);
            break;
        }
    }
}
