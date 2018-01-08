#include "operators.h"

Packet& operator >>(Packet& p, Bullet& b)
{
	return p >> b.x >> b.y >> b.turn;
}

Packet& operator >>(Packet& p, Player& pArray)
{
	return p >> pArray.ID >> pArray.x >> pArray.y >> pArray.turn;
}

Packet& operator <<(Packet& p, Bullet& b)
{
	return p << b.x << b.y << b.turn;
}

Packet& operator <<(Packet& p, Player& pArray)
{
	return p << pArray.ID << pArray.x << pArray.y << pArray.turn;
}