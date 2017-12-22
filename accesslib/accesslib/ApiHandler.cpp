#include "ApiHandler.h"

ApiHandler::ApiHandler() : bulletsSize(0)
{
	connectionID = -1;
	listeningMode = true;
	ts = nullptr;
	p = nullptr;
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

void ApiHandler::createMap(int sizeX, int sizeY)
{
	mapArray = new int*[sizeY];
	for (int i = 0; i < sizeY; i++)
	{
		mapArray[i] = new int[sizeX];
	}
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
	while (!listeningMode) {
		Packet move;
		move << action;
		if (ts->send(move) == Socket::Done) {
			listeningMode = true;
		}
	}
}

void ApiHandler::parsePacket(Packet* p, int type)
{
	if (type == PACKET_TYPE::TYPE_BULLET_COUNT)
	{
		*p >> bulletsSize;
		//create appropriate vector
	}

	if (type == PACKET_TYPE::TYPE_BULLETS)
	{
		// take all info about bullets and push to vector
	}

	if (type == PACKET_TYPE::TYPE_MAP_CREATOR)
	{
		*p >> mapSizeX >> mapSizeY;
		createMap(mapSizeX, mapSizeY);
	}

	if (type == PACKET_TYPE::TYPE_MAP_PLAYERS)
	{
		*p >> mapData >> *playersArr[0] >> *playersArr[1] >> *playersArr[2] >> *playersArr[3];
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