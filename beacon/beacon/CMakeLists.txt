find_package(PythonInterp)
find_package(Python)

set(PYTHON_SCRIPT "${CMAKE_SOURCE_DIR}//beacon/beacon/handleConfig.py")
set(GENERATED_HEADER "${CMAKE_BINARY_DIR}/cryptDef.hpp")
add_custom_command(
    OUTPUT ${GENERATED_HEADER}
    COMMAND ${PYTHON_EXECUTABLE} ${PYTHON_SCRIPT} -o ${GENERATED_HEADER} 
    COMMENT "Generating header.hpp using handleConfig.py"
    VERBATIM
)
add_custom_target(HandleConfig ALL DEPENDS ${GENERATED_HEADER})
include_directories(${CMAKE_BINARY_DIR})


set(SOURCES_BEACON_HTTP_LAUNCHER
	BeaconHttpLauncher.cpp
)
add_executable(BeaconHttp ${SOURCES_BEACON_HTTP_LAUNCHER} project.rc)
add_dependencies(BeaconHttp HandleConfig)
set_property(TARGET BeaconHttp PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconHttp BeaconHttpLib)
add_custom_command(TARGET BeaconHttp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconHttp> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconHttp>")

add_library(BeaconHttpDll SHARED ${SOURCES_BEACON_HTTP_LAUNCHER})
add_dependencies(BeaconHttpDll HandleConfig)
set_property(TARGET BeaconHttpDll PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconHttpDll BeaconHttpLib)
add_custom_command(TARGET BeaconHttpDll POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconHttpDll> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconHttpDll>")


set(SOURCES_BEACON_TCP_LAUNCHER
BeaconTcpLauncher.cpp
)
add_executable(BeaconTcp ${SOURCES_BEACON_TCP_LAUNCHER} project.rc)
set_property(TARGET BeaconTcp PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconTcp BeaconTcpLib)
add_custom_command(TARGET BeaconTcp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconTcp> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconTcp>")

add_library(BeaconTcpDll SHARED ${SOURCES_BEACON_TCP_LAUNCHER})
add_dependencies(BeaconTcpDll HandleConfig)
set_property(TARGET BeaconTcpDll PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconTcpDll BeaconTcpLib)
add_custom_command(TARGET BeaconTcpDll POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconTcpDll> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconTcpDll>")


set(SOURCES_BEACON_GITHUB_LAUNCHER
BeaconGithubLauncher.cpp
)
add_executable(BeaconGithub ${SOURCES_BEACON_GITHUB_LAUNCHER} project.rc)
set_property(TARGET BeaconGithub PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconGithub BeaconGithubLib)
add_custom_command(TARGET BeaconGithub POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconGithub> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconGithub>")

add_library(BeaconGithubDll SHARED ${SOURCES_BEACON_GITHUB_LAUNCHER})
add_dependencies(BeaconGithubDll HandleConfig)
set_property(TARGET BeaconGithubDll PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconGithubDll BeaconGithubLib)
add_custom_command(TARGET BeaconGithubDll POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconGithubDll> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconGithubDll>")


set(SOURCES_BEACON_SMB_LAUNCHER
	BeaconSmbLauncher.cpp
)
add_executable(BeaconSmb ${SOURCES_BEACON_SMB_LAUNCHER} project.rc)
set_property(TARGET BeaconSmb PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconSmb BeaconSmbLib)
add_custom_command(TARGET BeaconSmb POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconSmb> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconSmb>")

add_library(BeaconSmbDll SHARED ${SOURCES_BEACON_SMB_LAUNCHER})
add_dependencies(BeaconSmbDll HandleConfig)
set_property(TARGET BeaconSmbDll PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconSmbDll BeaconSmbLib)
add_custom_command(TARGET BeaconSmbDll POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconSmbDll> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconSmbDll>")


set(SOURCES_BEACON_DNS_LAUNCHER
	BeaconDnsLauncher.cpp
)
add_executable(BeaconDns ${SOURCES_BEACON_DNS_LAUNCHER} project.rc)
set_property(TARGET BeaconDns PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconDns BeaconDnsLib)
add_custom_command(TARGET BeaconDns POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconDns> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconDns>")

add_library(BeaconDnsDll SHARED ${SOURCES_BEACON_DNS_LAUNCHER})
add_dependencies(BeaconDnsDll HandleConfig)
set_property(TARGET BeaconDnsDll PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconDnsDll BeaconDnsLib)
add_custom_command(TARGET BeaconDnsDll POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconDnsDll> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconDnsDll>")

