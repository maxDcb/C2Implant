set(C2_BASE64_SOURCE_DIR "${CMAKE_SOURCE_DIR}/thirdParty/base64" CACHE PATH
    "Path to the cpp-base64 source tree consumed by C2Core")
set(C2_DONUT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/thirdParty/donut" CACHE PATH
    "Path to the donut source tree consumed by C2Core")

add_library(c2_base64 STATIC
  "${C2_BASE64_SOURCE_DIR}/base64.cpp"
)

target_include_directories(c2_base64 PUBLIC
  "${C2_BASE64_SOURCE_DIR}"
)

set_target_properties(c2_base64 PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)

add_subdirectory(coffLoader)
add_subdirectory(MemoryModule)

## donut
file(COPY ${CMAKE_SOURCE_DIR}/thirdParty/donut DESTINATION ${CMAKE_BINARY_DIR}/thirdParty)

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "ARM64" OR "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "^(ARM64|arm64|aarch64)$" OR "${MSVC_CXX_ARCHITECTURE_ID}" MATCHES "ARM64")

	execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Enterprise\\Common7\\Tools\\VsDevCmd.bat" -arch=arm64 && nmake -f .\\Makefile_arm64.msvc RESULT_VARIABLE ret)
	if(NOT ret EQUAL "0")
		execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Community\\Common7\\Tools\\VsDevCmd.bat" -arch=arm64 && nmake -f .\\Makefile_arm64.msvc RESULT_VARIABLE ret)
	endif()
	if(NOT ret EQUAL "0")
		execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && nmake -f .\\Makefile_arm64.msvc RESULT_VARIABLE ret)
	endif()

elseif("${MSVC_CXX_ARCHITECTURE_ID}" MATCHES "X86")
	
	execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Enterprise\\Common7\\Tools\\VsDevCmd.bat" -arch=x86 && nmake -f .\\Makefile_x86.msvc RESULT_VARIABLE ret)
	if(NOT ret EQUAL "0")
		execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Community\\Common7\\Tools\\VsDevCmd.bat" -arch=x86 && nmake -f .\\Makefile_x86.msvc RESULT_VARIABLE ret)
	endif()
	if(NOT ret EQUAL "0")
		execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && nmake -f .\\Makefile_x86.msvc RESULT_VARIABLE ret)
	endif()

else()

	execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Enterprise\\Common7\\Tools\\VsDevCmd.bat" -arch=amd64 && nmake -f .\\Makefile.msvc RESULT_VARIABLE ret)
	if(NOT ret EQUAL "0")
		execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Community\\Common7\\Tools\\VsDevCmd.bat" -arch=amd64 && nmake -f .\\Makefile.msvc RESULT_VARIABLE ret)
	endif()
	if(NOT ret EQUAL "0")
		execute_process(COMMAND cmd.exe /c cd ${CMAKE_BINARY_DIR}/thirdParty/donut && nmake -f .\\Makefile.msvc RESULT_VARIABLE ret)
	endif()

endif()

set(Donut "${CMAKE_BINARY_DIR}/thirdParty/donut/lib/donut.lib" PARENT_SCOPE)
