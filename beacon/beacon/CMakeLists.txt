set(SOURCES_BEACON_HTTP_LAUNCHER
	BeaconHttpLauncher.cpp
)
add_executable(BeaconHttp ${SOURCES_BEACON_HTTP_LAUNCHER} project.rc)
set_property(TARGET BeaconHttp PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconHttp BeaconHttpLib)
add_custom_command(TARGET BeaconHttp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconHttp> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconHttp>")


set(SOURCES_BEACON_TCP_LAUNCHER
BeaconTcpLauncher.cpp
)
add_executable(BeaconTcp ${SOURCES_BEACON_TCP_LAUNCHER} project.rc)
set_property(TARGET BeaconTcp PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconTcp BeaconTcpLib)
add_custom_command(TARGET BeaconTcp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconTcp> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconTcp>")


set(SOURCES_BEACON_GITHUB_LAUNCHER
BeaconGithubLauncher.cpp
)
add_executable(BeaconGithub ${SOURCES_BEACON_GITHUB_LAUNCHER} project.rc)
set_property(TARGET BeaconGithub PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconGithub BeaconGithubLib)
add_custom_command(TARGET BeaconGithub POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconGithub> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconGithub>")


set(SOURCES_BEACON_SMB_LAUNCHER
	BeaconSmbLauncher.cpp
)
add_executable(BeaconSmb ${SOURCES_BEACON_SMB_LAUNCHER} project.rc)
set_property(TARGET BeaconSmb PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconSmb BeaconSmbLib)
add_custom_command(TARGET BeaconSmb POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconSmb> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconSmb>")



set(SOURCES_BEACON_DNS_LAUNCHER
	BeaconDnsLauncher.cpp
)
add_executable(BeaconDns ${SOURCES_BEACON_DNS_LAUNCHER} project.rc)
set_property(TARGET BeaconDns PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
target_link_libraries(BeaconDns BeaconDnsLib)
add_custom_command(TARGET BeaconDns POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
$<TARGET_FILE:BeaconDns> "${CMAKE_SOURCE_DIR}/Release/Beacons/$<TARGET_FILE_NAME:BeaconDns>")

