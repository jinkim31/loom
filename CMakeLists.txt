cmake_minimum_required(VERSION 3.28)
project(loom)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/imgui_bundle)

add_library(loom
        loom/threading/thread.cpp
        loom/threading/queue.cpp
        loom/threading/transmitter_receiver.cpp
        loom/threading/server_client.cpp
        loom/state_machine/state_machine.cpp)

target_include_directories(loom PUBLIC .)

add_executable(loom_test_threading_transmitter_receiver tests/threading/transmitter_receiver.cpp)
target_link_libraries(loom_test_threading_transmitter_receiver PRIVATE loom)

add_executable(loom_test_threading_server_client_async tests/threading/server_client_async.cpp)
target_link_libraries(loom_test_threading_server_client_async PRIVATE loom)

add_executable(loom_test_threading_server_client_sync tests/threading/server_client_sync.cpp)
target_link_libraries(loom_test_threading_server_client_sync PRIVATE loom)

add_executable(loom_test_threading_main_thread tests/threading/main_thread.cpp)
target_link_libraries(loom_test_threading_main_thread PRIVATE loom)

add_executable(loom_test_threading_transmitter_clone_functor tests/threading/transmitter_clone_functor.cpp)
target_link_libraries(loom_test_threading_transmitter_clone_functor PRIVATE loom)

add_executable(loom_test_state_machine_threaded_state_machine tests/state_machine/threaded_state_machine.cpp)
target_link_libraries(loom_test_state_machine_threaded_state_machine PRIVATE loom)

imgui_bundle_add_app(loom_test_ui_imgui_demo tests/ui/imgui_demo.cpp)
target_link_libraries(loom_test_ui_imgui_demo PRIVATE loom)