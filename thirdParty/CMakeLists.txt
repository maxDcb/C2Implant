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
set(C2_DONUT_BUILD_DIR "${CMAKE_BINARY_DIR}/thirdParty/donut")
file(REMOVE_RECURSE "${C2_DONUT_BUILD_DIR}")
file(COPY "${C2_DONUT_SOURCE_DIR}" DESTINATION "${CMAKE_BINARY_DIR}/thirdParty")

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "ARM64" OR "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "^(ARM64|arm64|aarch64)$" OR "${MSVC_CXX_ARCHITECTURE_ID}" MATCHES "ARM64")
	set(C2_DONUT_MAKEFILE ".\\Makefile_arm64.msvc")
	set(C2_DONUT_VS_ARCH "arm64")

elseif("${MSVC_CXX_ARCHITECTURE_ID}" MATCHES "X86")
	set(C2_DONUT_MAKEFILE ".\\Makefile_x86.msvc")
	set(C2_DONUT_VS_ARCH "x86")

else()
	set(C2_DONUT_MAKEFILE ".\\Makefile.msvc")
	set(C2_DONUT_VS_ARCH "amd64")

endif()

execute_process(COMMAND cmd.exe /c cd ${C2_DONUT_BUILD_DIR} && "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Enterprise\\Common7\\Tools\\VsDevCmd.bat" -arch=${C2_DONUT_VS_ARCH} && nmake -f ${C2_DONUT_MAKEFILE} RESULT_VARIABLE ret)
if(NOT ret EQUAL "0")
	execute_process(COMMAND cmd.exe /c cd ${C2_DONUT_BUILD_DIR} && "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Community\\Common7\\Tools\\VsDevCmd.bat" -arch=${C2_DONUT_VS_ARCH} && nmake -f ${C2_DONUT_MAKEFILE} RESULT_VARIABLE ret)
endif()
if(NOT ret EQUAL "0")
	execute_process(COMMAND cmd.exe /c cd ${C2_DONUT_BUILD_DIR} && nmake -f ${C2_DONUT_MAKEFILE} RESULT_VARIABLE ret)
endif()
if(NOT ret EQUAL "0")
	message(FATAL_ERROR "Failed to build Donut with ${C2_DONUT_MAKEFILE}")
endif()

set(Donut "${C2_DONUT_BUILD_DIR}/lib/donut.lib" PARENT_SCOPE)
set(DonutDll "${C2_DONUT_BUILD_DIR}/lib/donut.dll" PARENT_SCOPE)
