cmake_minimum_required(VERSION 3.16)
project(main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/RTree)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hom_matVecMul)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/eigen)


set(CMAKE_CXX_STANDARD 17)


#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/include)
#link_directories(${PROJECT_ROOT}/deps/lib)

# 添加HEXL库路径
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/lib/cmake)
find_package(HEXL REQUIRED)

# 设置HEXL库路径
#set(HEXL_ROOT "/home/zhang/sllll/my/RTree-main/deps/hexl")
#list(APPEND CMAKE_PREFIX_PATH "${HEXL_ROOT}/cmake")

# 设置HEXL库路径
#set(HEXL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")

#list(APPEND CMAKE_PREFIX_PATH "${HEXL_ROOT}")
#find_package(HEXL REQUIRED)

# 尝试查找SEAL
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/deps/SEAL/cmake)
find_package(SEAL 3.7 REQUIRED)  # 确保版本号与你安装的SEAL版本匹配


#file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/RTree/*.cpp)

add_subdirectory(RTree)
add_subdirectory(hom_matVecMul)

add_executable(allproject main.cpp)




target_link_libraries(allproject RTree SEAL::seal ${CMAKE_CURRENT_SOURCE_DIR}/build/lib/libhexl.a)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/build/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/RTree)
#target_link_libraries(allproject RTree hom_matVecMul)
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/RTree ${CMAKE_CURRENT_SOURCE_DIR}/hom_matVecMul)



