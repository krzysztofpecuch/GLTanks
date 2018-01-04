#include "server.h"
#include "game.h"

#include <iostream>
#include <chrono>

#include "acceslib/Bullet.h"
#include "acceslib/operators.h"


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
    std::array<Player*, MAX_PLAYER_NUMBER> players;

	packetType << PACKET_TYPE::TYPE_MAP_CREATOR;
	mapSizePacket << m_game.getMap().getSizeX() << m_game.getMap().getSizeY();

	if (!mapSizeSent) {
		for (const auto& client : m_clients)
		{
			if (client->socket().send(packetType) == sf::Socket::Done)
			{
				if (client->socket().send(mapSizePacket) == sf::Socket::Done)
				{
					std::cout << "Map size sent to " << client->id() << std::endl;
				}
			}
		}
		mapSizeSent = true;
	}

	packetType.clear();
	mapSizePacket.clear();

	const int* map = m_game.getMap().getTileMap();
	std::string textMap;
	
    for (int i = 0; i < m_game.getMap().getTileCount(); i++)
    {
		textMap += std::to_string(map[i]);
	}
	
	playersMapPacket << textMap;

	int index = 0;
	for (const auto& tank : tanks)
	{
        Player player;
        player.ID = tank.first;
        player.x = tank.second.getPosition().x;
        player.y = tank.second.getPosition().y;
        player.turn = tank.second.getCurrentDirection();
        players[index] = &player;
		index++;
	}

	packetType << PACKET_TYPE::TYPE_MAP_PLAYERS;

	for (int i = 0; i < MAX_PLAYER_NUMBER; i++)
	{
		playersMapPacket << *players[i];
	}

	for (const auto& client : m_clients)
	{
        if (client->socket().send(packetType) == sf::Socket::Done)
        {
            if (client->socket().send(playersMapPacket) == sf::Socket::Done)
            {
				std::cout << "Map and players sent to " << client->id() << std::endl;
			}
		}
	}

	packetType.clear();
	playersMapPacket.clear();
}

void Server::sendDataMatchEnd(int winningId) {
	sf::Packet packetType;
	sf::Packet packetWin;

	packetType << PACKET_TYPE::TYPE_WIN;

	for (const auto& client : m_clients)
	{
		if (client->id() == winningId) {
			packetWin << 1;
		}
		else {
			packetWin << 0;
		}
		if (client->socket().send(packetType) == sf::Socket::Done)
		{
			if (client->socket().send(packetWin) == sf::Socket::Done)
			{
				std::cout << "Winning state sent to " << client->id() << std::endl;
			}
		}
		packetWin.clear();
	}
	resetServerFlags();
}

void Server::resetServerFlags()
{
	mapSizeSent = false;
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
        if (m_game.state == gameState::WAITING)
        {
			acceptNewClients();
		}
        else
        {
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
        if (Client::connectedClients() > 0)
        {
            bool idGranted = false;
            for (unsigned i = 0; i < m_clients.size(); ++i)
            {
                if (m_clients[i]->id() != i)
                {
                    client->setId(i);
                    idGranted = true;
                    break;
                }

                if (!idGranted)
                {
                    client->setId(Client::connectedClients());
                }
            }
        }
        else
        {
            client->setId(Client::connectedClients());
        }

        client->markAsConnected();

        std::cout << "Client with id " << client->id() << " connected to server" << std::endl;
        client->socket().setBlocking(false);

        sf::Packet packet;
        packet << client->id();

        client->socket().send(packet);

        m_game.addTank(client->id());

        m_clients.push_back(client);
		
        if (m_clients.size() == MAX_PLAYER_NUMBER)
        {
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
//            m_game.restart();
//            m_game.state = WAITING;
            std::cout << "Client with id " << m_clients[i]->id() << " disconnected from server" << std::endl;
            m_game.setMessageText("Waiting for " + std::to_string((MAX_PLAYER_NUMBER-Client::connectedClients())) + " more players");
            m_game.deleteTank(m_clients[i]->id());

            m_clients[i]->markAsDisonnected();
            //fix memory leak!!!
            m_clients.erase(m_clients.begin() + i);
            break;
        }
    }
}
