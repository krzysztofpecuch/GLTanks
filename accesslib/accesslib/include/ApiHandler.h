#ifndef APIHANDLER_H
#define APIHANDLER_H

#ifdef DLLSWITCH  

#define API __declspec(dllexport)   
#include <vector>
#include <array>
#include <iostream>
#include <thread>
#include "SFML\Network.hpp"
#include "commons.h"
#include "operators.h"

class ApiListener;

using namespace std;
using namespace sf;

#else
#define API __declspec(dllimport)
class TcpSocket;
class Packet;
struct Bullet;
struct Player;
struct Map;
enum PACKET_TYPE;
class ApiListener;

#include <array>
#include <string>
#include <vector>

using namespace std;
#endif  

class API ApiHandler {
public:
	ApiHandler();
	~ApiHandler();

	void connect(const char* serverAddr);
	TcpSocket* getSocket();

	void sendAction(int action);
	void forceSendAction(int action);

	bool getState();
	int getConnectionID();

	//void listenForPacket();
	void parsePacket(Packet* p, int type);

	int** mapArray;
private:
	TcpSocket *ts;
	ApiListener *al;
	
	Packet *p;
	int connectionID;
	int actionToTake;
	bool listeningMode;

	bool packetBullets;
	bool packetMap;
	bool packetMapPlayers;
	
	array<Player*, 4> playersArr;
	
	std::string mapData;
	int mapSizeX;
	int mapSizeY;
	void createMap(int mapSizeX, int mapSizeY);
	void updateMap(string data);

	vector<Bullet*> bullets;
	int bulletsSize;
	
	
	void sendPacket(Packet* p);
};
#endif // !PACKETHANDLER_H
