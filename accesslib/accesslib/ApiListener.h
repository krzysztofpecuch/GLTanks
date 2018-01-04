#ifndef APILISTENER_H
#define APILISTENER_H

#include <thread>
#include <chrono>
#include "SFML\Network.hpp"

class ApiHandler;

class ApiListener
{
public:
	ApiListener(ApiHandler &apihandler);
	~ApiListener();
private:
	ApiHandler& ah;
	std::thread* listeningThread;
	sf::Packet *p;
	void listen();
	bool startWait;
};


#endif