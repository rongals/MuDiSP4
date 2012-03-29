#
# Copyright 2010-2011 Ettus Research LLC
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#


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
