cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
set(CMAKE_COLOR_MAKEFILE ON)

project(Kletch)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory(src/lib)

if (NOT DEFINED BUILD_DEMO)
  set(BUILD_DEMO YES)
endif (NOT DEFINED BUILD_DEMO)
if (BUILD_DEMO)
  add_subdirectory(src/demo)
endif (BUILD_DEMO)

if (NOT DEFINED BUILD_TEST)
  set (BUILD_TEST NO)
endif (NOT DEFINED BUILD_TEST)
if (BUILD_TEST)
  add_subdirectory(test)
endif (BUILD_TEST)
