########################################################################
# Check Python Modules
########################################################################
#PYTHON_CHECK_MODULE(
#    "Python version 2.6 or greater"
#    "platform" "platform.python_version() >= '2.6'"
#    HAVE_PYTHON_PLAT_MIN_VERSION
#)
#
#PYTHON_CHECK_MODULE(
#    "Cheetah templates 2.0.0 or greater"
#    "Cheetah" "Cheetah.Version >= '2.0.0'"
#    HAVE_PYTHON_MODULE_CHEETAH
#)

#IF(NOT HAVE_PYTHON_PLAT_MIN_VERSION OR NOT HAVE_PYTHON_MODULE_CHEETAH)
#    MESSAGE(FATAL_ERROR "Error: python requirements not met for the build system.")
#ENDIF(NOT HAVE_PYTHON_PLAT_MIN_VERSION OR NOT HAVE_PYTHON_MODULE_CHEETAH)
#

MESSAGE(\n "Ingresso nella cartella ../library/ "\n)
########################################################################
# Helpful Macros
########################################################################
MACRO(LIBLIBRARY_APPEND_SOURCES)
    LIST(APPEND libblocks_sources ${ARGV})
ENDMACRO(LIBLIBRARY_APPEND_SOURCES)

MACRO(LIBLIBRARY_APPEND_LIBS)
    LIST(APPEND libblocks_libs ${ARGV})
ENDMACRO(LIBLIBRARY_APPEND_LIBS)

MACRO(LIBLIBRARY_PYTHON_GEN_SOURCE pyfile outfile)
    #ensure that the directory exists for outfile
    GET_FILENAME_COMPONENT(outfile_dir ${outfile} PATH)
    FILE(MAKE_DIRECTORY ${outfile_dir})


#make the outfile depend on the python script
ADD_CUSTOM_COMMAND(
        OUTPUT ${outfile} DEPENDS ${pyfile}
        COMMAND ${PYTHON_EXECUTABLE} ${pyfile} ${outfile}
        COMMENT "Generating ${outfile}"
	)
	#make libuhd depend on the outfile
    	LIBLIBRARY_APPEND_SOURCES(${outfile})
ENDMACRO(LIBLIBRARY_PYTHON_GEN_SOURCE)

########################################################################
# Include CMakeLists.txt from subdirectories
########################################################################
#INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/randlib/CMakeLists.txt)
#ADD_SUBDIRECTORY(randlib)

########################################################################
# Append to the list of sources for lib uhd
########################################################################
FILE(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR} LOCAL_PKG_DATA_DIR)
STRING(REPLACE "\\" "\\\\" LOCAL_PKG_DATA_DIR ${LOCAL_PKG_DATA_DIR})
MESSAGE(STATUS "Local package data directory: ${LOCAL_PKG_DATA_DIR}")

IF(UNIX)
    #on unix systems, installers will use this directory for the package data
    FILE(TO_NATIVE_PATH /usr/${PKG_DATA_DIR} INSTALLER_PKG_DATA_DIR)
    STRING(REPLACE "\\" "\\\\" INSTALLER_PKG_DATA_DIR ${INSTALLER_PKG_DATA_DIR})
    MESSAGE(STATUS "Installer package data directory: ${INSTALLER_PKG_DATA_DIR}")
ENDIF(UNIX)

#CONFIGURE_FILE(
#    ${CMAKE_CURRENT_SOURCE_DIR}/constants.hpp.in
#    ${CMAKE_CURRENT_BINARY_DIR}/constants.hpp
#@ONLY)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

MESSAGE(\n "DIRECTORY CORRENTE  ${CMAKE_CURRENT_SOURCE_DIR}")
########################################################################
# Local Include Dir
########################################################################
SET(BLOCKS_LIB ${CMAKE_CURRENT_SOURCE_DIR}
	       ${CMAKE_CURRENT_SOURCE_DIR}/base
	      # ${CMAKE_SOURE_DIR}/mudisp/engine
)


INCLUDE_DIRECTORIES(${BLOCKS_LIB})

MESSAGE(STATUS "Sorgenti ${BLOCKS_LIB} ")


MESSAGE(STATUS " Directory Corrente Binary ${CMAKE_SOURCE_DIR}")

LIBLIBRARY_APPEND_SOURCES(
    	${CMAKE_CURRENT_SOURCE_DIR}/base/filter.cpp  
	${CMAKE_CURRENT_SOURCE_DIR}/base/write.cpp  
	#${CMAKE_CURRENT_SOURCE_DIR}/base/uhd_mudisp_usrp_sink.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/base/uhd_mudisp_usrp_source.cpp
)

MESSAGE(STATUS " File Nella Cartella Corrente ${libblocks_sources}")

########################################################################
# Setup libblock library
########################################################################
ADD_LIBRARY(blocks STATIC ${libblocks_sources})
TARGET_LINK_LIBRARIES(blocks ${Boost_LIBRARIES} ${libinesis_libs} ${libmudisp_libs} ${libblocks_lib} mudisp)
#TARGET_LINK_LIBRARIES(${libblocks_lib} mudisp)
SET_TARGET_PROPERTIES(blocks PROPERTIES DEFINE_SYMBOL "NEWBLOCKS_DLL_EXPORTS")

INSTALL(TARGETS blocks
    LIBRARY DESTINATION ${LIBRARY_DIR} # .so file
    ARCHIVE DESTINATION ${LIBRARY_DIR} # .lib file
    RUNTIME DESTINATION ${LIBRARY_DIR} # .dll file
)
	





