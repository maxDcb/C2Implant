add_executable(SocketHandlerClientTest "testClient.cpp" )
if(WIN32)
	set_property(TARGET SocketHandlerClientTest PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
	target_link_libraries(SocketHandlerClientTest SocketHandler)
else()
endif()


add_executable(SocketHandlerServerTest "testServer.cpp" )
if(WIN32)
	set_property(TARGET SocketHandlerServerTest PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
	target_link_libraries(SocketHandlerServerTest SocketHandler)
else()
endif()
