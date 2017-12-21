#include "PacketHandler.h"

class Stub {
public:
	Stub() {
		x = 10;
	}
	int x;
};

void recievePacket() {
	Packet p;
	p << "cze" << "taki test";
	string t;
	string q;
	p >> t >> q;
	cout << t << " " << q << endl;
}

Test::Test() {
	x = 5;
	s = new Stub();
}

int Test::getX() {
	return x;
}

Stub Test::getStub() {
	return *s;
}

vector<Stub> Test::getStubs() {
	vector<Stub> listOfStubs;
	Stub p;
	for (int i = 0; i < 14; i++) {
		p.x = i;
		listOfStubs.push_back(p);
	}
	return listOfStubs;
}