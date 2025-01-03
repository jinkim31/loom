cmake_minimum_required(VERSION 3.28)
project(loom)

set(CMAKE_CXX_STANDARD 17)

add_library(loom
        loom/thread.cpp
        loom/queue.cpp
        loom/itc.cpp
        loom/service.cpp)
target_include_directories(loom PUBLIC .)

add_executable(test_transmitter_receiver tests/transmitter_receiver.cpp)
target_link_libraries(test_transmitter_receiver PRIVATE loom)

add_executable(test_server_client tests/server_client.cpp)
target_link_libraries(test_server_client PRIVATE loom)