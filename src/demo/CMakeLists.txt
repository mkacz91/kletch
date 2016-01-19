message(STATUS "Creating demo executable target: kletch")

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(AntTweakBar REQUIRED)

include_directories(
  ../
  ${SDL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${ANTTWEAKBAR_INCLUDE_DIR}
)

file(GLOB KLETCH_SOURCES *.cpp)

if (APPLE)
  list(APPEND KLETCH_SOURCES SDLMain/SDLMain.m)
endif (APPLE)

add_executable(kletch ${KLETCH_SOURCES})
set_property(TARGET kletch PROPERTY CXX_STANDARD 11)
set_property(TARGET kletch PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(kletch
  libkletch
  ${SDL_LIBRARY}
  ${OPENGL_gl_LIBRARY}
  ${ANTTWEAKBAR_LIBRARY}
)
