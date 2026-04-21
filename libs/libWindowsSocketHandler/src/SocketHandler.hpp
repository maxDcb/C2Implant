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

	bool sendData(std::string& data);
	bool receive(std::string& data);

private:
	void reset();
	void initServer();
	void creatServerTcp(int port);

	bool m_initDone;
	int m_port;

	std::thread* threadInit;

	struct addrinfo *result;
    SOCKET ListenSocket;
    SOCKET ClientSocket;
};



class Client
{
public:
	Client(std::string& ip, int port);
	~Client();

	bool sendData(std::string& data);
	bool receive(std::string& data);

private:
	void reset();
	void creatClientTcp(int port, std::string& ip);

	std::string m_ipServer;
	int m_port;

	struct addrinfo *result;
	SOCKET ConnectSocket;
};

}
