#cmake_minimum_required(VERSION 3.16)
#project(hom_mul)

# Specify the C++ standard
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED True)

# Microsoft SEAL
#find_package(SEAL 3.7 REQUIRED)  # 根据你实际安装的SEAL版本调整

# Include directories
#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Source files list
#set(SOURCE_FILES
    #logging.cc
    #math.cc
    #matVecMul.cpp
    #shape_inference.cc
    #tensor_shape.cc
    #types.cc
#)

# Add static library
#add_library(hom_matVecMul STATIC ${SOURCE_FILES})

# Link with Microsoft SEAL
#target_link_libraries(hom_matVecMul SEAL::seal)



cmake_minimum_required(VERSION 3.16)

# 项目名称
project(hom_matVec)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SEAL 3.7 REQUIRED)

# 添加源文件
set(SOURCES
    logging.cc
    math.cc
    shape_inference.cc
    tensor_shape.cc
    types.cc
    matVecMul.cpp
   
)

# 添加HEXL库路径
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../build/lib/cmake)
find_package(HEXL REQUIRED)

find_package(Eigen3 3.3.3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})


# 输出调试信息
message(STATUS "HEXL library path: ${CMAKE_CURRENT_SOURCE_DIR}/../build/lib/libhexl.a")
message(STATUS "SEAL library found: ${SEAL_FOUND}")
message(STATUS "HEXL library found: ${HEXL_FOUND}")

# 尝试查找SEAL
#list(APPEND CMAKE_PREFIX_PATH "/home/zhang/sllll/my/RTree-main/build/deps/SEAL/cmake")
#find_package(SEAL 3.7 REQUIRED)  # 确保版本号与你安装的SEAL版本匹配

# 添加HEXL库路径
#list(APPEND CMAKE_PREFIX_PATH "/home/zhang/sllll/my/RTree-main/deps/hexl")
#find_package(HEXL REQUIRED)

# 设置HEXL库路径
#set(HEXL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../build/lib")

#list(APPEND CMAKE_PREFIX_PATH "${HEXL_ROOT}")
#find_package(HEXL REQUIRED)

add_library(gemini SHARED ${SOURCES})
target_link_libraries(gemini SEAL::seal Eigen3::Eigen)


# 添加头文件目录
include_directories(${CMAKE_SOURCE_DIR} ${SEAL_INCLUDE_DIRS})

# 生成可执行文件
add_executable(test_hom_matVec test_hom_matVec.cpp)

# 链接 SEAL 库
target_link_libraries(test_hom_matVec gemini SEAL::seal ${CMAKE_CURRENT_SOURCE_DIR}/../build/lib/libhexl.a)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build/lib)