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
    void sendData();

    int connectedClientsCount() const;
private:
    void manageConnections();

    void acceptNewClients();
    void receiveData();
    void deleteDisconnectedClients();

    void end();

    Game& m_game;
    std::thread* m_listeningThread = nullptr;
    sf::TcpListener m_listener;
    std::vector<Client*> m_clients;
    sf::Packet m_packet;

    bool m_running = false;
};

#endif // SERVER_H
