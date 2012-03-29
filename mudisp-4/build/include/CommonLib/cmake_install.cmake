# Install script for directory: /home/massimo/MuDiSP4/mudisp-4/include/CommonLib

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local/mudisp4")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CommonLib" TYPE FILE FILES
    "/home/massimo/MuDiSP4/mudisp-4/include/CommonLib/BitVector.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/CommonLib/Configuration.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/CommonLib/F16.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/CommonLib/Interthread.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/CommonLib/LinkedLists.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/CommonLib/Logger.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/CommonLib/Regexp.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/CommonLib/Sockets.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/CommonLib/Threads.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/CommonLib/Timeval.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/CommonLib/Vector.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

