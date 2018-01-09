#include "ApiListener.h"
#include "ApiHandler.h"

ApiListener::ApiListener(ApiHandler& ah) : ah(ah)
{
	p = new sf::Packet();
	listeningThread = new thread(&ApiListener::listen, this);
	waitSecond = false;
}

ApiListener::~ApiListener() {
	delete p;
	listeningThread->join();
	delete listeningThread;
}

void ApiListener::listen() {
	while (true) {
		if (waitSecond)
		{
			this_thread::sleep_until(chrono::system_clock::now() + chrono::milliseconds(50));
		}
		
		int msgType;
		if (ah.getSocket()->receive(*p) == Socket::Done) {
			*p >> msgType;
			if (p->endOfPacket()) {
				p->clear();
				if (ah.getSocket()->receive(*p) == Socket::Done) {
					if (ah.getState())
					{
						ah.parsePacket(p, msgType);
					}
					waitSecond = true;
				}
			}
			p->clear();
		}
	}
}
