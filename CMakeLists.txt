cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

project(Kletch)

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
