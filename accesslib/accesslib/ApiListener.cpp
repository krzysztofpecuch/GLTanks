#include "ApiListener.h"
#include "ApiHandler.h"

ApiListener::ApiListener(ApiHandler& ah) : ah(ah)
{
	p = new sf::Packet();
	listeningThread = new thread(&ApiListener::listen, this);
	startWait = false;
}

ApiListener::~ApiListener() {
	delete p;
	listeningThread->join();
	delete listeningThread;
}

void ApiListener::listen() {
	while (true) {
		if (startWait)
		{
			this_thread::sleep_until(chrono::system_clock::now() + chrono::milliseconds(500));
		}
		
		int msgType;
		msgType = -1;
		if (ah.getSocket()->receive(*p) == Socket::Done) {
			*p >> msgType;
			if (p->endOfPacket()) {
				p->clear();
				if (ah.getSocket()->receive(*p) == Socket::Done) {
					if (ah.getState())
					{
						ah.parsePacket(p, msgType);
					}
					startWait = true;
				}
			}
			p->clear();
		}
	}
}
