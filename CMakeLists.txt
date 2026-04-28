cmake_minimum_required(VERSION 3.13) 
project(simple_example) 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -DNDEBUG")

set(ENGINE_SOURCES
    src/matching_engine/matching_engine.cpp
    src/order_book/order.cpp
    src/order_book/order_book.cpp
    src/order_book/price_level.cpp
)

add_library(engine_lib STATIC ${ENGINE_SOURCES})

add_executable(OrderBookEngine src/main.cpp)
target_link_libraries(OrderBookEngine engine_lib)

find_package(benchmark QUIET)
if(benchmark_FOUND)
    add_executable(bench_engine benchmarks/benchmark_engine.cpp)
    target_link_libraries(bench_engine engine_lib benchmark::benchmark)
    message(STATUS "Google Benchmark found — benchmark target enabled")
else()
    message(STATUS "Google Benchmark NOT found — benchmark target disabled")
endif()
