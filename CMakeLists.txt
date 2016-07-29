cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)
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

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#et(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(GEN_DIR "${CMAKE_BINARY_DIR}/gen")

if (WITH_DOUBLE)
  add_definitions(-DKLETCH_WITH_DOUBLE)
endif()

add_subdirectory(src/lib)

if (NOT DEFINED WITH_DEMO OR WITH_DEMO)
  add_subdirectory(src/demo)
endif()

if (WITH_TEST)
  add_subdirectory(test)
endif()
