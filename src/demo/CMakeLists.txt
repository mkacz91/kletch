message(STATUS "Creating demo executable target: kletch")

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(AntTweakBar REQUIRED)

# Executable 'resemb'

add_executable(resemb resemb.cpp)
set_property(TARGET resemb PROPERTY CXX_STANDARD 11)
set_property(TARGET resemb PROPERTY CXX_STANDARD_REQUIRED ON)

# Generated embedded resource files

set(GEN_DIR "${CMAKE_BINARY_DIR}/gen")

file(GLOB KLETCH_SHADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" shaders/*.glsl)
file(MAKE_DIRECTORY "${GEN_DIR}/shaders")

set(RES_SUMMARY_H "${GEN_DIR}/summary.h")
set(RES_SUMMARY_CPP "${GEN_DIR}/summary.cpp")
file(WRITE "${RES_SUMMARY_H}"
  "namespace kletch {\n"
  "extern void add_resource(const char*, const char*);\n"
  "namespace gen {\n"
)
file(WRITE "${RES_SUMMARY_CPP}"
  "#inclde \"summary.h\"\n"
  "namespace kletch { \n"
  "void init_resources() {\n"
)

set(RES_INDEX 0)
foreach(RES_IN ${KLETCH_SHADERS})
  string(RANDOM RES_VARNAME)
  set(RES_VARNAME "r_${RES_VARNAME}_${RES_INDEX}")
  set(RES "${GEN_DIR}/${RES_IN}.cpp")
  list(APPEND KLETCH_RESOURCES "${RES}")

  add_custom_command(
    OUTPUT "${RES}"
    COMMAND resemb "${CMAKE_CURRENT_SOURCE_DIR}" "${GEN_DIR}" "${RES_IN}"
    MAIN_DEPENDENCY "${RES_IN}"
    DEPENDS resemb
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  )

  file(APPEND "${RES_SUMMARY_H}" "extern const char* ${RES_VARNAME};\n")
  file(APPEND "${RES_SUMMARY_CPP}" "add_resource(\"${RES_IN}\", gen::${RES_VARNAME});\n")

  math(EXPR RES_INDEX "${RES_INDEX} + 1")
endforeach(RES_IN)

file(APPEND "${RES_SUMMARY_H}" "}\n}\n")
file(APPEND "${RES_SUMMARY_CPP}" "}\n}\n")

# Executable 'kletch'

include_directories(
  ../
  ${GEN_DIR}/..
  ${SDL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${ANTTWEAKBAR_INCLUDE_DIR}
)

file(GLOB KLETCH_SOURCES *.cpp *.h)
file(GLOB KLETCH_SOURCE_EXCLUDES resemb.cpp)
list(REMOVE_ITEM KLETCH_SOURCES ${KLETCH_SOURCE_EXCLUDES})

if (APPLE)
  list(APPEND KLETCH_SOURCES SDLMain/SDLMain.m)
endif (APPLE)

add_executable(kletch ${KLETCH_SOURCES} ${KLETCH_RESOURCES})
set_property(TARGET kletch PROPERTY CXX_STANDARD 11)
set_property(TARGET kletch PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(kletch
  libkletch
  ${SDL_LIBRARY}
  ${OPENGL_gl_LIBRARY}
  ${ANTTWEAKBAR_LIBRARY}
)
