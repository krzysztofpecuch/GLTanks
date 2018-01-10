#include "ApiHandler.h"
#include "ApiListener.h"

ApiHandler::ApiHandler()
{
	ts = nullptr;
	al = nullptr;
	p = nullptr;

	connectionID = -1;
	listeningMode = true;

	mapArray = nullptr;

	bulArray = nullptr;

	playerArr = new int*[MAX_PLAYER_NUMBER];
	for (int i = 0; i < MAX_PLAYER_NUMBER; i++)
	{
		playerArr[i] = new int[MAX_PLAYER_NUMBER];
	}

	for (int i = 0; i < MAX_PLAYER_NUMBER; i++)
	{
		playersArray[i] = new Player;
	}

	packetBullets = false;
	packetMap = false;
	packetMapPlayers = false;

	mapSizeX = 0;
	mapSizeY = 0;

	bulletsSize = 0;
}

ApiHandler::~ApiHandler()
{
	delete ts;
	ts = nullptr;

	delete al;
	al = nullptr;

	delete p;
	p = nullptr;

	for (int i = 0; i < mapSizeX; i++)
	{
		delete[] mapArray[i];
	}
	delete[] mapArray;

	for (int i = 0; i < MAX_PLAYER_NUMBER; i++)
	{
		delete[] playerArr[i];
	}
	delete[] playerArr;

	for (int i = 0; i < MAX_PLAYER_NUMBER; i++)
	{
		delete playersArray[i];
	}
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i] != nullptr)
		{
			delete bullets[i];
			bullets[i] = nullptr;
		}
	}
	bullets.clear();
}

void ApiHandler::connect(const char* serverAddr)
{
	ts = new TcpSocket();
	if (ts->connect(serverAddr, 55000) == sf::Socket::Done)
	{
		p = new Packet();
		if (ts->receive(*p) == sf::Socket::Done)
		{
			*p >> connectionID;
			cout << "Connected to server at " << serverAddr << " and my ID is " << connectionID << endl;
			p->clear();
			al = new ApiListener(*this);
		}		
	}
}

TcpSocket* ApiHandler::getSocket()
{
	return ts;
}

void ApiHandler::createMap(int sizeX, int sizeY)
{
	mapArray = new int*[sizeX];
	for (int i = 0; i < sizeX; i++)
	{
		mapArray[i] = new int[sizeY];
	}
}

void ApiHandler::updateMap(string data)
{
	int* arr = new int[data.length()];

	for (int i = 0; i < data.length(); i++)
	{
		arr[i] = data[i] - '0';
	}

	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			mapArray[i][j] = arr[i*mapSizeY + j];
		}
	}

	delete[] arr;
}

void ApiHandler::sendAction(int action)
{
	if (!listeningMode)
	{
		Packet movementInformation;
		movementInformation << action;
		if (ts->send(movementInformation) == Socket::Done)
		{
			listeningMode = true;
			packetBullets = false;
			packetMapPlayers = false;
		}
	}	
}

void ApiHandler::forceSendAction(int action)
{
	Packet movementInformation;
	movementInformation << action;
	if (!ts->send(movementInformation) == Socket::Done)
	{
		cout << "Error during send" << endl;
	}
	movementInformation.clear();
}

bool ApiHandler::getState()
{
	return listeningMode;
}

int ApiHandler::getConnectionID()
{
	return connectionID;
}

void ApiHandler::parsePacket(Packet* p, int type)
{
	if (type == PACKET_TYPE::TYPE_WIN)
	{
		int win;
		*p >> win;

		if (win)
		{
			cout << "You won! Class state reset." << endl;
		}
		else
		{
			cout << "Game ended. Class state reset." << endl;
		}

		if (mapArray != nullptr)
		{
			for (int i = 0; i < mapSizeX; i++)
			{
				delete[] mapArray[i];
			}
			delete[] mapArray;
		}
		
		listeningMode = true;
		packetBullets = false;
		packetMap = false;
		packetMapPlayers = false;
	}
	else if (type == PACKET_TYPE::TYPE_BULLETS)
	{
		*p >> bulletsSize;

		for (int i = 0; i < bulletsSize; i++)
		{
			Bullet *b = new Bullet;
			*p >> *b;
			bullets.push_back(b);
		}

		if (bulletsSize > 0) 
		{
			if (bulArray != nullptr) 
			{
				for (int i = 0; i < savedBulletSize; i++)
				{
					delete[] bulArray[i];
				}
				delete[] bulArray;
			}

			bulArray = new int*[bulletsSize];
			for (int i = 0; i < bulletsSize; i++)
			{
				bulArray[i] = new int[3];
				bulArray[i][0] = bullets[i]->turn;
				bulArray[i][1] = bullets[i]->x;
				bulArray[i][2] = bullets[i]->y;
			}
			savedBulletSize = bulletsSize;
		}
		else 
		{
			if (bulArray != nullptr)
			{
				for (int i = 0; i < savedBulletSize; i++)
				{
					delete[] bulArray[i];
				}
				delete[] bulArray;
			}
			bulArray = nullptr;
			savedBulletSize = bulletsSize;
		}

		for (int i = 0; i < bullets.size(); i++)
		{
			if (bullets[i] != nullptr)
			{
				delete bullets[i];
				bullets[i] = nullptr;
			}
		}
		bullets.clear();

		packetBullets = true;
	}
	else if (type == PACKET_TYPE::TYPE_MAP_CREATOR && !packetMap)
	{
		*p >> mapSizeX >> mapSizeY;
		createMap(mapSizeX, mapSizeY);

		packetMap = true;
	}
	else if (type == PACKET_TYPE::TYPE_MAP_PLAYERS)
	{
		*p >> mapData;
		for (int i = 0; i < MAX_PLAYER_NUMBER; i++)
		{
			*p >> *playersArray[i];
		}
		updateMap(mapData);
		packetMapPlayers = true;
	}
	
	if (packetMap && packetMapPlayers && packetBullets) 
	{
		listeningMode = false;
	}
}

void ApiHandler::setReadyFlag()
{
	listeningMode = true;
	packetBullets = false;
	packetMapPlayers = false;
}

int** ApiHandler::getMap()
{
	return mapArray;
}

int** ApiHandler::getBullets()
{
	return bulArray;
}

int ApiHandler::getBulletsCount()
{
	return savedBulletSize;
}

int** ApiHandler::getPlayers()
{
	for (int i = 0; i < MAX_PLAYER_NUMBER; i++)
	{
		playerArr[i][0] = playersArray[i]->ID;
		playerArr[i][1] = playersArray[i]->turn;
		playerArr[i][2] = playersArray[i]->x;
		playerArr[i][3] = playersArray[i]->y;
	}

	return playerArr;
}