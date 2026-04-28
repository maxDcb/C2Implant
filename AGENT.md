# AGENT.md — C2Implant

## Purpose

You are an expert C++/CMake contributor tasked with maintaining **C2Implant**.

- Keep the project buildable with the parent repository layout already in use.
- Treat `core/` as a shared codebase consumed by multiple parents, not as Windows-only code.
- Preserve the current release layout: Beacon executables and DLLs in `Release/Beacons`, module DLLs in `Release/Modules`.

## Repository Role

`C2Implant` is the Windows Beacon parent repository.

- The top-level `CMakeLists.txt` owns the Windows build, third-party dependencies, and output layout.
- `core/` contains the shared transport and module logic reused across other repositories such as `C2TeamServer` and Linux-oriented parents.
- `beacon/beacon/` contains the launcher targets that produce the final Beacon executables and DLLs.
- `libs/` and `thirdParty/` are vendored or submodule-based dependencies expected by this parent.

## Build Context

The main supported local build here is Windows with Visual Studio and Conan.

- Preferred generator: `Visual Studio 17 2022`
- Preferred configuration: `Release`
- Runtime: static CRT (`/MT`)
- Dependency manager: Conan 2, through the top-level `conan_provider.cmake`

Typical x64 flow from WSL or Git Bash, using the Visual Studio bundled CMake:

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

Typical x86 flow:

```bash
cmake -S . -B build-conan-x86 -G "Visual Studio 17 2022" -A Win32
cmake --build build-conan-x86 --config Release -- /m
```

Typical ARM64 flow:

```bash
cmake -S . -B build-conan-arm64 -G "Visual Studio 17 2022" -A ARM64 "-DCONAN_INSTALL_ARGS=--build=missing;-o;openssl/*:no_asm=True"
cmake --build build-conan-arm64 --config Release -- /m
```

If running directly from a Windows developer prompt, the same commands can use `cmake` instead of the absolute Visual Studio CMake path.

The current Conan dependencies are declared in `conanfile.txt`:

- `libssh2`, consumed by `core/modules/SshExec`
- `nlohmann_json`, consumed by `core` beacon/listener/module command code

The top-level CMake sets `CMAKE_MSVC_RUNTIME_LIBRARY` before `project(...)` so local targets and Conan packages agree on the static MSVC runtime. Do not replace this with raw `-MT` or `/MT` flag edits; Conan will not reliably see those during provider initialization.

For Visual Studio multi-config generators, the project sets `CONAN_INSTALL_CONFIGURATIONS=Release`. This avoids building Debug Conan packages when the supported local validation is Release-only. If Debug builds become a requirement, update that cache variable to include both `Release` and `Debug` and expect Conan to build a second dependency set.

The parent builds `thirdParty/base64` as the `c2_base64` target. Shared `core/` code must link to `c2_base64` instead of assuming a raw include-only base64 dependency.

## Core Duality: Windows vs Linux

This repository consumes shared code from `core/`, but `core/` is not symmetric across platforms.

- On Windows Beacon-side code, HTTP/HTTPS and GitHub transports use WinHTTP, WinCrypt, BCrypt, and other WinAPI facilities.
- On Linux, the equivalent transport code may rely on `httplib` and OpenSSL.
- Because of that, **do not blindly link Linux transport dependencies on Windows targets** just because the shared source file lives under `core/`.

Current rule for Beacon-side shared libraries:

- `BeaconHttpLib` and `BeaconGithubLib` must not require `openssl::openssl` or `httplib::httplib` on Windows.
- Those dependencies are valid only for non-Windows builds unless the source code explicitly changes to require them on Windows too.
- When editing `core/beacon/CMakeLists.txt`, keep transport-specific link dependencies conditional on platform.

The same caution applies to tests:

- If a test target covers Linux-only transport plumbing, guard its link dependencies with platform checks.
- Do not make Windows test builds depend on Linux-only imported targets unless the Windows code path truly uses them.

## Editing Rules

- Prefer minimal CMake changes that keep the existing structure intact.
- Avoid moving ownership of build logic out of the top-level parent unless there is a clear repo-wide migration.
- When changing `core/`, assume the same code may be compiled by another parent with a different dependency graph.
- If a change in `core/` introduces new imported targets, add them conditionally and verify both the Windows parent behavior and the non-Windows intent.
- When adding a Conan dependency used by `core/`, expose a stable imported target name at the parent level if Conan's package target name differs from the name expected by `core`.
- Keep `c2_base64` as a normal CMake target. Do not reintroduce direct compilation of `base64.cpp` in multiple consumers.

## Validation

For Windows-facing changes, validate with a real Visual Studio build when possible.

- Regenerate with CMake after touching `CMakeLists.txt`.
- Confirm that `SshExec`, `BeaconTcp`, `BeaconSmb`, `BeaconDns`, `BeaconHttp`, and `BeaconGithub` all link successfully.
- Check timestamps or outputs in `Release/Beacons` if you need to distinguish freshly produced binaries from stale ones.
- Watch specifically for `LNK2038 RuntimeLibrary` and unresolved CRT externals from Conan libraries. Those usually mean a `/MD` vs `/MT` mismatch between local targets and Conan packages.

Warnings currently exist in the tree, especially around macro redefinitions from vendored dependencies. Do not confuse those with the transport-link regressions described above.

## CI/CD Contract

- CI must run on pull requests and branch pushes, not only on release tags.
- CI test workflows must build and test `x64`, `x86`, and `ARM64` with `C2CORE_BUILD_TESTS=ON`. ARM64 validation must run on a Windows ARM64 runner.
- CD workflows must build production Beacon and Module deliverables without enabling `C2CORE_BUILD_TESTS`.
- CD must publish only the Windows Beacon and Module deliverables consumed by `C2TeamServer`.
- Do not mutate `Release/Beacons` or `Release/Modules` during packaging. Copy deliverables into a clean staging directory, then zip the staging directory.
- Keep the release archive layout stable: `WindowsBeacons/` and `WindowsModules/`.
- Publish one archive per architecture: `C2Implant-windows-x64.zip`, `C2Implant-windows-x86.zip`, and `C2Implant-windows-arm64.zip`.
- If a test is not stable enough for CI, fix or isolate it explicitly; do not silently remove the full CTest step.
