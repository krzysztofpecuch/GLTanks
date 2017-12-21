#include "socket.h"
#include <iostream>


ConnectSocket::ConnectSocket()
{
}

void ConnectSocket::connect()
{
	mySocket = new sf::TcpSocket();
	char serverAddr[] = "127.0.0.1";
	if (mySocket->connect(serverAddr, 55000) == sf::Socket::Done)
	{
		std::string message = "Hi, I am a client";
		mySocket->send(message.c_str(), message.size() + 1);

		//// Receive an answer from the server
		//char buffer[1024];
		//std::size_t received = 0;
		//mySocket->receive(buffer, sizeof(buffer), received);
		//std::cout << "The server said: " << buffer << std::endl;
	}
}

int ConnectSocket::ReceiveData()
{
	return 0;
}
