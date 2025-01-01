cmake_minimum_required(VERSION 3.28)
project(loom)

set(CMAKE_CXX_STANDARD 17)

add_executable(loom
        main.cpp
        thread.cpp
        queue.cpp
        itc.cpp)
