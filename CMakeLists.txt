cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
set(CMAKE_COLOR_MAKEFILE ON)

project(Kletch)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-DKLETCH_CLANG)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_definitions(-DKLETCH_GNU)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_definitions(-DKLETCH_MSVC)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  add_definitions(-DKLETCH_CLANG)
  add_definitions(-DKLETCH_APPLECLANG)
endif()

if (DEFINED WIN32)
  add_definitions(-DKLETCH_WINDOWS)
endif()

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if (WITH_DOUBLE)
  add_definitions(-DKLETCH_WITH_DOUBLE)
endif()

add_subdirectory(src/lib)

add_subdirectory(src/asset) # TODO: conditionally, depending on BUILD_DEMO and BUILD_TEST
set(ASSC "${CMAKE_SOURCE_DIR}/src/asset/assc.py")

if (NOT DEFINED WITH_DEMO OR WITH_DEMO)
  add_subdirectory(src/demo)
endif()

if (WITH_TEST)
  add_subdirectory(test)
endif()

if (WITH_BENCH)
  add_subdirectory(src/bench)
endif()
