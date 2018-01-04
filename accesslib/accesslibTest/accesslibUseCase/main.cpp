#include "ApiHandler.h"
#include <iostream>
#include <vector>
#include <conio.h>
using namespace std;

int main() {
	ApiHandler *ah = new ApiHandler();

	ah->connect("10.0.80.73");
	char c = 'z';
	while (c != 'q') {
		cin.get(c);
		if (c == '\n')
			continue;
		int q = c - '0';
		ah->forceSendAction(q);
		/*ah->listenForPacket();*/
		//obliczanie taktyki
		//send
	}
	
	
	system("pause");
	return 0;
}