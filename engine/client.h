#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>

class Client
{
public:
    static int connectedClients();

    sf::TcpSocket& socket();

    void setId(int id);
    int id() const;

    void markAsConnected();
    void markAsDisonnected();

private:
    sf::TcpSocket m_socket;

    static int m_connectedClients;

    int m_id = -1;
};

#endif // CLIENT_H
