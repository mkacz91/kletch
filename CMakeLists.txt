cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
cmake_policy(SET CMP0004 OLD)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

project(Kletch)

# Core library 'libkletch'

message(STATUS "Creating library target: libkletch")

set(LIBKLETCH_SOURCES
  src/hello.cpp
)

add_library(libkletch ${LIBKLETCH_SOURCES})
set_property(TARGET libkletch PROPERTY OUTPUT_NAME kletch)
set_property(TARGET libkletch PROPERTY CXX_STANDARD 11)
set_property(TARGET libkletch PROPERTY CXX_STANDARD_REQUIRED ON)

# Demo executable 'kletch'

if (NOT DEFINED BUILD_DEMO)
  set(BUILD_DEMO YES)
endif (NOT DEFINED BUILD_DEMO)
if (BUILD_DEMO)
  message(STATUS "Creating demo executable target: kletch")

  find_package(SDL REQUIRED)
  find_package(OpenGL REQUIRED)
  find_package(AntTweakBar REQUIRED)

  set(KLETCH_SOURCES
    src/kletch.cpp
  )

  if (APPLE)
    set(KLETCH_SOURCES ${KLETCH_SOURCES} SDLMain/SDLMain.m)
  endif (APPLE)

  add_executable(kletch ${KLETCH_SOURCES})
  set_property(TARGET kletch PROPERTY CXX_STANDARD 11)
  set_property(TARGET kletch PROPERTY CXX_STANDARD_REQUIRED ON)
  set_property(TARGET kletch PROPERTY INCLUDE_DIRECTORIES
    ${SDL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${ANTTWEAKBAR_INCLUDE_DIR}
  )
  target_link_libraries(kletch
    libkletch
    ${SDL_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${ANTTWEAKBAR_LIBRARY}
  )
endif (BUILD_DEMO)

# Executable 'kletch_test'

if (NOT DEFINED BUILD_TEST)
  set (BUILD_TEST NO)
endif (NOT DEFINED BUILD_TEST)
if (BUILD_TEST)
  message(STATUS "Creating test executable target: kletch_test")

  find_package(Threads REQUIRED)

  # The Google Test framework is recommended not to be installed in binary form
  # but compiled with the project instead. So it is done here using
  # ExternalProject.
  # For details refer to:
  # https://groups.google.com/forum/#!msg/googletestframework/Zo7_HOv1MJ0/F4ZBGjh_ePcJ
  include(ExternalProject)
  ExternalProject_Add(
    gtest_external
    URL https://github.com/google/googletest/archive/release-1.7.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
  )
  ExternalProject_Get_Property(gtest_external SOURCE_DIR BINARY_DIR)
  set(GTEST_SRC_DIR ${SOURCE_DIR})
  set(GTEST_BIN_DIR ${BINARY_DIR})
  set(GTEST_INCLUDE_DIR "${GTEST_SRC_DIR}/include")

  add_library(gtest IMPORTED STATIC GLOBAL)
  add_dependencies(gtest gtest_external)

  set_target_properties(gtest PROPERTIES
    IMPORTED_LOCATION "${GTEST_BIN_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
  )

  set(KLETCH_TEST_SOURCES
    test/hello_test.cpp
    test/main.cpp
  )

  add_executable(kletch_test ${KLETCH_TEST_SOURCES})
  set_property(TARGET kletch_test PROPERTY CXX_STANDARD 11)
  set_property(TARGET kletch_test PROPERTY CXX_STANDARD_REQUIRED ON)
  set_property(TARGET kletch_test PROPERTY INCLUDE_DIRECTORIES
    ${GTEST_INCLUDE_DIR}
  )
  message(STATUS "GTEST_INCLUDE_DIR=${GTEST_INCLUDE_DIR}")
  target_link_libraries(kletch_test
    libkletch
    gtest
    ${CMAKE_THREAD_LIBS_INIT}
  )
endif (BUILD_TEST)
