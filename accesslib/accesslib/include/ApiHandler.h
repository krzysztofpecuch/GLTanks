#ifndef APIHANDLER_H
#define APIHANDLER_H

#ifdef DLLSWITCH  

#define API __declspec(dllexport)   
#include <vector>
#include <array>
#include <iostream>
#include "SFML\Network.hpp"
#include "Bullet.h"

using namespace std;
using namespace sf;

#else
#define API __declspec(dllimport)
class TcpSocket;
class Packet;
struct Bullet;
struct Players;
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
	void createMap();
	void listenForPacket();
	void sendAction(int action);
	int** mapArray;
private:
	TcpSocket *ts;
	Packet *p;
	int connectionID;
	int actionToTake;
	bool readyForAction;
	vector<Bullet*> bullets;
	array<Players*, 4> playersarr;
	int mapSizeX;
	int mapSizeY;
	int bulletsSize;
	
	void parsePacket(Packet p, PACKET_TYPE type);
	void sendPacket(Packet p);
};

Packet& operator >>(Packet& p, Bullet& bullet);
Packet& operator >>(Packet& p, Players& pArray);
Packet& operator <<(Packet& p, Bullet& b);
Packet& operator <<(Packet& p, Players& pArray);
#endif // !PACKETHANDLER_H
