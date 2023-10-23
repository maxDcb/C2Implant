add_subdirectory(coffLoader)
add_subdirectory(MemoryModule)

## donut
file(COPY ${CMAKE_SOURCE_DIR}/thirdParty/donut DESTINATION ${CMAKE_BINARY_DIR}/thirdParty)
file(REMOVE ${CMAKE_BINARY_DIR}/thirdParty/donut/Makefile.msvc)
file(COPY ${CMAKE_SOURCE_DIR}/thirdParty/custom/Makefile.msvc DESTINATION ${CMAKE_BINARY_DIR}/thirdParty/donut/)

if(${MSVC_CXX_ARCHITECTURE_ID} MATCHES "X86")
	
	execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Enterprise\\Common7\\Tools\\VsDevCmd.bat" -arch=x86 && nmake -f .\\Makefile_x86.msvc RESULT_VARIABLE ret)
	if(ret EQUAL "1")
		execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Community\\Common7\\Tools\\VsDevCmd.bat" -arch=x86 && nmake -f .\\Makefile_x86.msvc RESULT_VARIABLE ret)
	endif()
	if(ret EQUAL "1")
		execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && nmake -f .\\Makefile_x86.msvc RESULT_VARIABLE ret)
	endif()

else()

	execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Enterprise\\Common7\\Tools\\VsDevCmd.bat" -arch=amd64 && nmake -f .\\Makefile.msvc RESULT_VARIABLE ret)
	if(ret EQUAL "1")
		execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Community\\Common7\\Tools\\VsDevCmd.bat" -arch=amd64 && nmake -f .\\Makefile.msvc RESULT_VARIABLE ret)
	endif()
	if(ret EQUAL "1")
		execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && nmake -f .\\Makefile.msvc RESULT_VARIABLE ret)
	endif()

endif()

set(Donut "${CMAKE_BINARY_DIR}/thirdParty/donut/lib/donut.lib" PARENT_SCOPE)



