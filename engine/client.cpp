#include "client.h"

int Client::m_connectedClients = 0;
int Client::m_counter = 0;

Client::Client()
{
}

int Client::connectedClients()
{
    return m_connectedClients;
}

void Client::markAsConnected()
{
    m_counter++;
    m_id = m_counter;
    m_connectedClients++;
}

int Client::id() const
{
    return m_id;
}

sf::TcpSocket &Client::socket()
{
    return m_socket;
}
