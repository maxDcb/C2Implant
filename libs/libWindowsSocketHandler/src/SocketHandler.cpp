#include "SocketHandler.hpp"


#pragma comment(lib, "ws2_32.lib")


using namespace SocketHandler;

// https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code
Server::Server(int port)
{
	m_port = port;

    // Initialize Winsock
	WSADATA wsaData;
   int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        // printf("WSAStartup failed with error: %d\n", iResult);
        return;
    }

	m_initDone=false;
	threadInit = new std::thread(&Server::initServer, this);

}


void Server::reset()
{
	m_initDone=false;
	threadInit = new std::thread(&Server::initServer, this);
}


void Server::initServer()
{
	creatServerTcp(m_port);
}


Server::~Server()
{
	threadInit->join();

	// cleanup
    closesocket(ListenSocket);
	closesocket(ClientSocket);
    WSACleanup();
}


void Server::creatServerTcp(int port)
{
	int iResult;

	ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;

	struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

	std::string sPort = std::to_string(m_port);

    // Resolve the server address and port
	result = NULL;
    iResult = getaddrinfo(NULL, sPort.c_str(), &hints, &result);
    if ( iResult != 0 ) 
	{
        // printf("getaddrinfo failed with error: %d\n", iResult);
        return ;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) 
	{
        // printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        return ;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) 
	{
        // printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        return ;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) 
	{
        // printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        return ;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) 
	{
        // printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        return ;
    }

    // No longer need server socket
    closesocket(ListenSocket);

	m_initDone=true;
}


bool Server::sendData(std::string& data)
{
	if(!m_initDone)
	{
		// std::cerr << "Server: No connection" << std::endl;
		return false;
	}

	int nbBytes = data.size();
	int iResult = send(ClientSocket, (char*)&nbBytes, (int) sizeof(int), 0);
	if (iResult == SOCKET_ERROR) 
	{
		// printf("send failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		reset();
		return false;
	}

	iResult = send(ClientSocket, (char*)&data[0], (int) nbBytes, 0);
	if (iResult == SOCKET_ERROR) 
	{
		// printf("send failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		reset();
		return false;
	}

	return true;
}


bool Server::receive(std::string& data)
{
	if(!m_initDone)
	{
		// std::cerr << "Server: No connection" << std::endl;
		return false;
	}

	int nbBytes=0;
	int iResult = recv(ClientSocket, (char*)&nbBytes, sizeof(int), 0);
	if (iResult == SOCKET_ERROR) 
	{
		// printf("recv failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		reset();
		return false;
	}
	data.resize(nbBytes);

	iResult = recv(ClientSocket, &data[0], nbBytes, 0);
	if (iResult == SOCKET_ERROR) 
	{
		// printf("recv failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		reset();
		return false;
	}

	return true;
}


// https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code
Client::Client(std::string& ip, int port)
{
	m_ipServer = ip;
	m_port = port;

    // Initialize Winsock
	WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) 
	{
        // printf("WSAStartup failed with error: %d\n", iResult);
        return ;
    }

	creatClientTcp(m_port, m_ipServer);
}


Client::~Client()
{
	// shutdown the connection since no more data will be sent
	int iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) 
	{
	    // printf("shutdown failed with error: %d\n", WSAGetLastError());
	    closesocket(ConnectSocket);
	    WSACleanup();
	    return ;
	}

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
}


void Client::reset()
{
	creatClientTcp(m_port, m_ipServer);
}


void Client::creatClientTcp(int port, std::string& ip)
{
	int iResult;

	ConnectSocket = INVALID_SOCKET;

	struct addrinfo hints;
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
	result = NULL;
	std::string sPort = std::to_string(m_port);
    iResult = getaddrinfo(m_ipServer.c_str(), sPort.c_str(), &hints, &result);
    if ( iResult != 0 ) 
	{
         // printf("getaddrinfo failed with error: %d\n", iResult);
        return ;
    }

	// Attempt to connect to an address until one succeeds
	struct addrinfo *ptr = NULL;
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) 
	{

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) 
		{
             // printf("socket failed with error: %ld\n", WSAGetLastError());
            return ;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) 
		{
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

	freeaddrinfo(result);
}


bool Client::sendData(std::string& data)
{
	int nbBytes = data.size();
	int iResult = send(ConnectSocket, (char*)&nbBytes, (int) sizeof(int), 0);
	if (iResult == SOCKET_ERROR) 
	{
		// printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		reset();
		return false;
	}

	iResult = send(ConnectSocket, (char*)&data[0], (int) nbBytes, 0);
	if (iResult == SOCKET_ERROR) 
	{
		// printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		reset();
		return false;
	}

	return true;
}


bool Client::receive(std::string& data)
{
	int nbBytes=0;
	int iResult = recv(ConnectSocket, (char*)&nbBytes, sizeof(int), 0);

	if (iResult == SOCKET_ERROR) 
	{
		// printf("recv failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		
		reset();
		return false;
	}
	data.resize(nbBytes);

	if (nbBytes > 0)
	{
		iResult = recv(ConnectSocket, &data[0], nbBytes, 0);
		if (iResult == SOCKET_ERROR)
		{
			// printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			reset();
			return false;
		}
	}

	return true;
}


