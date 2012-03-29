########################################################################
INCLUDE(MUDISPVersion) #sets version information

########################################################################
# Setup CPack
########################################################################
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MuDiSP")
SET(CPACK_PACKAGE_VENDOR              "University of Florence")
SET(CPACK_PACKAGE_CONTACT             "innocentimassimo@hotmail.com")
SET(CPACK_PACKAGE_VERSION_MAJOR ${MUDISP_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${MUDISP_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${MUDISP_VERSION_PATCH})
#SET(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README)
#SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
SET(BOOST_MIN_VERSION 1.46) #used in setup for boost
STRING(REPLACE "," ", " CPACK_DEBIAN_PACKAGE_DEPENDS
    "libboost-date-time-dev          (>= ${BOOST_MIN_VERSION}),"
    "libboost-filesystem-dev         (>= ${BOOST_MIN_VERSION}),"
    "libboost-program-options-dev    (>= ${BOOST_MIN_VERSION}),"
    "libboost-regex-dev              (>= ${BOOST_MIN_VERSION}),"
    "libboost-system-dev             (>= ${BOOST_MIN_VERSION}),"
    "libboost-test-dev               (>= ${BOOST_MIN_VERSION}),"
    "libboost-thread-dev             (>= ${BOOST_MIN_VERSION})"
)
SET(CPACK_DEBIAN_PACKAGE_RECOMMENDS "python, python-tk")
SET(CPACK_RPM_PACKAGE_REQUIRES "boost-devel >= ${BOOST_MIN_VERSION}")
INCLUDE(CPack) #include after setting vars
