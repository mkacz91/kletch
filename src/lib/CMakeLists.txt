message(STATUS "Creating library target: libkletch")

include_directories(
  ../
)

file(GLOB LIBKLETCH_SOURCES *.cpp *.h)

add_library(libkletch ${LIBKLETCH_SOURCES})
set_property(TARGET libkletch PROPERTY OUTPUT_NAME kletch)
set_property(TARGET libkletch PROPERTY CXX_STANDARD 11)
set_property(TARGET libkletch PROPERTY CXX_STANDARD_REQUIRED ON)
