

MESSAGE(\n "Ingresso nella cartella ../library/ "\n)

INSTALL(FILES
	${CMAKE_CURRENT_SOURCE_DIR}/base/filter.h 
	${CMAKE_CURRENT_SOURCE_DIR}/base/write.h
	#${CMAKE_CURRENT_SOURCE_DIR}/base/uhd_mudisp_api.h
	#${CMAKE_CURRENT_SOURCE_DIR}/base/gr_uhd_usrp_sink.h
	${CMAKE_CURRENT_SOURCE_DIR}/usrp/uhd_mudisp_usrp_sink.h 
	${CMAKE_CURRENT_SOURCE_DIR}/usrp/uhd_mudisp_usrp_source.h 
	DESTINATION  ${INCLUDE_DIR}/library/base
)

