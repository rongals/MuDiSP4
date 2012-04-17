
MESSAGE(\n "Ingresso nella cartella ../mudisp/ "\n)
########################################################################
# Helpful Macros
########################################################################
MACRO(LIBMUDISP_APPEND_SOURCES)
    LIST(APPEND libmudisp_sources ${ARGV})
ENDMACRO(LIBMUDISP_APPEND_SOURCES)

MACRO(LIBMUDISP_APPEND_LIBS)
    LIST(APPEND libmudisp_libs ${ARGV})
ENDMACRO(LIBMUDISP_APPEND_LIBS)

########################################################################
# Include CMakeLists.txt from subdirectories
########################################################################
MACRO(INCLUDE_SUBDIRECTORY subdir)
    #insert the current directories on the front of the list
    LIST(INSERT _cmake_source_dirs 0 ${CMAKE_CURRENT_SOURCE_DIR})
    LIST(INSERT _cmake_binary_dirs 0 ${CMAKE_CURRENT_BINARY_DIR})

    #set the current directories to the names of the subdirs
    SET(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
    SET(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${subdir})

    #include the subdirectory CMakeLists to run it
    FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

    #reset the value of the current directories
    LIST(GET _cmake_source_dirs 0 CMAKE_CURRENT_SOURCE_DIR)
    LIST(GET _cmake_binary_dirs 0 CMAKE_CURRENT_BINARY_DIR)

    #pop the subdir names of the front of the list
    LIST(REMOVE_AT _cmake_source_dirs 0)
    LIST(REMOVE_AT _cmake_binary_dirs 0)
ENDMACRO(INCLUDE_SUBDIRECTORY)

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
SET(MUDISP_LIB ${CMAKE_SOURE_DIR}/mudisp
	       ${CMAKE_SOURE_DIR}/mudisp/engine
	       ${INCLUDE_DIR}
)

INCLUDE_DIRECTORIES(${MUDISP_LIB})

MESSAGE(STATUS "Directory To Include ${INCLUDE_DIRECTORIES}")
#INSTALL(FILES
#	${CMAKE_CURRENT_SOURCE_DIR}/engine/errors.h  
	#${CMAKE_CURRENT_SOURCE_DIR}/engine/main.h    
	#${CMAKE_CURRENT_SOURCE_DIR}/engine/param.h
	#${CMAKE_CURRENT_SOURCE_DIR}/engine/block.h              
	#${CMAKE_CURRENT_SOURCE_DIR}/engine/lcm.h     
	#${CMAKE_CURRENT_SOURCE_DIR}/engine/mudisp.h  
	#${CMAKE_CURRENT_SOURCE_DIR}/engine/parlist.h  
	#${CMAKE_CURRENT_SOURCE_DIR}/engine/port.h
	#DESTINATION  ${INCLUDE_DIR}
#)

MESSAGE(STATUS " Directory Corrente Binary ${CMAKE_SOURCE_DIR}")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib/usrp/usrp_e100)
INCLUDE(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(mudisp.h __MUDISP2_MUDISP_HXX)

LIBMUDISP_APPEND_SOURCES(
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/block.cpp  
	${CMAKE_CURRENT_SOURCE_DIR}/engine/lcm.cpp  
	${CMAKE_CURRENT_SOURCE_DIR}/engine/param.cpp  
	${CMAKE_CURRENT_SOURCE_DIR}/engine/parlist.cpp  
	${CMAKE_CURRENT_SOURCE_DIR}/engine/port.cpp  
)

MESSAGE(STATUS " Directory Corrente ${libmudisp_sources}")

########################################################################
# Setup libmudisp library
########################################################################
	ADD_LIBRARY(mudisp STATIC ${libmudisp_sources})
	TARGET_LINK_LIBRARIES(mudisp ${Boost_LIBRARIES} ${libinesis_libs} ${libmudisp_libs})
	SET_TARGET_PROPERTIES(mudisp PROPERTIES DEFINE_SYMBOL "NEWMUDISP_DLL_EXPORTS")
  
    INSTALL(TARGETS mudisp
		LIBRARY DESTINATION ${LIBRARY_DIR} # .so file
		ARCHIVE DESTINATION ${LIBRARY_DIR} # .lib file
		RUNTIME DESTINATION ${LIBRARY_DIR} # .dll file
	)





	





