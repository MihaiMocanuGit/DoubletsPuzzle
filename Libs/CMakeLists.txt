cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)

add_library(Libs
        Graph/Graph.cpp Graph/Node.cpp Tools/BFS.cpp Tools/BFS.h GraphGenerator/Generator.cpp GraphGenerator/Generator.h
        Application/Application.cpp Application/Application.h UI/UI.cpp UI/UI.h Logging/UserInfo.cpp Logging/UserInfo.h)