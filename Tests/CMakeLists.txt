cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)


find_package(Catch2 3 REQUIRED)


# These tests can use the Catch2-provided main
add_executable(tests test.cpp
                Test_Graph/test_Graph.cpp Test_Graph/test_Node.cpp Test_Tools/test_BFS.cpp)



include_directories(../Libs)


target_link_libraries(tests PRIVATE Libs Catch2::Catch2WithMain)
