# CMakeLists.txt in tnt dir

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB itemviews-ng_SOURCES *.cpp )

#MESSAGE(STATUS "Sorgenti: ${itemviews-ng_SOURCES}")

MESSAGE(STATUS "Sorgenti: ${itemviews-ng_SOURCES}")


#for each source: build an executable and install
FOREACH(itemviews-ng_SOURCES ${itemviews-ng_SOURCES})
    GET_FILENAME_COMPONENT(example_name ${itemviews-ng_SOURCES} NAME_WE)
    MESSAGE( STATUS "Sorgenti: ${example_name}") 
    ADD_EXECUTABLE(${example_name} ${itemviews-ng_SOURCES})
    SET_TARGET_PROPERTIES(${example_name} PROPERTIES COMPILE_FLAGS "-w")
    #TARGET_LINK_LIBRARIES(${itemviews-ng_SOURCES} TNT)
    INSTALL(TARGETS ${example_name} RUNTIME DESTINATION ${TOOLS_RUNTIME_DIR} )
ENDFOREACH(itemviews-ng_SOURCES)

INSTALL(FILES
	launcher
    DESTINATION  ${TOOLS_RUNTIME_DIR}
)





