cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
cmake_policy(SET CMP0004 OLD)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

project(Kletch)

# Core library 'libkletch'

set(LIBKLETCH_SOURCES
  src/hello.cpp
)

add_library(libkletch ${LIBKLETCH_SOURCES})
set_property(TARGET libkletch PROPERTY OUTPUT_NAME kletch)
set_property(TARGET libkletch PROPERTY CXX_STANDARD 14)
set_property(TARGET libkletch PROPERTY CXX_STANDARD_REQUIRED ON)

# Demo executable 'kletch'

if (NOT DEFINED BUILD_DEMO)
  message(STATUS "lol")
  set(BUILD_DEMO YES)
endif (NOT DEFINED BUILD_DEMO)
if (BUILD_DEMO)
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
  set_property(TARGET kletch PROPERTY CXX_STANDARD 14)
  set_property(TARGET kletch PROPERTY CXX_STANDARD_REQUIRED ON)
  target_link_libraries(kletch
    libkletch
    ${SDL_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${ANTTWEAKBAR_LIBRARY}
  )
  target_include_directories(kletch PUBLIC ${SDL_INCLUDE_DIR})
  target_include_directories(kletch PUBLIC ${OPENGL_INCLUDE_DIR})
  target_include_directories(kletch PUBLIC ${ANTTWEAKBAR_INCLUDE_DIR})
endif (BUILD_DEMO)

# Executable kletch_test

if (NOT DEFINED BUILD_TEST)
  set (BUILD_TEST NO)
endif (NOT DEFINED BUILD_TEST)
if (BUILD_TEST)
  find_package(GTEST REQUIRED)

  set(KLETCH_TEST_SOURCES
    test/hello_test.cpp
    test/main.cpp
  )

  add_executable(kletch_test ${KLETCH_TEST_SOURCES})
  set_property(TARGET kletch_test PROPERTY CXX_STANDARD 14)
  set_property(TARGET kletch_test PROPERTY CXX_STANDARD_REQUIRED ON)
  target_link_libraries(kletch_test libkletch ${GTEST_LIBRARY})
  target_include_directories(kletch_test PUBLIC ${GTEST_INCLUDE_DIR})
endif (BUILD_TEST)
