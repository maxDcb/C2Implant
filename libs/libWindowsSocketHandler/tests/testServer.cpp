#include "SocketHandler.hpp"

#include <iostream>


using namespace std;
using namespace SocketHandler;

int main()
{ 
    int port=4444;
    Server* server = new Server(port);

    while(1)
    {

        string ret;
        server->receive(ret);

        std::cout << "Server - " << ret << std::endl;

        string out="{}";
        server->sendData(out);

        Sleep(1000);        
    }

}