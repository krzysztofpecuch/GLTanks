#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>

class Client
{
public:
    Client();

    static int connectedClients();

    sf::TcpSocket& socket();

    void markAsConnected();
    void markAsDisconnected();
    int id() const;


private:
    sf::TcpSocket m_socket;

    static int m_connectedClients;
    static int m_counter;

    int m_id;
};

#endif // CLIENT_H
