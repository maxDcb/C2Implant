#pragma once

#include <thread>
#include <string>
#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>


namespace SocketHandler
{

class Server
{
public:
	Server(int port);
	~Server();

	void initServer();
	void closeConnection();

	bool sendData(std::string& data);
	bool receive(std::string& data);

private:
	int m_port;

	struct addrinfo *result;
    SOCKET m_listenSocket;
    SOCKET m_clientSocket;
};



class Client
{
public:
	Client(std::string& ip, int port);
	~Client();

	bool initConnection();
	void closeConnection();

	bool sendData(std::string& data);
	bool receive(std::string& data);

private:
	std::string m_ipServer;
	int m_port;

	struct addrinfo *result;
	SOCKET m_connectSocket;
};

}
