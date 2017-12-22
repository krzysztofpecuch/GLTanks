#pragma once
#include "SFML\Network.hpp"
#include "Bullet.h"
using namespace sf;

Packet& operator >>(Packet& p, Bullet& bullet);
Packet& operator >>(Packet& p, Player& pArray);
Packet& operator >>(Packet& p, Map& mArray);
Packet& operator <<(Packet& p, Bullet& b);
Packet& operator <<(Packet& p, Player& pArray);
Packet& operator <<(Packet& p, Map& mArray);