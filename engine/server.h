#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>

#include <thread>
#include <map>
#include <vector>

#include "client.h"

class Game;

class Server
{
public:
    Server(Game& game);
    ~Server();

    void run();
private:
    void manageConnections();
    void end();

    std::thread* m_listeningThread = nullptr;
    sf::TcpListener m_listener;
    std::vector<Client*> m_clients;
    Game& m_game;

    int m_connectedClients = 0;

    bool m_running = false;
};

#endif // SERVER_H
