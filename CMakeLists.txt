cmake_minimum_required(VERSION 3.13) 
project(simple_example) 
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp src/matching_engine/matching_engine.cpp src/order_book/order.cpp src/order_book/order_book.cpp src/order_book/price_level.cpp)

add_executable(OrderBookEngine ${SOURCE_FILES})

