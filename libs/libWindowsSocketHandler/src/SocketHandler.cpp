#include "SocketHandler.hpp"

#pragma comment(lib, "ws2_32.lib")


#define SPDLOG_LEVEL_TRACE 0
#define SPDLOG_LEVEL_DEBUG 1
#define SPDLOG_LEVEL_INFO 2
#define SPDLOG_LEVEL_WARN 3
#define SPDLOG_LEVEL_ERROR 4
#define SPDLOG_LEVEL_CRITICAL 5
#define SPDLOG_LEVEL_OFF 6

#if defined(BUILD_TEAMSERVER) || defined(BUILD_TESTS) 

    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

    #if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
        #define SPDLOG_TRACE(...) printf(__VA_ARGS__)
    #else
        #define SPDLOG_TRACE(...) (void)0
    #endif

    #if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
        #define SPDLOG_DEBUG(...) printf(__VA_ARGS__)
    #else
        #define SPDLOG_DEBUG(...) (void)0
    #endif

    #if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
        #define SPDLOG_INFO(...) printf(__VA_ARGS__)
    #else
        #define SPDLOG_INFO(...) (void)0
    #endif

    #if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
        #define SPDLOG_WARN(...) printf(__VA_ARGS__)
    #else
        #define SPDLOG_WARN(...) (void)0
    #endif

    #if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
        #define SPDLOG_ERROR(...) printf(__VA_ARGS__)
    #else
        #define SPDLOG_ERROR(...) (void)0
    #endif

    #if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
        #define SPDLOG_CRITICAL(...) printf(__VA_ARGS__)
    #else
        #define SPDLOG_CRITICAL(...) (void)0
    #endif

#else

    #define SPDLOG_TRACE(...) (void)0
    #define SPDLOG_DEBUG(...) (void)0
    #define SPDLOG_INFO(...) (void)0
    #define SPDLOG_WARN(...) (void)0
    #define SPDLOG_ERROR(...) (void)0
    #define SPDLOG_CRITICAL(...) (void)0

#endif

using namespace SocketHandler;


// https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code
Server::Server(int port)
{
	m_port = port;
}


Server::~Server()
{
	closeConnection();
}


void Server::initServer()
{
	SPDLOG_DEBUG("initServer");

	WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) 
	{
        SPDLOG_DEBUG("WSAStartup failed with error: {0}", iResult);
        return;
    }

	m_listenSocket = INVALID_SOCKET;
    m_clientSocket = INVALID_SOCKET;

	struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

	std::string sPort = std::to_string(m_port);

	result = NULL;
    iResult = getaddrinfo(NULL, sPort.c_str(), &hints, &result);
    if ( iResult != 0 ) 
	{
        SPDLOG_DEBUG("getaddrinfo failed with error: {0}", iResult);
        return ;
    }

    m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_listenSocket == INVALID_SOCKET) 
	{
        SPDLOG_DEBUG("socket failed with error: {0}", WSAGetLastError());
        freeaddrinfo(result);
        return ;
    }

    iResult = bind( m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) 
	{
        SPDLOG_DEBUG("bind failed with error: {0}", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(m_listenSocket);
        return ;
    }

    freeaddrinfo(result);

    iResult = listen(m_listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) 
	{
        SPDLOG_DEBUG("listen failed with error: {0}", WSAGetLastError());
        closesocket(m_listenSocket);
        return ;
    }

    m_clientSocket = accept(m_listenSocket, NULL, NULL);
    if (m_clientSocket == INVALID_SOCKET) 
	{
        SPDLOG_DEBUG("accept failed with error: {0}", WSAGetLastError());
        closesocket(m_listenSocket);
        return ;
    }

	linger lingerOpt = { 1, 0 };
	setsockopt(m_listenSocket, SOL_SOCKET, SO_LINGER, (char*)&lingerOpt, sizeof(lingerOpt));

    closesocket(m_listenSocket);

	int timeOutMs = 5000;
	unsigned int  sz = sizeof(timeOutMs);
    setsockopt(m_clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeOutMs, sz);
}


bool Server::sendData(std::string& data)
{
	SPDLOG_DEBUG("sendData");

	int nbBytes = data.size();
	int iResult = send(m_clientSocket, (char*)&nbBytes, (int) sizeof(int), 0);
	if (iResult == SOCKET_ERROR) 
	{
		SPDLOG_DEBUG("send failed: {0}", WSAGetLastError());
		closesocket(m_clientSocket);
		return false;
	}

	if(nbBytes!=0)
	{
		iResult = send(m_clientSocket, (char*)&data[0], (int) nbBytes, 0);
		if (iResult == SOCKET_ERROR) 
		{
			SPDLOG_DEBUG("send failed: {0}", WSAGetLastError());
			closesocket(m_clientSocket);
			return false;
		}
	}

	return true;
}


