

MESSAGE(\n "Ingresso nella cartella ../library/ "\n)

INSTALL(FILES
	${CMAKE_CURRENT_SOURCE_DIR}/base/filter.h 
	${CMAKE_CURRENT_SOURCE_DIR}/base/write.h
	DESTINATION  ${INCLUDE_DIR}/library/base
)
#
#
IF(ENABLE_ENABLE_USRP_II OR ENABLE_USRP_I)
INSTALL(FILES
	${CMAKE_CURRENT_SOURCE_DIR}/usrp/uhd_mudisp_usrp_sink.h 
	${CMAKE_CURRENT_SOURCE_DIR}/usrp/uhd_mudisp_usrp_source.h
    DESTINATION  ${INCLUDE_DIR}/library/usrp
)
ENDIF(ENABLE_USRP)

