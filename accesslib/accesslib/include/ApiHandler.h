#ifndef APIHANDLER_H
#define APIHANDLER_H

#ifdef DLLSWITCH  

#define API __declspec(dllexport)   
#include <vector>
#include <array>
#include <iostream>
#include "SFML\Network.hpp"
#include "Bullet.h"
#include "operators.h"

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
	void listenForPacket();
	void sendAction(int action);
	void forceSendAction(int action);
	bool getState();
	int getConnectionID();
	int** mapArray;
private:
	TcpSocket *ts;
	Packet *p;
	int connectionID;
	int actionToTake;
	bool listeningMode;

	bool packetBullets;
	bool packetMap;
	bool packetMapPlayers;
	bool allPacketsReceived;
	
	array<Player*, 4> playersArr;
	
	std::string mapData;
	int mapSizeX;
	int mapSizeY;
	void createMap(int mapSizeX, int mapSizeY);

	vector<Bullet> *bullets;
	int bulletsSize;
	
	void parsePacket(Packet* p, int type);
	void sendPacket(Packet* p);
};
#endif // !PACKETHANDLER_H
