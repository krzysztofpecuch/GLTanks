#pragma once
struct Bullet
{
	int ID;
	int x;
	int y;
	int turn;
};

struct Players
{
	int ID;
	int x;
	int y;
	int turn;
};

enum PACKET_TYPE
{
	TYPE_BULLET_COUNT,
	TYPE_BULLETS,
	TYPE_MAP_PLAYERS
};