#include "server.h"

#include <iostream>

const int PORT = 55000;

Server::Server()
{
    std::cout << sf::IpAddress::getLocalAddress().toString() << std::endl;
}

void Server::run()
{
    m_listener.listen(PORT);

//    std::thread listeningThread(&Server::listenForClients, this);
//    listenForClients();
//    listeningThread.join();


    sf::Thread thread(&Server::listenForClients, this);

    thread.launch();

    std::cout << "Thread joined" << std::endl;

}

void Server::listenForClients()
{
    m_listener.accept(m_client);
    std::cout << "New client connected: " << m_client.getRemoteAddress() << std::endl;
}
