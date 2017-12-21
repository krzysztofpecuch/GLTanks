#include "server.h"
#include "game.h"

#include <iostream>
#include <chrono>

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
        Client* client = new Client();
        if (m_listener.accept(client->socket()) == sf::Socket::Done)
        {
            client->markAsConnected();
            client->socket().setBlocking(false);

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

        for (unsigned i = 0; i < m_clients.size(); ++i)
        {
            sf::Packet dummy;
            if (m_clients[i]->socket().receive(dummy) == sf::Socket::Disconnected)
            {
                std::cout << "Client with id " << m_clients[i]->id() << " disconnected from server" << std::endl;
                m_game.deleteTank(m_clients[i]->id());
                m_clients.erase(m_clients.begin() + i);
                break;
            }
        }
    }
}
