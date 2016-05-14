message(STATUS "Creating library target: libkletch")

include_directories(
  ../
)

file(GLOB LIBKLETCH_SOURCES *.cpp *.h)

add_library(libkletch ${LIBKLETCH_SOURCES})
set_property(TARGET libkletch PROPERTY OUTPUT_NAME kletch)
