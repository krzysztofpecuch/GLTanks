#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#ifdef DLLSWITCH  
#define API __declspec(dllexport)   

#include <iostream>
#include <vector>
#include "SFML\Network.hpp"

using namespace std;
using namespace sf;

#else

#define API __declspec(dllimport)
#include <vector>
using namespace std;

#endif  

class Stub;

API void recievePacket();

class API Test {
public:
	Test();
	int getX();
	Stub getStub();
	vector<Stub> getStubs();
private:
	int x;
	Stub *s;
};
#endif // !PACKETHANDLER_H
