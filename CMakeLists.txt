cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
cmake_policy(SET CMP0004 OLD)

project(Kletch)

# Library kletch

set(LIBKLETCH_SOURCES
  src/hello.cpp
)

add_library(libkletch ${LIBKLETCH_SOURCES})
set_property(TARGET libkletch PROPERTY OUTPUT_NAME kletch)
set_property(TARGET libkletch PROPERTY CXX_STANDARD 14)
set_property(TARGET libkletch PROPERTY CXX_STANDARD_REQUIRED ON)

# Executable kletch

find_package(SDL)
if (NOT SDL_FOUND)
  message(WARNING "Could not find SDL. Target 'kletch' won't be created.")
  return()
endif (NOT SDL_FOUND)

set(KLETCH_SOURCES
  src/kletch.cpp
)

if (APPLE)
  set(KLETCH_SOURCES ${KLETCH_SOURCES} SDLMain/SDLMain.m)
endif (APPLE)

add_executable(kletch ${KLETCH_SOURCES})
set_property(TARGET kletch PROPERTY CXX_STANDARD 14)
set_property(TARGET kletch PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(kletch libkletch ${SDL_LIBRARY})
target_include_directories(kletch PUBLIC ${SDL_INCLUDE_DIR})
