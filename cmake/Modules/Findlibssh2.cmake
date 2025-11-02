include_guard(GLOBAL)

include(FetchContent)

if(TARGET libssh2::libssh2)
  get_target_property(_libssh2_inc libssh2::libssh2 INTERFACE_INCLUDE_DIRECTORIES)
  set(LIBSSH2_INCLUDE_DIR "${_libssh2_inc}")
  set(LIBSSH2_INCLUDE_DIRS "${_libssh2_inc}")
  set(LIBSSH2_LIBRARIES libssh2::libssh2)
  set(LIBSSH2_LIBRARY libssh2::libssh2)
  set(LIBSSH2_VERSION "1.11.0")
  set(LIBSSH2_FOUND TRUE)
  mark_as_advanced(LIBSSH2_INCLUDE_DIR LIBSSH2_LIBRARY)
  unset(_libssh2_inc)
  return()
endif()

set(_libssh2_cmake_args
  -DBUILD_SHARED_LIBS=OFF
  -DLIBSSH2_BUILD_SHARED_LIBS=OFF
  -DLIBSSH2_BUILD_STATIC_LIBS=ON
  -DBUILD_EXAMPLES=OFF
  -DBUILD_TESTING=OFF
  -DLIBSSH2_WITH_WINCNG=ON
  -DLIBSSH2_WITH_OPENSSL=OFF
  -DLIBSSH2_WITH_GCRYPT=OFF
)

FetchContent_Declare(
  libssh2
  GIT_REPOSITORY https://github.com/libssh2/libssh2.git
  GIT_TAG        libssh2-1.11.0
  CMAKE_ARGS     ${_libssh2_cmake_args}
)

FetchContent_MakeAvailable(libssh2)

set(_libssh2_target "")
foreach(_candidate libssh2 libssh2_static libssh2_shared)
  if(TARGET ${_candidate})
    set(_libssh2_target ${_candidate})
    break()
  endif()
endforeach()
unset(_candidate)

if(NOT _libssh2_target)
  message(FATAL_ERROR "libssh2 target not available after FetchContent")
endif()

if(NOT TARGET libssh2::libssh2)
  add_library(libssh2::libssh2 ALIAS ${_libssh2_target})
endif()

get_target_property(_libssh2_type libssh2::libssh2 TYPE)
if(_libssh2_type STREQUAL "STATIC_LIBRARY")
  target_compile_definitions(libssh2::libssh2 INTERFACE LIBSSH2_STATIC)
endif()
unset(_libssh2_type)

get_target_property(_libssh2_inc libssh2::libssh2 INTERFACE_INCLUDE_DIRECTORIES)
if(NOT _libssh2_inc)
  set(_libssh2_inc "${libssh2_SOURCE_DIR}/include")
  target_include_directories(libssh2::libssh2 INTERFACE "${_libssh2_inc}")
endif()

set(LIBSSH2_INCLUDE_DIR "${_libssh2_inc}")
set(LIBSSH2_INCLUDE_DIRS "${_libssh2_inc}")
set(LIBSSH2_LIBRARIES libssh2::libssh2)
set(LIBSSH2_LIBRARY libssh2::libssh2)
set(LIBSSH2_VERSION "1.11.0")
set(LIBSSH2_FOUND TRUE)

mark_as_advanced(LIBSSH2_INCLUDE_DIR LIBSSH2_LIBRARY)

unset(_libssh2_inc)
unset(_libssh2_target)
unset(_libssh2_cmake_args)
