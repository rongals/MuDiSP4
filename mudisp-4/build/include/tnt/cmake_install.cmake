# Install script for directory: /home/massimo/MuDiSP4/mudisp-4/include/tnt

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/TNT" TYPE FILE FILES
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/blas.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/cmat.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/fmat.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/index.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/qr.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/stpwatch.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/tnt.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/transv.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/vecadaptor.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/cholesky.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/fortran.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/lapack.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/region1d.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/subscrpt.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/tntmath.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/triang.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/vec.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/fcscmat.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/fspvec.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/lu.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/region2d.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/tntcmplx.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/tntreqs.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/trislv.h"
    "/home/massimo/MuDiSP4/mudisp-4/include/tnt/version.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