bool Server::receive(std::string& data)
{
	SPDLOG_DEBUG("receive");

	int nbBytes=0;
	int iResult = recv(m_clientSocket, (char*)&nbBytes, sizeof(int), 0);
	if (iResult == SOCKET_ERROR) 
	{
		SPDLOG_DEBUG("recv failed: {0}", WSAGetLastError());
		closesocket(m_clientSocket);
		return false;
	}
	data.resize(nbBytes);

	if(nbBytes!=0)
	{
		int nbBytesReceived=0;
		while(nbBytesReceived!=nbBytes)
		{
			iResult = recv(m_clientSocket, &data[nbBytesReceived], nbBytes-nbBytesReceived, 0);
			if (iResult == SOCKET_ERROR) 
			{
				SPDLOG_DEBUG("recv failed: {0}", WSAGetLastError());
				closesocket(m_clientSocket);
				return false;
			}
			nbBytesReceived+=iResult;
		}
	}

	return true;
}


void Server::closeConnection()
{
	SPDLOG_DEBUG("closeConnection");

    int iResult = shutdown(m_clientSocket, SD_BOTH);
    if (iResult == SOCKET_ERROR) 
	{
        SPDLOG_DEBUG("shutdown failed with error: {0}", WSAGetLastError());
        closesocket(m_clientSocket);
        WSACleanup();
        return;
    }

	linger lingerOpt = { 1, 0 };
	setsockopt(m_clientSocket, SOL_SOCKET, SO_LINGER, (char*)&lingerOpt, sizeof(lingerOpt));

    closesocket(m_clientSocket);
    WSACleanup();
}


// https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code
Client::Client(std::string& ip, int port)
{
	m_ipServer = ip;
	m_port = port;
}


Client::~Client()
{
	closeConnection();
}


bool Client::initConnection()
{
	SPDLOG_DEBUG("initConnection");

	WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) 
	{
        SPDLOG_DEBUG("WSAStartup failed with error: {0}", iResult);
        return false;
    }

	m_connectSocket = INVALID_SOCKET;

	struct addrinfo hints;
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

	result = NULL;
	std::string sPort = std::to_string(m_port);
    iResult = getaddrinfo(m_ipServer.c_str(), sPort.c_str(), &hints, &result);
    if ( iResult != 0 ) 
	{
        SPDLOG_DEBUG("getaddrinfo failed with error: {0}", iResult);
		WSACleanup();
        return false;
    }

	struct addrinfo *ptr = NULL;
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) 
	{
        m_connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (m_connectSocket == INVALID_SOCKET) 
		{
            SPDLOG_DEBUG("socket failed with error: {0}", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
            return false;
        }

        iResult = connect( m_connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) 
		{
            closesocket(m_connectSocket);
            m_connectSocket = INVALID_SOCKET;
			freeaddrinfo(result);
			WSACleanup();
            return false;
        }
        break;
    }

	freeaddrinfo(result);

	int timeOutMs = 1000;
	unsigned int  sz = sizeof(timeOutMs);
    setsockopt(m_connectSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeOutMs, sz);

	return true;
}


bool Client::sendData(std::string& data)
{
	SPDLOG_DEBUG("sendData");

	int nbBytes = data.size();
	int iResult = send(m_connectSocket, (char*)&nbBytes, (int) sizeof(int), 0);
	if (iResult == SOCKET_ERROR) 
	{
		SPDLOG_DEBUG("send failed: {0}", WSAGetLastError());
		closesocket(m_connectSocket);
		return false;
	}

	if(nbBytes!=0)
	{
		iResult = send(m_connectSocket, (char*)&data[0], (int) nbBytes, 0);
		if (iResult == SOCKET_ERROR) 
		{
			SPDLOG_DEBUG("send failed: {0}", WSAGetLastError());
			closesocket(m_connectSocket);
			return false;
		}
	}

	return true;
}


bool Client::receive(std::string& data)
{
	SPDLOG_DEBUG("receive");

	int nbBytes=0;
	int iResult = recv(m_connectSocket, (char*)&nbBytes, sizeof(int), 0);

	if (iResult == SOCKET_ERROR) 
	{
		SPDLOG_DEBUG("recv failed: {0}", WSAGetLastError());
		closesocket(m_connectSocket);
		return false;
	}
	data.resize(nbBytes);

	if(nbBytes!=0)
	{
		int nbBytesReceived=0;
		while(nbBytesReceived!=nbBytes)
		{
			iResult = recv(m_connectSocket, &data[nbBytesReceived], nbBytes-nbBytesReceived, 0);
			if (iResult == SOCKET_ERROR) 
			{
				SPDLOG_DEBUG("recv failed: {0}", WSAGetLastError());
				closesocket(m_connectSocket);
				return false;
			}
			nbBytesReceived+=iResult;
		}

	}

	return true;
}


void Client::closeConnection()
{
	SPDLOG_DEBUG("closeConnection");

	int iResult = shutdown(m_connectSocket, SD_BOTH);
	if (iResult == SOCKET_ERROR) 
	{
	    SPDLOG_DEBUG("shutdown failed with error: {0}", WSAGetLastError());
	    closesocket(m_connectSocket);
	    WSACleanup();
	    return ;
	}

	linger lingerOpt = { 1, 0 };
	setsockopt(m_connectSocket, SOL_SOCKET, SO_LINGER, (char*)&lingerOpt, sizeof(lingerOpt));

    closesocket(m_connectSocket);
    WSACleanup();
}

