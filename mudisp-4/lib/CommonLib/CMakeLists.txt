########################################################################
# Helpful Macros
########################################################################
MACRO(LIBINESIS_APPEND_SOURCES)
    LIST(APPEND libinesis_sources ${ARGV})
ENDMACRO(LIBINESIS_APPEND_SOURCES)

MACRO(LIBINESIS_APPEND_LIBS)
    LIST(APPEND libinesis_libs ${ARGV})
ENDMACRO(LIBINESIS_APPEND_LIBS)

MACRO(LIBINESIS_PYTHON_GEN_SOURCE pyfile outfile)
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
    LIBINESIS_APPEND_SOURCES(${outfile})
ENDMACRO(LIBINESIS_PYTHON_GEN_SOURCE)

########################################################################
# Include CMakeLists.txt from subdirectories
########################################################################
#INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/randlib/CMakeLists.txt)
#ADD_SUBDIRECTORY(randlib)

########################################################################
# Append to the list of sources for lib INesis
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
SET(COMMON_LIB ${CMAKE_SOURE_DIR}/common
	#       ${CMAKE_SOURE_DIR}/common/(randlib)
)

INCLUDE_DIRECTORIES(${COMMON_LIB})

MESSAGE(STATUS " Directory Corrente ${CMAKE_CURRENT_BINARY_DIR}")

LIBINESIS_APPEND_SOURCES(
    ${CMAKE_CURRENT_SOURCE_DIR}/BitVector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Configuration.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/F16.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/Interthread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LinkedLists.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/Regexp.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Sockets.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Threads.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Timeval.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/Vector.cpp
)

MESSAGE(STATUS " Directory Corrente ${libinesis_sources}")

########################################################################
# Setup libinesis library
########################################################################
IF("${CMAKE_BUILD_TYPE}" MATCHES Debug) 
	ADD_LIBRARY(inesis_dbg STATIC ${libinesis_sources})
	ADD_DEFINITIONS(-pthread) 
	TARGET_LINK_LIBRARIES(inesis_dbg ${libinesis_libs} pthread)
	SET_TARGET_PROPERTIES(inesis_dbg PROPERTIES DEFINE_SYMBOL "NEWINESIS_DLL_EXPORTS")

INSTALL(TARGETS inesis_dbg
    LIBRARY DESTINATION ${LIBRARY_DIR} # .so file
    ARCHIVE DESTINATION ${LIBRARY_DIR} # .lib file
    RUNTIME DESTINATION ${LIBRARY_DIR} # .dll file
)
ENDIF("${CMAKE_BUILD_TYPE}" MATCHES Debug)

IF("${CMAKE_BUILD_TYPE}" MATCHES Release) 

	ADD_LIBRARY(inesis STATIC ${libinesis_sources})
	ADD_DEFINITIONS(-pthread) 
	TARGET_LINK_LIBRARIES(inesis ${libinesis_libs} pthread)
	SET_TARGET_PROPERTIES(inesis PROPERTIES DEFINE_SYMBOL "NEWINESIS_DLL_EXPORTS")

INSTALL(TARGETS inesis
    LIBRARY DESTINATION ${LIBRARY_DIR} # .so file
    ARCHIVE DESTINATION ${LIBRARY_DIR} # .lib file
    RUNTIME DESTINATION ${LIBRARY_DIR} # .dll file
)
ENDIF("${CMAKE_BUILD_TYPE}" MATCHES Release)





	





