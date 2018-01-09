#include "server.h"
#include "game.h"

#include <iostream>
#include <chrono>

#include "acceslib/commons.h"
#include "acceslib/operators.h"


const int PORT = 55000;


Server::Server(Game &game) :
    m_game(game)
{
    std::cout << sf::IpAddress::getLocalAddress().toString() << std::endl;
}

Server::~Server()
{

}

void Server::close()
{
    m_running = false;

    for (const auto& client : m_clients)
    {
        client->socket().disconnect();
    }

    for (auto& thread : m_clientsThreads)
    {
        thread.second->join();
        delete thread.second;
    }

    m_listeningThread->join();

    if (m_listeningThread)
        delete m_listeningThread;

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
    sf::Packet bulletPacket;
    std::array<Player*, 4> players;

    packetType << PACKET_TYPE::TYPE_MAP_CREATOR;
    mapSizePacket << m_game.getMap().getSizeX() << m_game.getMap().getSizeY();

    if (!mapSizeSent) {
        for (const auto& client : m_clients)
        {
            if (client->socket().send(packetType) == sf::Socket::Done)
            {
                if (!client->socket().send(mapSizePacket) == sf::Socket::Done)
                {
					std::cout << "Error sending map size packet" << client->id() << std::endl;
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
	std::vector<Player*> playerData;
    for (const auto& tank : tanks)
    {
		Player *player = new Player;
		playerData.push_back(player);
        player->ID = tank.first;
        player->x = tank.second.getPosition().x;
        player->y = tank.second.getPosition().y;
        player->turn = tank.second.getCurrentDirection();
        players[index] = player;
        index++;
    }
    for (int i = index; i < 4; i++)
    {
		Player *player = new Player;
		playerData.push_back(player);
        player->ID = -1;
        player->x = -1;
        player->y = -1;
		player->turn = -1;
        players[i] = player;
    }

    packetType << PACKET_TYPE::TYPE_MAP_PLAYERS;

    for (int i = 0; i < 4; i++)
    {
        playersMapPacket << *players[i];
    }

    for (const auto& client : m_clients)
    {
        if (client->socket().send(packetType) == sf::Socket::Done)
        {
            if (!client->socket().send(playersMapPacket) == sf::Socket::Done)
            {
                std::cout << "Error sending map and players packet to " << client->id() << std::endl;
            }
        }
    }

	for (int i = 0; i < playerData.size(); i++)
	{
		delete playerData[i];
	}

	playerData.clear();

    packetType.clear();
    playersMapPacket.clear();

    packetType << PACKET_TYPE::TYPE_BULLETS;

	bulletPacket << (int)m_game.getBullets().size();
	for (int i = 0; i < m_game.getBullets().size(); i++)
	{
		Bullet b;
		b.turn = m_game.getBullets()[i].getDirection();
		b.x = m_game.getBullets()[i].getTile().y;
		b.y = m_game.getBullets()[i].getTile().x;
		bulletPacket << b;
	}

	
	for (const auto& client : m_clients)
	{
		if (client->socket().send(packetType) == sf::Socket::Done)
		{
			if (!client->socket().send(bulletPacket) == sf::Socket::Done)
			{
				std::cout << "Error sending bullet packet to " << client->id() << std::endl;
			}
		}
	}
	

	packetType.clear();
	bulletPacket.clear();
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

void Server::setSecondFlag()
{
	secondPassed = true;
}

int Server::connectedClientsCount() const
{
    return Client::connectedClients();
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

        //join and delete threads of disconnected clients
        for (auto& thread : m_clientsThreads)
        {
            if (disconnectedClientID == thread.first)
            {
                disconnectedClientID = -1;
                thread.second->join();
                delete thread.second;

                m_clientsThreads.erase(m_clientsThreads.find(thread.first));
                break;
            }
        }
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

        m_clientsThreads[client->id()] = new std::thread(&Server::receiveData, this, client);

        //        std::cout << "Client with id " << client->id() << " connected to server" << std::endl;

        sf::Packet packet;
        packet << client->id();

        client->socket().send(packet);

        m_game.addTank(client->id());

        m_clients.push_back(client);
    }
    else
    {
        delete client;
    }
}

void Server::receiveData(Client* client)
{
    bool connected = true;
	
	sf::Socket::Status status;

    while (connected)
    {
        if (!m_running)
            return;

        sf::Packet packet;

		if (secondPassed)
		{
			status = client->socket().receive(packet);
			secondPassed = false;
		}
		else 
		{
			continue;
		}

        if (status == sf::Socket::Done)
        {
            enum Action
            {
                MoveUp,
                MoveLeft,
                MoveRight,
                MoveDown,
                Shoot
            };

            int action;
            packet >> action;
            //            std::cout << "Data " << action << " received" << std::endl;

            if (!m_game.isTankInGame(client->id()))
            {
                std::cout << "tank not in game!" << std::endl;
                continue;
            }

            if (action == Shoot)
                m_game.performTankShoot(client->id());
            else
                m_game.moveTank(client->id(), action);
        }
        else if (status == sf::Socket::Disconnected)
        {
            std::cout << "Client with id " << client->id() << " disconnected from server" << std::endl;

            //ogarnac
            if (m_game.tanksCount() > 0)
                m_game.deleteTank(client->id());

            client->markAsDisonnected();

            //find client's index to remove client from vector
            int clientIndex = -1;
            for (unsigned i = 0; i < m_clients.size(); ++i)
            {
                if (m_clients[i]->id() == client->id())
                {
                    clientIndex = i;
                    break;
                }
            }

            disconnectedClientID = client->id();
            delete client;
            m_clients.erase(m_clients.begin() + clientIndex);

            connected = false;
        }
    }
}
