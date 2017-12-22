#include "operators.h"

Packet& operator >>(Packet& p, Bullet& b)
{
	return p >> b.ID >> b.x >> b.y >> b.turn;
}

Packet& operator >>(Packet& p, Player& pArray)
{
	return p >> pArray.ID >> pArray.x >> pArray.y >> pArray.turn;
}

//Packet& operator >>(Packet& p, Map& mArray)
//{
//	return p >> mArray.sizeX >> mArray.sizeY >> mArray.mapData;
//}

Packet& operator <<(Packet& p, Bullet& b)
{
	return p << b.ID << b.x << b.y << b.turn;
}

Packet& operator <<(Packet& p, Player& pArray)
{
	return p << pArray.ID << pArray.x << pArray.y << pArray.turn;
}

//Packet& operator <<(Packet& p, Map& mArray)
//{
//	return p << mArray.sizeX << mArray.sizeY << mArray.mapData;
//}