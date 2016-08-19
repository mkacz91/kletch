message(STATUS "Creating asset library target: asset")

file(GLOB ASSET_SOURCES *.cpp *.h)

add_library(libasset ${ASSET_SOURCES})
set_property(TARGET libasset PROPERTY OUTPUT_NAME asset)
