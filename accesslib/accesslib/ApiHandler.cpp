#include "ApiHandler.h"

ApiHandler::ApiHandler()
{
	readyForAction = false;
	p.clear();
//	connect();
}


ApiHandler::~ApiHandler()
{
}

void ApiHandler::connect() {

}

void ApiHandler::listenForPacket() {
	while (!readyForAction) {
		if (ts.receive(p) == Socket::Done) {
			readyForAction = true;
//			parsePacket();
//			p.clear();
		}
	}
}

void ApiHandler::sendAction(int action) {
	
	while (readyForAction) {
		Packet move;
		move << action;
		if (ts.send(move) == Socket::Done) {
			readyForAction = false;
		}
	}
}
