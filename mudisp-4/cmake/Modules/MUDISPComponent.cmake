
########################################################################
SET(_MuDiSP_enabled_components "" CACHE INTERNAL "" FORCE)
SET(_MuDiSP_disabled_components "" CACHE INTERNAL "" FORCE)

########################################################################
# Register a component into the system
#  - name the component string name
#  - var the global enable variable
#  - enb the default enable setting
#  - deps a list of dependencies
#  - dis the default disable setting
########################################################################
MACRO(LIBMUDISP_REGISTER_COMPONENT name var enb deps dis)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "Configuring ${name} support...")
    FOREACH(dep ${deps})
        MESSAGE(STATUS "  Dependency ${dep} = ${${dep}}")
    ENDFOREACH(dep)

    #setup the dependent option for this component
    INCLUDE(CMakeDependentOption)
    CMAKE_DEPENDENT_OPTION(${var} "enable ${name} support" ${enb} "${deps}" ${dis})

    #append the component into one of the lists
    IF(${var})
        MESSAGE(STATUS "  Enabling ${name} support.")
        LIST(APPEND _MuDiSP_enabled_components ${name})
    ELSE(${var})
        MESSAGE(STATUS "  Disabling ${name} support.")
        LIST(APPEND _MuDiSP_disabled_components ${name})
    ENDIF(${var})
    MESSAGE(STATUS "  Override with -D${var}=ON/OFF")

    #make components lists into global variables
    SET(_MuDiSP_enabled_components ${_MuDiSP_enabled_components} CACHE INTERNAL "" FORCE)
    SET(_MuDiSP_disabled_components ${_MuDiSP_disabled_components} CACHE INTERNAL "" FORCE)
ENDMACRO(LIBMUDISP_REGISTER_COMPONENT)

########################################################################
# Print the registered component summary
########################################################################
FUNCTION(MUDISP_PRINT_COMPONENT_SUMMARY)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "######################################################")
    MESSAGE(STATUS "# MUDISP enabled components                              ")
    MESSAGE(STATUS "######################################################")
    FOREACH(comp ${_MuDiSP_enabled_components})
        MESSAGE(STATUS "  * ${comp}")
    ENDFOREACH(comp)

    MESSAGE(STATUS "")
    MESSAGE(STATUS "######################################################")
    MESSAGE(STATUS "# MUDISP disabled components                             ")
    MESSAGE(STATUS "######################################################")
    FOREACH(comp ${_MuDiSP_disabled_components})
        MESSAGE(STATUS "  * ${comp}")
    ENDFOREACH(comp)

    MESSAGE(STATUS "")
ENDFUNCTION(MUDISP_PRINT_COMPONENT_SUMMARY)
