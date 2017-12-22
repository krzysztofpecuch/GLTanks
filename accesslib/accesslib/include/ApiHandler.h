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
struct Players;
struct Map;
enum PACKET_TYPE;

#include <array>
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
	bool getState();
	int getConnectionID();
	int** mapArray;
private:
	TcpSocket *ts;
	Packet *p;
	int connectionID;
	int actionToTake;
	bool listeningMode;
	
	array<Player*, 4> playersArr;
	
	Map* map;
	int mapSizeX;
	int mapSizeY;
	void createMap();

	vector<Bullet> *bullets;
	int bulletsSize;
	
	void parsePacket(Packet* p, PACKET_TYPE type);
	void sendPacket(Packet* p);
};
#endif // !PACKETHANDLER_H
