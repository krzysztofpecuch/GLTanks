#ifndef APIHANDLER_H
#define APIHANDLER_H

#ifdef DLLSWITCH  

#define API __declspec(dllexport)   
#include "SFML\Network.hpp"

using namespace std;
using namespace sf;

#else
#define API __declspec(dllimport)
#endif  

class API ApiHandler {
public:
	ApiHandler();
	~ApiHandler();

	void connect();
	void listenForPacket();
	void sendAction(int action);
private:
	TcpSocket ts;
	Packet p;
	int actionToTake;
	bool readyForAction;
	
	void parsePacket();
	
	void sendPacket();
};
#endif // !PACKETHANDLER_H
