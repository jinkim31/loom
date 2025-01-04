cmake_minimum_required(VERSION 3.28)
project(loom)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/imgui_bundle)
add_subdirectory(external/poco)

add_library(loom
        loom/threading/thread.cpp
        loom/threading/queue.cpp
        loom/threading/transmitter_receiver.cpp
        loom/threading/server_client.cpp
        loom/state_machine/state_machine.cpp
        loom/ui/ui.cpp
        loom/ui/style.cpp
        loom/comm/udp_client.cpp)

target_link_libraries(loom PRIVATE Poco::Net PUBLIC imgui_bundle)
target_include_directories(loom PUBLIC .)

add_executable(loom_example_threading_transmitter_receiver examples/threading/transmitter_receiver.cpp)
target_link_libraries(loom_example_threading_transmitter_receiver PRIVATE loom)

add_executable(loom_example_threading_server_client_async examples/threading/server_client_async.cpp)
target_link_libraries(loom_example_threading_server_client_async PRIVATE loom)

add_executable(loom_example_threading_server_client_sync examples/threading/server_client_sync.cpp)
target_link_libraries(loom_example_threading_server_client_sync PRIVATE loom)

add_executable(loom_example_threading_main_thread examples/threading/main_thread.cpp)
target_link_libraries(loom_example_threading_main_thread PRIVATE loom)

add_executable(loom_example_threading_transmitter_clone_functor examples/threading/transmitter_clone_functor.cpp)
target_link_libraries(loom_example_threading_transmitter_clone_functor PRIVATE loom)

add_executable(loom_example_state_machine_threaded_state_machine examples/state_machine/threaded_state_machine.cpp)
target_link_libraries(loom_example_state_machine_threaded_state_machine PRIVATE loom)

imgui_bundle_add_app(loom_example_ui_imgui_demo examples/ui/imgui_demo.cpp)
target_link_libraries(loom_example_ui_imgui_demo PRIVATE loom)

imgui_bundle_add_app(loom_example_ui_ui examples/ui/ui.cpp)
target_link_libraries(loom_example_ui_ui PRIVATE loom)

imgui_bundle_add_app(loom_example_ui_ui_threaded examples/ui/ui_threaded.cpp)
target_link_libraries(loom_example_ui_ui_threaded PRIVATE loom)
