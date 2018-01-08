#pragma once
typedef struct Bullet
{
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
	TYPE_WIN,
	TYPE_BULLETS,
	TYPE_MAP_CREATOR,
	TYPE_MAP_PLAYERS
};