cmake_minimum_required(VERSION 3.16)
project(RTree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# 查找SEAL库
find_package(SEAL 3.7 REQUIRED)

# 包含头文件目录
include_directories(
    ${CMAKE_SOURCE_DIR}/../hom_matVecMul
    ${CMAKE_SOURCE_DIR}
    ${SEAL_INCLUDE_DIRS}
)

add_executable(RTree_main main.cpp
        RTree.h
        RTree.cpp
        Node.h
        Node.cpp
        DataProcessor.h
        DataProcessor.cpp
        EncRTree.h
        EncRTree.cpp
        EncNode.h
        EncNode.cpp
        DataOutsourcing.h
        DataOutsourcing.cpp
        Token.h
        Token.cpp
        #        RTree2.cpp
        #        RTree2.h
        #        RTree.cpp
#        RTree2.h
        )


# 链接SEAL库和gemini库
target_link_libraries(RTree_main gemini)
link_directories(${CMAKE_SOURCE_DIR}/../build/hom_matVecMul/hom_matVecMul)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build/lib)