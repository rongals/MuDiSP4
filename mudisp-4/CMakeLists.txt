#
# MuDiSP4 Makefile
# $Id: Makefile,v 1 2011/01/11 11:53:08 innocenti Exp $
#
#################################################
################################################
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(MUDSISP CXX)
ENABLE_TESTING()
MESSAGE(STATUS "Build Running on: ${CMAKE_SYSTEM_NAME} OS width ${CMAKE_SYSTEM_PROCESSOR} extension ")
########################################################################
# Config Files (include order is important)
########################################################################
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
INCLUDE(MUDISPComponent) #enable components
INCLUDE(MUDISPPackage)   #setup cpack
#INCLUDE(UHDComponent) 	 #enable components
#INCLUDE(UHDPackage)   	 #setup cpack
########################################################################
# Install Dirs
########################################################################
# Offer the user the choice of overriding the installation directories
SET(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
SET(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
SET(INSTALL_INCLUDE_DIR include CACHE PATH   "Installation directory for header files")
SET(INSTALL_DATA_DIR share CACHE PATH   "Installation directory for data files")
#

SET(LIB_SUFFIX ${LIB_SUFFIX} CACHE STRING "lib directory suffix")
SET(PKG_DATA_DIR share/mudisp)
SET(RUNTIME_DIR bin)
SET(LIBRARY_DIR lib${LIB_SUFFIX})
SET(INCLUDE_DIR include)
SET(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)
#SET(PKG_DATA_DIR share/uhd)
SET(TOOLS_RUNTIME_DIR tools)
SET(PKG_DOC_DIR  docs)
########################################################################
# Local Include Dir
########################################################################

#find the postgresql headers
#INCLUDE_DIRECTORIES(${INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

#MESSAGE(STATUS "Valore della Cartella ${HEADER_DIR}")
#file(GLOB itemviews-ng_SOURCES ${HEADER_DIR}/mudisp/engine/*.h )
#MESSAGE(STATUS "File nella cartella ${itemviews-ng_SOURCES}")

#MESSAGE ( STATUS "Vediamola qui ${INCLUDE_DIRECTORIES}")

# Flag di Compilazione
SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-O4 -c")
SET(CMAKE_C_FLAGS_DISTRIBUTION "-O4 -c")

# Impostazione dei Flag di compilazione per il DEBUG
SET(CMAKE_CXX_FLAGS_DEBUG "-g")



#Imposta le librerie
SET(BUILD_SHARED_LIBS ON)

#SET(CMAKE_BINARY_DIR ${CMAKE_INCLUDE_PATH} "/usr/local/libboost/include/boost")

#file(GLOB itemviews-ng_SOURCES ${INCLUDE_DIR}/tnt/*.h )
#MESSAGE(STATUS "File nella cartella ${itemviews-ng_SOURCES}")
#
#MESSAGE ( STATUS "Vediamola qui ${INCLUDE_DIRECTORIES}")
########################################################################
# Optional Compiler Flags
########################################################################
INCLUDE(CheckCXXCompilerFlag)
#for UHD
#MACRO(UHD_ADD_OPTIONAL_CXX_COMPILER_FLAG flag have)
#    CHECK_CXX_COMPILER_FLAG(${flag} ${have})
#    IF(${have})
#        ADD_DEFINITIONS(${flag})
#    ENDIF(${have})
#ENDMACRO(UHD_ADD_OPTIONAL_CXX_COMPILER_FLAG)

#for MuDiSP
MACRO(MUDISP_ADD_OPTIONAL_CXX_COMPILER_FLAG flag have)
    CHECK_CXX_COMPILER_FLAG(${flag} ${have})
    IF(${have})
        ADD_DEFINITIONS(${flag})
    ENDIF(${have})
ENDMACRO(MUDISP_ADD_OPTIONAL_CXX_COMPILER_FLAG)

#select the release build type by default to get optimization flags
IF(NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE "Release")
   MESSAGE(STATUS "Build type not specified: defaulting to release.")
ENDIF(NOT CMAKE_BUILD_TYPE)
SET(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "")

IF(CMAKE_COMPILER_IS_GNUCXX)
    ADD_DEFINITIONS(-Wall)
    ADD_DEFINITIONS(-Wextra)
    ADD_DEFINITIONS(-c)
    ADD_DEFINITIONS(-w)
    ADD_DEFINITIONS(-pthread) #-pthread
    #ADD_DEFINITIONS(-std=gnu++0x)
    ADD_DEFINITIONS(-ansi)
    #only export symbols that are declared to be part of the uhd api:
    MUDISP_ADD_OPTIONAL_CXX_COMPILER_FLAG(-fvisibility=hidden HAVE_VISIBILITY_HIDDEN)
    IF(NOT WIN32)
        #only export symbols that are declared to be part of the uhd api (non dll platforms)
   #     UHD_ADD_OPTIONAL_CXX_COMPILER_FLAG(-fvisibility=hidden HAVE_VISIBILITY_HIDDEN)
    ENDIF(NOT WIN32)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(MSVC)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/msvc)
    ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501) #minimum version required is windows xp
    ADD_DEFINITIONS(-DNOMINMAX) #disables stupidity and enables std::min and std::max
    ADD_DEFINITIONS( #stop all kinds of compatibility warnings
        -D_SCL_SECURE_NO_WARNINGS
        -D_SCL_SECURE_NO_DEPRECATE
        -D_CRT_SECURE_NO_WARNINGS
        -D_CRT_SECURE_NO_DEPRECATE
        -D_CRT_NONSTDC_NO_WARNINGS
        -D_CRT_NONSTDC_NO_DEPRECATE
    )
