#include "ApiHandler.h"

ApiHandler::ApiHandler() : bulletsSize(0)
{
	readyForAction = false;
	ts = nullptr;
	p = nullptr;
//	connect();
}


ApiHandler::~ApiHandler()
{
	for (int i = 0; i < mapSizeY; i++)
	{
		delete[] mapArray[i];
	}
	

	delete[] mapArray;

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

void ApiHandler::createMap()
{
	*p >> mapSizeX >> mapSizeY;
	mapArray = new int*[mapSizeY];
	for (int i = 0; i < mapSizeY; i++)
	{
		mapArray[i] = new int[mapSizeX];
	}
}

void ApiHandler::listenForPacket() {
	while (!readyForAction) {
		if (ts->receive(*p) == Socket::Done) {
//			readyForAction = true;
//			parsePacket();
//			p.clear();
		}
	}
}

void ApiHandler::sendAction(int action) {
	
	while (readyForAction) {
		Packet move;
		move << action;
		if (ts->send(move) == Socket::Done) {
			readyForAction = false;
		}
	}
}

void ApiHandler::parsePacket(Packet* p, PACKET_TYPE type)
{
	if (type == TYPE_BULLET_COUNT)
	{
		*p >> bulletsSize;
		//create appropriate vector
	}

	if (type == TYPE_BULLETS)
	{
		// take all info about bullets and push to vector
	}

	if (type == TYPE_MAP_PLAYERS)
	{
		*p >> *map >> *playersArr[0] >> *playersArr[1] >> *playersArr[2] >> *playersArr[3];
	}
}

// for parsePacket, to take bullets
//{
//	Packet p;
//	for () {
//		Bullet *b = new Bullet();
//		p >> b;
//		vec.push_back(b);
//		
//	}
//}