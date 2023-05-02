cmake_minimum_required(VERSION 3.24)
project(DoubletsPuzzle)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
add_executable(DoubletsPuzzle
        main.cpp Graph.cpp Graph.h Node.cpp Node.h Node.cpp Node.h)
