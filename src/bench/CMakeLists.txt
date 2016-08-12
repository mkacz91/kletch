message(STATUS "Creating executable target: kletch_bench")

include_directories(
  ../
)

file(GLOB KLETCH_BENCH_SOURCES *.cpp *.h)

add_executable(kletch_bench ${KLETCH_BENCH_SOURCES})
target_link_libraries(kletch_bench
  libkletch
)
