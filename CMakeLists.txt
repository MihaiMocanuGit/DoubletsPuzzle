cmake_minimum_required(VERSION 3.24)
project(DoubletsPuzzle)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

add_subdirectory(Libs)
add_subdirectory(Tests)


add_executable(DoubletsPuzzle main.cpp)

target_link_libraries(DoubletsPuzzle Libs)






