#include "client.h"

int Client::m_connectedClients = 0;

int Client::connectedClients()
{
    return m_connectedClients;
}

int Client::id() const
{
    return m_id;
}

void Client::markAsConnected()
{
    m_connectedClients++;
}

void Client::markAsDisonnected()
{
    m_connectedClients--;
}

Action Client::currentAction() const
{
    return m_currentAction;
}

void Client::setCurrentAction(Action currentAction)
{
    m_currentAction = currentAction;
}

void Client::setId(int id)
{
    m_id = id;
}

sf::TcpSocket &Client::socket()
{
    return m_socket;
}
