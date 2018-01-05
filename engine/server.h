#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>

#include <thread>
#include <map>
#include <vector>
#include <array>

#include "client.h"

class Game;
class Tank;

class Server
{
public:
    Server(Game& game);
    ~Server();

    void run();
    void sendData(const std::map<int, Tank>& tanks);
	void sendDataMatchEnd(int winningId);
	void resetServerFlags();
    int connectedClientsCount() const;
private:
    void manageConnections();

    void acceptNewClients();
    void receiveData(Client *client);

    Game& m_game;
    std::thread* m_listeningThread = nullptr;
//    std::vector<std::thread*> m_clientsThreads;
    std::map<int, std::thread*> m_clientsThreads;
    sf::TcpListener m_listener;
    std::vector<Client*> m_clients;
    sf::Packet m_packet;

    bool m_running = false;
	bool mapSizeSent = false;

    int disconnectedClientID = -1;
};

#endif // SERVER_H
