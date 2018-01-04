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
}

ApiHandler::~ApiHandler()
{
	for (int i = 0; i < mapSizeX; i++)
	{
		delete[] mapArray[i];
	}
	delete[] mapArray;

	for (int i = 0; i < 4; i++)
	{
		delete playersArr[i];
	}

	bullets.clear();

	delete ts;
	ts = nullptr;

	delete p;
	p = nullptr;
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
	mapArray = new int*[sizeX];
	for (int i = 0; i < sizeX; i++)
	{
		mapArray[i] = new int[sizeY];
	}
}

void ApiHandler::updateMap(string data)
{
	int* arr = new int[data.length()];

	for (int i = 0; i < data.length(); i++)
	{
		arr[i] = data[i] - '0';
	}
	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			mapArray[i][j] = arr[i*mapSizeY + j];
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
				p->clear();
				if (ts->receive(*p) == Socket::Done) {
					parsePacket(p, msgType);
				}
			}
			p->clear();
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
			packetBullets = false;
			packetMapPlayers = false;
		}
	}	
}

void ApiHandler::forceSendAction(int action) {
	Packet movementInformation;
	movementInformation << action;
	if (ts->send(movementInformation) == Socket::Done) {
		cout << "Sent move " << action << endl;
		listeningMode = true;
		packetBullets = false;
		packetMapPlayers = false;
	}
	movementInformation.clear();
}

void ApiHandler::parsePacket(Packet* p, int type)
{
	if (type == PACKET_TYPE::TYPE_WIN) {
		int win;
		*p >> win;
		if (win) {
			cout << "You won! Class state reset." << endl;
		}
		else {
			cout << "Game ended. Class state reset." << endl;
		}

		for (int i = 0; i < mapSizeX; i++)
		{
			delete[] mapArray[i];
		}
		delete[] mapArray;

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

		cout << "Recieved bullet packet" << endl;
	}

	if (type == PACKET_TYPE::TYPE_MAP_CREATOR && !packetMap)
	{
		*p >> mapSizeX >> mapSizeY;
		createMap(mapSizeX, mapSizeY);

		packetMap = true;

		cout << "Recieved map size packet" << endl;
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

		cout << "Recieved map and player data packet" << endl;
	}

	if (packetMap && packetMapPlayers) //&& packetBullets
	{
		listeningMode = false;
	}
}