# Install script for directory: /home/zhang/sllll/my/RTree-main/deps/eigen/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/zhang/sllll/my/RTree-main/build")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE FILE FILES
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/Cholesky"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/CholmodSupport"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/Core"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/Dense"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/Eigen"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/Eigenvalues"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/Geometry"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/Householder"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/IterativeLinearSolvers"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/Jacobi"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/LU"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/MetisSupport"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/OrderingMethods"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/PaStiXSupport"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/PardisoSupport"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/QR"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/QtAlignedMalloc"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/SPQRSupport"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/SVD"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/Sparse"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/SparseCholesky"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/SparseCore"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/SparseLU"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/SparseQR"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/StdDeque"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/StdList"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/StdVector"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/SuperLUSupport"
    "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/UmfPackSupport"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE DIRECTORY FILES "/home/zhang/sllll/my/RTree-main/deps/eigen/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

