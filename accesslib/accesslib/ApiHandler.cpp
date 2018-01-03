#include "ApiHandler.h"

ApiHandler::ApiHandler()
{
	connectionID = -1;
	listeningMode = true;
	ts = nullptr;
	p = nullptr;

	for (int i = 0; i < 4; i++)
	{
		playersArr[i] = new Player;
	}

	bulletsSize = 0;
	packetBullets = false;
	packetMap = false;
	packetMapPlayers = false;
	allPacketsReceived = false;
}

ApiHandler::~ApiHandler()
{
	for (int i = 0; i < mapSizeY; i++)
	{
		delete[] mapArray[i];
	}
	delete[] mapArray;

	bullets.clear();

	if (ts != nullptr)
		delete ts;
	if (p != nullptr)
		delete p;
}

void ApiHandler::connect(const char* serverAddr) {
	ts = new TcpSocket();
	if (ts->connect(serverAddr, 55000) == sf::Socket::Done)
	{
		p = new Packet();
		if (ts->receive(*p) == sf::Socket::Done) {
			*p >> connectionID;
			cout << "Connected to server at " << serverAddr << " and my ID is " << connectionID << endl;
			p->clear();
		}		
	}
}

void ApiHandler::createMap(int sizeX, int sizeY)
{
	mapArray = new int*[sizeY];
	for (int i = 0; i < sizeY; i++)
	{
		mapArray[i] = new int[sizeX];
	}
}

void ApiHandler::updateMap(string data)
{
	int* arr = new int[data.length()];

	for (int i = 0; i < data.length(); i++)
	{
		arr[i] = data[i] - '0';
	}
	for (int i = 0; i < mapSizeY; i++)
	{
		for (int j = 0; j < mapSizeX; j++)
		{
			mapArray[i][j] = arr[i*mapSizeX + j];
		}
	}

	delete[] arr;
}

void ApiHandler::listenForPacket() {
	int msgType;
	while (listeningMode) {
		msgType = -1;
		if (ts->receive(*p) == Socket::Done) {
			*p >> msgType;
			if (p->endOfPacket()) {
				if (ts->receive(*p) == Socket::Done) {
					parsePacket(p, msgType);
				}
			}
		}
	}
}

void ApiHandler::sendAction(int action) {
	if (!listeningMode) {
		Packet movementInformation;
		movementInformation << action;
		if (ts->send(movementInformation) == Socket::Done) {
			cout << "Sent move " << action << endl;
			listeningMode = true;
			allPacketsReceived = false;
		}
	}	
}

void ApiHandler::forceSendAction(int action) {
	Packet movementInformation;
	movementInformation << action;
	if (ts->send(movementInformation) == Socket::Done) {
		cout << "Sent move " << action << endl;
	}
	movementInformation.clear();
}

void ApiHandler::parsePacket(Packet* p, int type)
{
	if (type == PACKET_TYPE::TYPE_WIN) {
		int win;
		*p >> win;
		if (win) {
			cout << "You won!" << endl;
		}
		else {
			cout << "You lost!" << endl;
		}
		allPacketsReceived = false;
		listeningMode = true;
		packetBullets = false;
		packetMap = false;
		packetMapPlayers = false;
		return;
	}

	if (type == PACKET_TYPE::TYPE_BULLETS)
	{
		*p >> bulletsSize;

		bullets.clear();
		Bullet b = {};
		for (int i = 0; i < bulletsSize; i++)
		{
			*p >> b;
			bullets.push_back(&b);
		}

		packetBullets = true;
	}

	if (type == PACKET_TYPE::TYPE_MAP_CREATOR)
	{
		*p >> mapSizeX >> mapSizeY;
		createMap(mapSizeX, mapSizeY);

		packetMap = true;
	}

	if (type == PACKET_TYPE::TYPE_MAP_PLAYERS)
	{
		*p >> mapData;
		for (int i = 0; i < 2; i++)
		{
			*p >> *playersArr[i];
		}
		updateMap(mapData);
		packetMapPlayers = true;
	}

	if (!allPacketsReceived && packetMap && packetMapPlayers) //&& packetBullets
	{
		allPacketsReceived = true;
		listeningMode = false;
		packetBullets = false;
		packetMapPlayers = false;
	}
}