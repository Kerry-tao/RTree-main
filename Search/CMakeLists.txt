cmake_minimum_required(VERSION 3.16)

# 项目名称
project(RTreeProtocol)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找SEAL库
find_package(SEAL 3.7 REQUIRED)

# 包含头文件目录
include_directories(
    ${CMAKE_SOURCE_DIR}/../hom_matVecMul
    ${SEAL_INCLUDE_DIRS}
)

# 添加可执行文件
# add_executable(protocol
#     protocol.cpp
# )

# 链接SEAL库和gemini库
# target_link_libraries(protocol gemini)
# link_directories(${CMAKE_SOURCE_DIR}/../build/hom_matVecMul/hom_matVecMul)
# link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build/lib)

