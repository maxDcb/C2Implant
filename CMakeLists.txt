cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING
    "Use the static MSVC runtime so beacon targets and Conan libraries agree.")

set(C2_CONAN_PROVIDER "${CMAKE_CURRENT_LIST_DIR}/conan_provider.cmake")
if(EXISTS "${C2_CONAN_PROVIDER}")
  list(PREPEND CMAKE_PROJECT_TOP_LEVEL_INCLUDES "${C2_CONAN_PROVIDER}")
endif()

project(C2Implant VERSION 0.0.0 LANGUAGES CXX C)

if(WIN32 AND NOT DEFINED CONAN_COMMAND)
  file(GLOB C2_CONAN_HINT_DIRS LIST_DIRECTORIES TRUE
    "$ENV{LOCALAPPDATA}/Programs/Python/Python*/Scripts"
    "$ENV{APPDATA}/Python/Python*/Scripts")
  find_program(CONAN_COMMAND
    NAMES conan conan.exe
    HINTS ${C2_CONAN_HINT_DIRS})
endif()

if(WIN32)
  set(C2_CMAKE_COMMAND "${CMAKE_COMMAND}")
  set(C2_CONAN_CMAKE_PROGRAM_ARGS)
  if(C2_CMAKE_COMMAND MATCHES " ")
    file(GLOB C2_CONAN_CMAKE_CANDIDATES "$ENV{USERPROFILE}/.conan2/p/cmake*/p/bin/cmake.exe")
    if(C2_CONAN_CMAKE_CANDIDATES)
      list(SORT C2_CONAN_CMAKE_CANDIDATES ORDER DESCENDING)
      list(GET C2_CONAN_CMAKE_CANDIDATES 0 C2_CMAKE_COMMAND)
      file(TO_CMAKE_PATH "${C2_CMAKE_COMMAND}" C2_CMAKE_COMMAND)
      set(C2_CONAN_CMAKE_PROGRAM_ARGS
          "-c:h;tools.cmake:cmake_program=${C2_CMAKE_COMMAND};-c:b;tools.cmake:cmake_program=${C2_CMAKE_COMMAND}")
    endif()
  else()
    file(TO_CMAKE_PATH "${C2_CMAKE_COMMAND}" C2_CMAKE_COMMAND)
    set(C2_CONAN_CMAKE_PROGRAM_ARGS
        "-c:h;tools.cmake:cmake_program=${C2_CMAKE_COMMAND};-c:b;tools.cmake:cmake_program=${C2_CMAKE_COMMAND}")
  endif()
  if(DEFINED CONAN_INSTALL_ARGS AND NOT CONAN_INSTALL_ARGS STREQUAL "")
    set(_C2_CONAN_INSTALL_ARGS "${CONAN_INSTALL_ARGS}")
    if(C2_CONAN_CMAKE_PROGRAM_ARGS)
      list(APPEND _C2_CONAN_INSTALL_ARGS "${C2_CONAN_CMAKE_PROGRAM_ARGS}")
    endif()
  else()
    set(_C2_CONAN_INSTALL_ARGS
        "--build=missing;${C2_CONAN_CMAKE_PROGRAM_ARGS}")
  endif()
  set(CONAN_INSTALL_ARGS
      "${_C2_CONAN_INSTALL_ARGS}"
      CACHE STRING "Conan install arguments for C2Implant" FORCE)
  set(CONAN_INSTALL_CONFIGURATIONS Release CACHE STRING
      "Only install the configuration built by C2Implant in Visual Studio generators." FORCE)
endif()

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(MSVC)
  add_compile_options($<$<CONFIG:Release>:/O1> $<$<CONFIG:Release>:/Ob0>)
else()
  set(CMAKE_CXX_FLAGS_RELEASE "-O1")
  set(CMAKE_C_FLAGS_RELEASE "-O1")
endif()

add_compile_definitions(
  NOMINMAX                # <-- disables Windows min/max macros
)

option(C2CORE_BUILD_TESTS "Enable C2Core tests from the parent project" OFF)
if(C2CORE_BUILD_TESTS)
  enable_testing()
endif()

set(C2_RUNTIME_MODULE_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Release/Modules" CACHE PATH
    "Directory where C2Core runtime modules are copied after build")
set(C2_TEST_BIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/tests" CACHE PATH
    "Directory where C2Core test binaries are copied after build")

file(MAKE_DIRECTORY "${C2_RUNTIME_MODULE_OUTPUT_DIR}")
file(MAKE_DIRECTORY "${C2_TEST_BIN_OUTPUT_DIR}")


add_definitions(-DBUILD_IMPLANT)

##
## Conan Dependencies
##
find_package(Libssh2 QUIET CONFIG)
if(NOT Libssh2_FOUND)
  find_package(libssh2 REQUIRED CONFIG)
endif()

if(TARGET libssh2::libssh2 AND NOT TARGET Libssh2::libssh2)
  add_library(Libssh2::libssh2 INTERFACE IMPORTED)
  target_link_libraries(Libssh2::libssh2 INTERFACE libssh2::libssh2)
endif()

find_package(nlohmann_json REQUIRED CONFIG)

include_directories(${CMAKE_INCLUDE_PATH})

get_target_property(C2_NLOHMANN_JSON_INCLUDE_DIRS nlohmann_json::nlohmann_json INTERFACE_INCLUDE_DIRECTORIES)
if(C2_NLOHMANN_JSON_INCLUDE_DIRS)
  include_directories(BEFORE ${C2_NLOHMANN_JSON_INCLUDE_DIRS})
endif()

include_directories(thirdParty)

add_definitions(-DBUILD_IMPLANT)


##
## Build
##

add_subdirectory(libs)

add_subdirectory(thirdParty)
include_directories(thirdParty/base64)
include_directories(thirdParty/donut/include)
include_directories(thirdParty/coffLoader/coffLoader)
include_directories(thirdParty/MemoryModule)

set(DONUT_BUILD_DIR "${CMAKE_BINARY_DIR}/thirdParty/donut")
add_library(Donut STATIC IMPORTED)
set_target_properties(Donut PROPERTIES
IMPORTED_LOCATION "${DONUT_BUILD_DIR}/lib/donut.lib"
)

add_subdirectory(core/modules)
add_subdirectory(core/beacon)

include_directories(core/listener)
include_directories(core/beacon)
include_directories(core/modules/ModuleCmd)
add_subdirectory(beacon/beacon)
