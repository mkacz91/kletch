cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(Kletch)

# Library kletch

set(LIBKLETCH_HEADERS
  src/hello.h
)

set(LIBKLETCH_SOURCES
  src/hello.cpp
)

add_library(libkletch ${LIBKLETCH_HEADERS} ${LIBKLETCH_SOURCES})
set_property(TARGET libkletch PROPERTY OUTPUT_NAME kletch)
set_property(TARGET libkletch PROPERTY CXX_STANDARD 14)
set_property(TARGET libkletch PROPERTY CXX_STANDARD_REQUIRED ON)

# Executable kletch

set(KLETCH_HEADERS

)

set(KLETCH_SOURCES
  src/kletch.cpp
)

add_executable(kletch ${KLETCH_HEADERS} ${KLETCH_SOURCES})
set_property(TARGET kletch PROPERTY CXX_STANDARD 14)
set_property(TARGET kletch PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(kletch libkletch)
