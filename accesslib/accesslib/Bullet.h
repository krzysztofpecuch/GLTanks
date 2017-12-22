#pragma once
typedef struct Bullet
{
	int ID;
	int x;
	int y;
	int turn;
} Bullet;

typedef struct Players
{
	int ID;
	int x;
	int y;
	int turn;
} Players;

typedef struct Map
{
	int sizeX;
	int sizeY;
	std::string mapData;
} Map;

enum PACKET_TYPE
{
	TYPE_BULLET_COUNT,
	TYPE_BULLETS,
	TYPE_MAP_PLAYERS
};