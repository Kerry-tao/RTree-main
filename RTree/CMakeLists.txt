cmake_minimum_required(VERSION 3.16)
project(RTree)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(RTree STATIC
        RTree.cpp
        Node.cpp
        )

