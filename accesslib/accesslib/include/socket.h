#ifndef _SOCKET_H
#define _SOCKET_H

#ifdef DLLSWITCH  
#define API __declspec(dllexport)

#include "SFML\Network.hpp"

using namespace std;
using namespace sf;

#else

#define API __declspec(dllimport)
#include <vector>
using namespace std;

#endif  

class API ConnectSocket {
public:
	ConnectSocket();
	void connect();
	int ReceiveData();
private:
	sf::TcpSocket *mySocket;
};

#endif