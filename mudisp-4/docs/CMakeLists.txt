########################################################################
# Setup Manual
########################################################################
MESSAGE(STATUS "")
FIND_PACKAGE(Docutils)

LIBMUDISP_REGISTER_COMPONENT("Manual" ENABLE_MANUAL ON "DOCUTILS_FOUND" OFF)


########################################################################
# Setup Doxygen
########################################################################
MESSAGE(STATUS "")
FIND_PACKAGE(Doxygen)

LIBMUDISP_REGISTER_COMPONENT("Doxygen" ENABLE_DOXYGEN ON "DOXYGEN_FOUND" OFF)

IF(ENABLE_DOXYGEN)
    #generate the doxygen configuration file
    SET(CMAKE_CURRENT_BINARY_DIR_DOXYGEN ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/DoxyRule
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY)

   # INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Mainpage.dox
	#			  ${CMAKE_CURRENT_SOURCE_DIR}/logo.gif
	#	${PROJECT_SOURCE_DIR}/manifest.xml
	#        ${PROJECT_SOURCE_DIR}/Resources/ROS_NOBUILD
    # DESTINATION ${CMAKE_CURRENT_BINARY_DIR_DOXYGEN}  
    # )

    #make doxygen directory depend on the header files
    FILE(GLOB_RECURSE header_files ${CMAKE_SOURCE_DIR}/include/*.hpp)
    FILE(GLOB_RECURSE header_files ${CMAKE_SOURCE_DIR}/include/*.h)
   # FILE(GLOB_RECURSE header_files ${CMAKE_SOURCE_DIR}/docs/*.dox)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR_DOXYGEN} DEPENDS ${header_files}
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        COMMENT "Generating documentation with doxygen"
    )

    #make the doxygen generation a built-time dependency
    ADD_CUSTOM_TARGET(doxygen_docs ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR_DOXYGEN})
    INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR_DOXYGEN} DESTINATION ${PKG_DOC_DIR} COMPONENT doxygen)
ENDIF(ENABLE_DOXYGEN)
