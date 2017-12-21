#include "PacketHandler.h"
#include <iostream>
#include <vector>
using namespace std;

class Stub {
public:
	Stub() {
		x = 11;
	}
	int x;
	int y;
	int z;
};

int main() {
	recievePacket();
	int n;
	

	Test *t = new Test();
	cout << t->getX();
	Stub s;
	
		cout << t->getStub().x;

		vector<Stub> list = t->getStubs();
	system("pause");
	return 0;
}