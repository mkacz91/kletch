message(STATUS "Creating test executable target: kletch_test")

find_package(Threads REQUIRED)

# Google test framework

# The Google Test framework is recommended not to be installed in binary form # but compiled with
# the project instead. So it is done here using ExternalProject. For details refer to:
# https://groups.google.com/forum/#!msg/googletestframework/Zo7_HOv1MJ0/F4ZBGjh_ePcJ
include(ExternalProject)
ExternalProject_Add(
  gtest_external
  URL https://github.com/google/googletest/archive/release-1.7.0.zip
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
)
ExternalProject_Get_Property(gtest_external SOURCE_DIR BINARY_DIR)
set(GTEST_SRC_DIR ${SOURCE_DIR})
set(GTEST_BIN_DIR ${BINARY_DIR})

include_directories(
  ../src/
  "${GTEST_SRC_DIR}/include")

add_library(gtest IMPORTED STATIC GLOBAL)
add_dependencies(gtest gtest_external)

set_target_properties(gtest PROPERTIES
  IMPORTED_LOCATION "${GTEST_BIN_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
)

# Generated test data

set(TEST_GEN_OUT_DIR="${GEN_DIR}/test")
set(TEST_GEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generators")
file(GLOB TEST_GENS RELATIVE "${TEST_GEN_DIR}" "${TEST_GEN_DIR}/*.py")
list(REMOVE_ITEM TEST_GENS "genutil.py")

foreach(TEST_GEN ${TEST_GENS})
  get_filename_component(GEN_NAME "${TEST_GEN}" NAME_WE)
  set(GEN_OUT_BASE "${TEST_GEN_OUT_DIR}/${GEN_NAME}")
endforeach(TEST_GENERATOR)

message(STATUS "TEST_GENERATOR_DIR: ${TEST_GENERATOR_DIR}")
message(STATUS "TEST_GENERATORS: ${TEST_GENERATORS}")

file(GLOB KLETCH_TEST_SOURCES *.cpp)

add_executable(kletch_test ${KLETCH_TEST_SOURCES})
target_link_libraries(kletch_test
  libkletch
  gtest
  ${CMAKE_THREAD_LIBS_INIT}
)
