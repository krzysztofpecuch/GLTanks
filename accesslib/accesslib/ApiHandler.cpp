#include "ApiHandler.h"

ApiHandler::ApiHandler() : bulletsSize(0)
{
	readyForAction = false;
//	connect();
}


ApiHandler::~ApiHandler()
{
	for (int i = 0; i < mapSizeY; i++)
	{
		delete[] mapArray[i];
	}
	delete(ts);

	delete[] mapArray;

	delete p;
}

void ApiHandler::connect(const char* serverAddr) {
	ts = new TcpSocket();
	if (ts->connect(serverAddr, 55000) == sf::Socket::Done)
	{
		std::string message = "Hi, I am a client";
		ts->send(message.c_str(), message.size() + 1);
		p = new Packet();
	}
	createMap();
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
			readyForAction = true;
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

void ApiHandler::parsePacket(Packet p, PACKET_TYPE type)
{
	if (type == TYPE_BULLET_COUNT)
	{
		p >> bulletsSize;
		//create appropriate vector
	}

	if (type == TYPE_BULLETS)
	{
		// take all info about bullets and push to vector
	}

	if (type == TYPE_MAP_PLAYERS)
	{
		p >> **mapArray >> playersarr[0] >> playersarr[1] >> playersarr[2] >> playersarr[3];
	}
}

Packet& operator >>(Packet& p, Bullet& b)
{
	return p >> b.ID >> b.x >> b.y >> b.turn;
}

Packet& operator >>(Packet& p, Players& pArray)
{
	return p >> pArray.ID >> pArray.x >> pArray.y >> pArray.turn;
}

Packet& operator <<(Packet& p, Bullet& b)
{
	return p << b.ID << b.x << b.y << b.turn;
}

Packet& operator <<(Packet& p, Players& pArray)
{
	return p << pArray.ID << pArray.x << pArray.y << pArray.turn;
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