#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>

class Server
{
public:
    Server();
    void run();
private:
    void listenForClients();

    sf::TcpListener m_listener;
    sf::TcpSocket m_client;
};

#endif // SERVER_H
