#include "server.h"
#include "game.h"
#include "acceslib/commons.h"
#include "acceslib/operators.h"

#include <chrono>
#include <iostream>

const int PORT = 55000;

Server::Server(Game &game) :
    m_game(game)
{
    std::cout << sf::IpAddress::getLocalAddress().toString() << std::endl;
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

std::map<int, Action> Server::getActionsToPerform() const
{
    std::map<int, Action> actions;

    for (auto& client : m_clients)
    {
        int id = client->id();

        if (m_game.isTankInGame(id))
        {
            actions[id] = client->currentAction();
            client->setCurrentAction(Stay);
        }
    }

    return actions;
}

void Server::run()
{
    m_listeningThread = new std::thread(&Server::manageConnections, this);
}

void Server::sendData(const std::map<int, Tank>& tanks)
{
 //   std::cout << "sendData()" << std::endl;
    sf::Packet packetType;
    sf::Packet playersMapPacket;
    sf::Packet mapSizePacket;
    sf::Packet bulletPacket;
    std::array<Player*, 4> players;

    packetType << PACKET_TYPE::TYPE_MAP_CREATOR;
    mapSizePacket << m_game.getMap().getSizeX() << m_game.getMap().getSizeY();

    if (!m_mapSizeSent)
    {
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

        m_mapSizeSent = true;
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

    const std::vector<BulletGFX>& gameBullets = m_game.getBullets();

    bulletPacket << (int)gameBullets.size();
    for (int i = 0; i < gameBullets.size(); i++)
    {
        Bullet b;
        b.turn = gameBullets[i].getDirection();
        b.x = gameBullets[i].getTile().y;
        b.y = gameBullets[i].getTile().x;
        bulletPacket << b;
    }

	
 //   std::cout << "Before sending" << std::endl;
	for (const auto& client : m_clients)
	{
    //    std::cout << "client->id()" << client->id() << std::endl;
		if (client->socket().send(packetType) == sf::Socket::Done)
		{
			if (!client->socket().send(bulletPacket) == sf::Socket::Done)
			{
				std::cout << "Error sending bullet packet to " << client->id() << std::endl;
			}
		}
	}
  //  std::cout << "After sending" << std::endl;
	

	packetType.clear();
	bulletPacket.clear();

  //  std::cout << "end of sendData()" << std::endl;
}

void Server::sendDataMatchEnd(int winningId)
{
    sf::Packet packetType;
    sf::Packet packetWin;

    packetType << PACKET_TYPE::TYPE_WIN;

    for (const auto& client : m_clients)
    {
        if (client->id() == winningId)
        {
            packetWin << 1;
        }
        else
        {
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
    m_mapSizeSent = false;
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
        if (m_game.state == GameState::WAITING)
        {
            acceptNewClients();
        }

        //join and delete threads of disconnected clients
        for (auto& thread : m_clientsThreads)
        {
            if (m_disconnectedClientID == thread.first)
            {
                m_disconnectedClientID = -1;
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

        status = client->socket().receive(packet);

        if (status == sf::Socket::Done)
        {
            if (!m_game.isTankInGame(client->id()))
            {
                continue;
            }

            int action;
            packet >> action;
//                        std::cout << "Data " << action << " received" << std::endl;

            client->setCurrentAction(static_cast<Action>(action));
        }
        else if (status == sf::Socket::Disconnected)
        {
            std::cout << "Client with id " << client->id() << " disconnected from server" << std::endl;

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

            m_disconnectedClientID = client->id();
            delete client;
            m_clients.erase(m_clients.begin() + clientIndex);

            connected = false;
        }
    }
}
