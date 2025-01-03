cmake_minimum_required(VERSION 3.28)
project(loom)

set(CMAKE_CXX_STANDARD 17)

add_library(loom
        loom/threading/thread.cpp
        loom/threading/queue.cpp
        loom/threading/transmitter_receiver.cpp
        loom/threading/server_client.cpp
        loom/state_machine/state_machine.cpp)

target_include_directories(loom PUBLIC .)

add_executable(test_transmitter_receiver tests/transmitter_receiver.cpp)
target_link_libraries(test_transmitter_receiver PRIVATE loom)

add_executable(test_server_client_async tests/server_client_async.cpp)
target_link_libraries(test_server_client_async PRIVATE loom)

add_executable(test_server_client_sync tests/server_client_sync.cpp)
target_link_libraries(test_server_client_sync PRIVATE loom)

add_executable(test_main_thread tests/main_thread.cpp)
target_link_libraries(test_main_thread PRIVATE loom)

add_executable(test_transmitter_clone_functor tests/transmitter_clone_functor.cpp)
target_link_libraries(test_transmitter_clone_functor PRIVATE loom)

add_executable(test_state_machine tests/state_machine.cpp)
target_link_libraries(test_state_machine PRIVATE loom)

