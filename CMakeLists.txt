cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)

project(C2Implant VERSION 0.0.0 LANGUAGES CXX C)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_RELEASE "-MT -O1 -Ob0")
set(CMAKE_C_FLAGS_RELEASE "-MT -O1 -Ob0")

include_directories(thirdParty/nlohmann/single_include/nlohmann)

add_subdirectory(libs)

add_subdirectory(thirdParty)
include_directories(thirdParty/base64)
include_directories(thirdParty/donut/include)
include_directories(thirdParty/coffLoader/coffLoader)
include_directories(thirdParty/MemoryModule)

enable_testing()

add_subdirectory(core/modules)
add_subdirectory(core/beacon)

add_subdirectory(core/beacon/tests)
add_subdirectory(core/listener/tests)


