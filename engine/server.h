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

    void run();
    void sendData(const std::map<int, Tank>& tanks);
	void sendDataMatchEnd(int winningId);
	void resetServerFlags();
	void setSecondFlag();
    int connectedClientsCount() const;
    void close();

private:
    void manageConnections();
    void acceptNewClients();
    void receiveData(Client *client);

    Game& m_game;
    sf::TcpListener m_listener;
    std::thread* m_listeningThread = nullptr;
    std::map<int, std::thread*> m_clientsThreads;
    std::vector<Client*> m_clients;

	bool secondPassed = false;
	int m_serverSecondCounter;

    bool m_running = false;
    bool m_mapSizeSent = false;

    int m_disconnectedClientID = -1;
};

#endif // SERVER_H
