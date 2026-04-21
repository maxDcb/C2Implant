cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)

project(C2Implant VERSION 0.0.0 LANGUAGES CXX C)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_RELEASE "-MT -O1 -Ob0")
set(CMAKE_C_FLAGS_RELEASE "-MT -O1 -Ob0")


##
## Conan Dependencies
##

include_directories(thirdParty)

##
## Config Tests et Logs
##

option(WITH_TESTS "Compile for tests" OFF)

message(STATUS "WITH_TESTS is set to ${WITH_TESTS}")

add_definitions(-DBUILD_IMPLANT)
if(NOT WITH_TESTS)
  message(STATUS "[-] Tests are disabled.")
else()
  message(STATUS "[+] Tests are enabled.")
  add_definitions(-DBUILD_TESTS)
endif()


##
## Build
##

add_subdirectory(libs)

add_subdirectory(thirdParty)
include_directories(thirdParty/base64)
include_directories(thirdParty/donut/include)
include_directories(thirdParty/coffLoader/coffLoader)
include_directories(thirdParty/MemoryModule)

if(WITH_TESTS)
    enable_testing()
endif()

add_subdirectory(core/modules)
add_subdirectory(core/beacon)

if(WITH_TESTS)
    add_subdirectory(core/beacon/tests)
    add_subdirectory(core/listener/tests)
endif()


