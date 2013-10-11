########################################################################
# Find the library for the USRP Hardware Driver
########################################################################
if(NOT UHD_FOUND)
INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_UHD uhd)

FIND_PATH(
    UHD_INCLUDE_DIR
    NAMES uhd/config.hpp
    HINTS $ENV{UHD_DIR}/include
        ${PC_UHD_INCLUDEDIR}
    PATHS /usr/local/include
          /usr/include
)

#find_path(UHD_INCLUDE_DIR NAMES libusb.h
#    PATHS
#    ${LIBUSB_PKG_INCLUDE_DIRS}
#    /usr/include/libusb-1.0
#    /usr/include
#    /usr/local/include
#  )


FIND_LIBRARY(
    UHD_LIBRARIES
    NAMES uhd
    HINTS $ENV{UHD_DIR}/lib
        ${PC_UHD_LIBDIR}
    PATHS /usr/local/lib
          /usr/lib
)
message(STATUS "UHD include dir: ${UHD_INCLUDE_DIR}")
message(STATUS "UHD include dir: ${UHD_INCLUDE_DIR}")
#INCLUDE(FindPackageHandleStandardArgs)
#FIND_PACKAGE_HANDLE_STANDARD_ARGS(UHD DEFAULT_MSG UHD_LIBRARIES UHD_INCLUDE_DIRS)
if(UHD_INCLUDE_DIR AND UHD_LIBRARIES)
  set(UHD_FOUND TRUE CACHE INTERNAL "libuhd found")
  message(STATUS "Found libuhd: ${UHD_INCLUDE_DIR}, ${UHD_LIBRARIES}")
else(UHD_INCLUDE_DIR AND UHD_LIBRARIES)
  set(UHD_FOUND FALSE CACHE INTERNAL "libuhd found")
  message(STATUS "libuhd not found.")
endif(UHD_INCLUDE_DIR AND UHD_LIBRARIES)

MARK_AS_ADVANCED(UHD_LIBRARIES UHD_INCLUDE_DIRS)
endif(NOT UHD_FOUND)



