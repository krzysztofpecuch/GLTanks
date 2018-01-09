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
#define MAX_PLAYER_NUMBER 4

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

class API ApiHandler
{
public:
	ApiHandler();
	~ApiHandler();

	void connect(const char* serverAddr);
	TcpSocket* getSocket();

	void sendAction(int action);
	void forceSendAction(int action);

	bool getState();
	int getConnectionID();

	void parsePacket(Packet* p, int type);

	void setReadyFlag();

	int** getMap();
	int** getBullets();
	int getBulletsCount();
	int** getPlayers();

private:
	void createMap(int mapSizeX, int mapSizeY);
	void updateMap(string data);

	TcpSocket *ts;
	ApiListener *al;
	Packet *p;

	int connectionID;
	bool listeningMode;

	int** mapArray;
	int** playerArr;
	int** bulArray;
	array<Player*, 4> playersArray;

	bool packetBullets;
	bool packetMap;
	bool packetMapPlayers;
	
	std::string mapData;
	int mapSizeX;
	int mapSizeY;

	vector<Bullet*> bullets;
	int bulletsSize;
	int savedBulletSize;
};
#endif // !PACKETHANDLER_H
