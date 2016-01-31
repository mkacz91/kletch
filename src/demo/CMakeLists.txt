message(STATUS "Creating demo executable target: kletch")

add_executable(resemb resemb.cpp)
set_property(TARGET resemb PROPERTY CXX_STANDARD 11)
set_property(TARGET resemb PROPERTY CXX_STANDARD_REQUIRED ON)

file(GLOB KLETCH_SHADERS shaders/*.glsl)
set(GEN_DIR "${CMAKE_BINARY_DIR}/gen")
set(RESEMB_SHADERS_ARGS
  "--prefix" "${CMAKE_CURRENT_SOURCE_DIR}/"
  "${GEN_DIR}/shaders.h"
  "${GEN_DIR}/shaders.cpp"
)
list(APPEND RESEMB_SHADERS_ARGS ${KLETCH_SHADERS})

file(MAKE_DIRECTORY "${GEN_DIR}")
add_custom_target(resemb_shaders resemb ${RESEMB_SHADERS_ARGS})

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(AntTweakBar REQUIRED)

include_directories(
  ../
  ${GEN_DIR}/..
  ${SDL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${ANTTWEAKBAR_INCLUDE_DIR}
)

file(GLOB KLETCH_SOURCES *.cpp)
file(GLOB KLETCH_SOURCE_EXCLUDES resemb.cpp)
list(REMOVE_ITEM KLETCH_SOURCES ${KLETCH_SOURCE_EXCLUDES})

if (APPLE)
  list(APPEND KLETCH_SOURCES SDLMain/SDLMain.m)
endif (APPLE)

add_executable(kletch ${KLETCH_SOURCES})
set_property(TARGET kletch PROPERTY CXX_STANDARD 11)
set_property(TARGET kletch PROPERTY CXX_STANDARD_REQUIRED ON)
add_dependencies(kletch resemb_shaders)
target_link_libraries(kletch
  libkletch
  ${SDL_LIBRARY}
  ${OPENGL_gl_LIBRARY}
  ${ANTTWEAKBAR_LIBRARY}
)
