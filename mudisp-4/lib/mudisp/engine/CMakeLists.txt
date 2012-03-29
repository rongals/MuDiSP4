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


message("\n\Ingresso nella cartella .\mudisp\")
########################################################################
# Helpful Macros
########################################################################
MACRO(LIBMUDISP_APPEND_SOURCES)
    LIST(APPEND LIBMUDISP_sources ${ARGV})
ENDMACRO(LIBMUDISP_APPEND_SOURCES)

MACRO(LIBMUDISP_APPEND_LIBS)
    LIST(APPEND LIBMUDISP_libs ${ARGV})
ENDMACRO(LIBMUDISP_APPEND_LIBS)

MACRO(LIBMUDISP_PYTHON_GEN_SOURCE pyfile outfile)
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
    LIBMUDISP_APPEND_SOURCES(${outfile})
ENDMACRO(LIBMUDISP_PYTHON_GEN_SOURCE)

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

########################################################################
# Local Include Dir
########################################################################
SET(MUDISP_LIB ${CMAKE_SOURE_DIR}/mudisp
	       ${CMAKE_SOURE_DIR}/mudisp/engine
)

INCLUDE_DIRECTORIES(${MUDISP_LIB})

INSTALL(FILES
	errors.h  
	main.h    
	param.h
	block.h              
	lcm.h     
	mudisp.h  
	parlist.h  
	port.h
	DESTINATION  ${INCLUDE_DIR}
)

MESSAGE(STATUS " Directory Corrente ${CMAKE_SOURCE_DIR}")

LIBMUDISP_APPEND_SOURCES(
    	${CMAKE_SOURCE_DIR}/mudisp/engine/block.cpp  
	${CMAKE_SOURCE_DIR}/mudisp/engine/lcm.cpp  
	${CMAKE_SOURCE_DIR}/mudisp/engine/param.cpp  
	${CMAKE_SOURCE_DIR}/mudisp/engine/parlist.cpp  
	${CMAKE_SOURCE_DIR}/mudisp/engine/port.cpp  
	${CMAKE_SOURCE_DIR}/mudisp/engine/stdfun.cpp
)

MESSAGE(STATUS " Directory Corrente ${libmudisp_sources}")

########################################################################
# Setup libuhd library
########################################################################
ADD_LIBRARY(mudisp STATIC ${libmudisp_sources})
TARGET_LINK_LIBRARIES(mudisp ${libmudisp_libs})
SET_TARGET_PROPERTIES(mudisp PROPERTIES DEFINE_SYMBOL "NEWMUDISP_DLL_EXPORTS")

INSTALL(TARGETS mudisp
    LIBRARY DESTINATION ${LIBRARY_DIR} # .so file
    ARCHIVE DESTINATION ${LIBRARY_DIR} # .lib file
    RUNTIME DESTINATION ${LIBRARY_DIR} # .dll file
)


	