ENDIF(MSVC)

IF(CYGWIN)
    ADD_DEFINITIONS(-D__USE_W32_SOCKETS) #boost asio says we need this
ENDIF(CYGWIN)

IF(WIN32)
    ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501) #minimum version required is windows xp
    ADD_DEFINITIONS(-DNOMINMAX) #disables stupidity and enables std::min and std::max
ENDIF(WIN32)

########################################################################
# Setup Boost
########################################################################
MESSAGE(STATUS "")
MESSAGE(STATUS "Configuring Boost C++ Libraries...")
SET(BOOST_REQUIRED_COMPONENTS
    date_time
    filesystem
    program_options
    regex
    system
    thread
    unit_test_framework
)

IF(UNIX AND EXISTS "/usr/lib64")
    LIST(APPEND BOOST_LIBRARYDIR "/usr/lib64") #fedora 64-bit fix
ENDIF(UNIX AND EXISTS "/usr/lib64")

IF(MSVC)
    SET(BOOST_ALL_DYN_LINK "${BOOST_ALL_DYN_LINK}" CACHE BOOL "boost enable dynamic linking")
    IF(BOOST_ALL_DYN_LINK)
        ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK) #setup boost auto-linking in msvc
    ELSE(BOOST_ALL_DYN_LINK)
        UNSET(BOOST_REQUIRED_COMPONENTS) #empty components list for static link
    ENDIF(BOOST_ALL_DYN_LINK)
ENDIF(MSVC)

SET(Boost_ADDITIONAL_VERSIONS "1.42.0" "1.42" "1.43.0" "1.43" "1.44.0" "1.44" "1.45.0" "1.45" "1.46.0" "1.46" "1.47.0" "1.47"  "1.48.0" "1.48"
 "1.49.0" "1.49")
FIND_PACKAGE(Boost 1.36 COMPONENTS ${BOOST_REQUIRED_COMPONENTS})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

MESSAGE(STATUS "Boost include directories: ${Boost_INCLUDE_DIRS}")
MESSAGE(STATUS "Boost library directories: ${Boost_LIBRARY_DIRS}")
MESSAGE(STATUS "Boost libraries: ${Boost_LIBRARIES}")

########################################################################
# Setup LAPACK
########################################################################

#FIND_PACKAGE(clapack)


