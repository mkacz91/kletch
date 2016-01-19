message(STATUS "Creating library target: libkletch")

file(GLOB LIBKLETCH_SOURCES *.cpp)

add_library(libkletch ${LIBKLETCH_SOURCES})
set_property(TARGET libkletch PROPERTY OUTPUT_NAME kletch)
set_property(TARGET libkletch PROPERTY CXX_STANDARD 11)
set_property(TARGET libkletch PROPERTY CXX_STANDARD_REQUIRED ON)
