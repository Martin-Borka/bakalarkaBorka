# Install script for directory: /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/lib60870" TYPE FILE FILES
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/hal/inc/hal_time.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/hal/inc/hal_thread.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/hal/inc/hal_socket.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/hal/inc/hal_serial.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/hal/inc/tls_config.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/inc/api/cs101_master.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/inc/api/cs101_slave.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/inc/api/cs104_slave.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/inc/api/iec60870_master.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/inc/api/iec60870_slave.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/inc/api/iec60870_common.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/inc/api/cs101_information_objects.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/inc/api/cs104_connection.h"
    "/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/src/inc/api/link_layer_parameters.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug/lib60870-C/examples/cmake_install.cmake")
  include("/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug/lib60870-C/tests/cmake_install.cmake")
  include("/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug/lib60870-C/src/cmake_install.cmake")

endif()