########################################################################
# Setup GNU Scientific Library
########################################################################
MESSAGE(STATUS "\n")

FIND_PACKAGE(GSL)
INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIRS})
LINK_DIRECTORIES(${GSL_LIBRARY_DIRS})



# std C libraries
#FIND_PACKAGE(stdlibs REQUIRED)

# libpthread with pthread.h
#FIND_PACKAGE(pthread REQUIRED)

########################################################################
# Check Python Modules
########################################################################
#INCLUDE(MUDISPPython)
#INCLUDE(UHDPython)

PYTHON_CHECK_MODULE(
    "Python version 2.6 or greater"
    "platform" "platform.python_version() >= '2.6'"
    HAVE_PYTHON_PLAT_MIN_VERSION
)

PYTHON_CHECK_MODULE(
    "Cheetah templates 2.0.0 or greater"
    "Cheetah" "Cheetah.Version >= '2.0.0'"
    HAVE_PYTHON_MODULE_CHEETAH
)

########################################################################
# Create Uninstall Target
########################################################################
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake
@ONLY)

ADD_CUSTOM_TARGET(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

########################################################################
# Create Pkg Config File
########################################################################
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/mudisp.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/mudisp.pc
@ONLY)

INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/mudisp.pc
    DESTINATION ${LIBRARY_DIR}/pkgconfig
	COMPONENT libraries
)


MESSAGE( "${CMAKE_SOURCE_DIR}/include")


########################################################################
# Install Package Docs
########################################################################
INSTALL(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/README
    ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
    ${CMAKE_CURRENT_SOURCE_DIR}/AUTHORS
    DESTINATION ${PKG_DOC_DIR}
  #  COMPONENT readme
)

########################################################################
# Register top level components
########################################################################
LIBMUDISP_REGISTER_COMPONENT("Docs" ENABLE_DOCS ON "ENABLE_LIBMUDISP" ON)
LIBMUDISP_REGISTER_COMPONENT("LibMUDISP" ENABLE_LIBMUDISP ON "Boost_FOUND;HAVE_PYTHON_PLAT_MIN_VERSION;HAVE_PYTHON_MODULE_CHEETAH" OFF)
#LIBUHD_REGISTER_COMPONENT("Examples" ENABLE_EXAMPLES ON "ENABLE_LIBUHD" OFF)
#LIBUHD_REGISTER_COMPONENT("Utils" ENABLE_UTILS ON "ENABLE_LIBUHD" OFF)
#LIBUHD_REGISTER_COMPONENT("Tests" ENABLE_TESTS ON "ENABLE_LIBUHD" OFF)

########################################################################
# Add the subdirectories
########################################################################
IF(ENABLE_DOCS)
    ADD_SUBDIRECTORY(docs)
ENDIF(ENABLE_DOCS)

#IF(ENABLE_EXAMPLES)
#    ADD_SUBDIRECTORY(examples)
#ENDIF(ENABLE_EXAMPLES)

ADD_SUBDIRECTORY(include)

#ADD_SUBDIRECTORY(lib)

IF(ENABLE_LIBMUDISP)
   ADD_SUBDIRECTORY(lib)
ENDIF(ENABLE_LIBMUDISP)

#IF(ENABLE_TESTS)
#    ADD_SUBDIRECTORY(tests)
#ENDIF(ENABLE_TESTS)

#IF(ENABLE_UTILS)
#    ADD_SUBDIRECTORY(utils)#
#	ADD_SUBDIRECTORY(tools)
#ENDIF(ENABLE_UTILS)

#ADD_SUBDIRECTORY(usrp_e_utils)


########################################################################
# Install Makefile.sim
########################################################################
INSTALL(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.sim
    DESTINATION ${LIBRARY_DIR}
)

########################################################################
# Print Summary
########################################################################
MUDISP_PRINT_COMPONENT_SUMMARY()
#UHD_PRINT_COMPONENT_SUMMARY()
MESSAGE(STATUS "Building version: ${CPACK_PACKAGE_VERSION}")
MESSAGE(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")


