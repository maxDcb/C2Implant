# Exploration C2 Implant

## What it is

Exploration is a rudimentary red team command and control frameworks.  
This repository contain the Implant in C++ to target windows.
This development is in education exercises to tackle well know red teaming concepts.

## Dependencies:

### Sumbodule & External Projects:  

* [Donut](https://github.com/TheWover/donut): Creat shellcode from PE files.  
* [CoffLoader](https://github.com/trustedsec/COFFLoader): Run object files from [CS-Situational-Awareness-BOF](https://github.com/trustedsec/CS-Situational-Awareness-BOF).
* [MemoryModule](https://github.com/fancycode/MemoryModule): Load DLL at runtime.
* [UnmanagedPowerShell](https://github.com/leechristensen/UnmanagedPowerShell): Powershell for unmanager code.
* [cpp-base64](https://github.com/ReneNyffenegger/cpp-base64): base64.
* [json](https://github.com/nlohmann/json): json parser.

### Compilation

#### Windows - Build the Windows Beacon and Modules

* https://chocolatey.org/install
* choco install cmake --pre 

git submodule update --init   
mkdir buildWindows  
cd buildWindows  
with "x64 Native Tools Command Prompt for VS":  
cmake  -G "Visual Studio 17 2022" ..   
msbuild .\C2.sln /property:Configuration=Release  
or  
compile the generated C2.sln in release with Visual studio (config Runtime Library Multi-threaded (/MT) & Release)   

Beacons are in: "Release\Beacons"  
Modules DLL in: "Release\Modules"   

At this point you should copy all the production: modules and beacons on you TeamServer box to be able to use it.



