#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>

#include <thread>
#include <map>
#include <vector>

#include "client.h"

class Server
{
public:
    Server();
    ~Server();

    void run();
private:
    void manageConnections();
    void end();

    std::thread* m_listeningThread = nullptr;
    sf::TcpListener m_listener;
//    sf::TcpSocket m_client;
    std::vector<Client*> m_clients; // last client is always not connected, added in case of new connection incoming

//    std::map<int, Client> m_clients;

    int m_connectedClients = 0;

    bool m_running = false;
};

#endif // SERVER_H
