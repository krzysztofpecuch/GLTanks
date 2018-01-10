#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>

enum Action
{
    MoveUp,
    MoveLeft,
    MoveRight,
    MoveDown,
    Shoot,
    Stay
};

class Client
{
public:
    static int connectedClients();

    sf::TcpSocket& socket();

    void setId(int id);
    int id() const;

    void markAsConnected();
    void markAsDisonnected();

    Action currentAction() const;
    void setCurrentAction(Action currentAction);

private:
    sf::TcpSocket m_socket;
    static int m_connectedClients;
    int m_id = -1;
    Action m_currentAction = Stay;
};

#endif // CLIENT_H
