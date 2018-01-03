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
    void receiveData();
    void deleteDisconnectedClients();

    void end();

    Game& m_game;
    std::thread* m_listeningThread = nullptr;
    sf::TcpListener m_listener;
    std::vector<Client*> m_clients;
    sf::Packet m_packet;

    bool m_running = false;
	bool mapSizeSent = false;
};

#endif // SERVER_H
