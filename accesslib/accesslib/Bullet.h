#pragma once
typedef struct Bullet
{
	int ID;
	int x;
	int y;
	int turn;
} Bullet;

typedef struct Player
{
	int ID;
	int x;
	int y;
	int turn;
} Player;

enum PACKET_TYPE
{
	TYPE_BULLET_COUNT,
	TYPE_BULLETS,
	TYPE_MAP_CREATOR,
	TYPE_MAP_PLAYERS
};