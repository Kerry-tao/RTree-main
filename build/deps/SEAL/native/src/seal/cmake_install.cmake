# Install script for directory: /home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/zhang/sllll/new-cheetah/OpenCheetah-main/build")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SEAL-3.7/seal" TYPE FILE FILES
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/batchencoder.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/ciphertext.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/ckks.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/modulus.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/context.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/decryptor.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/dynarray.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/encryptionparams.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/encryptor.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/evaluator.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/galoiskeys.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/keygenerator.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/kswitchkeys.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/memorymanager.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/plaintext.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/publickey.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/randomgen.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/randomtostd.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/relinkeys.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/seal.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/secretkey.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/serializable.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/serialization.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/valcheck.h"
    "/home/zhang/sllll/new-cheetah/OpenCheetah-main/deps/SEAL/native/src/seal/version.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/zhang/sllll/new-cheetah/OpenCheetah-main/build/deps/SEAL/native/src/seal/util/cmake_install.cmake")

endif()

