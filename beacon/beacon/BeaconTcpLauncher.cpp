#include "BeaconTcp.hpp"


using namespace std;


int main(int argc, char* argv[])
{	
	std::string ip = "127.0.0.1";
	if(argc > 1)
		ip = argv[1];

	int port = 4444;
	if (argc > 2)
		port = atoi(argv[2]);

	std::unique_ptr<Beacon> beacon;
	beacon = make_unique<BeaconTcp>(ip, port);

	beacon->run();
}