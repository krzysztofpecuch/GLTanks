#include "ApiHandler.h"
#include <iostream>
#include <vector>
#include <conio.h>
using namespace std;

int main() {
	ApiHandler *ah = new ApiHandler();

	ah->connect("10.0.80.43");
	char c = 'z';
	while (c != 'q') {		
		cin.get(c);
		if (c == '\n')
			continue;
		int q = c - '0';
		ah->forceSendAction(q);
		ah->setReadyFlag();
		

		while (ah->getState()) {
			
		}
		
		int **players = ah->getPlayers();
		int **map = ah->getMap();
		int **bullets = ah->getBullets();
		/*for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				cout << players[i][j] << " ";
			}
			cout << endl;
		}
		for (int i = 0; i < 17; i++)
		{
			for (int j = 0; j < 17; j++)
			{
				cout << map[i][j] << " ";
			}
			cout << endl;
		}*/
		for (int i = 0; i < ah->getBulletsCount(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cout << bullets[i][j] << " ";
			}
			cout << endl;
		}
		
		
		/*ah->listenForPacket();*/
		//obliczanie taktyki
		//send
	}


	system("pause");
	return 0;
}