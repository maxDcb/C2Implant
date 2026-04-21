add_executable(SocketHandlerClientTest "testClient.cpp" )
if(WIN32)
	target_link_libraries(SocketHandlerClientTest SocketHandler spdlog::spdlog)
else()
endif()


add_executable(SocketHandlerServerTest "testServer.cpp" )
if(WIN32)
	target_link_libraries(SocketHandlerServerTest SocketHandler spdlog::spdlog)
else()
endif()
