# CMakeLists.txt in tnt dir

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

########################################################################
# Local Include Dir
########################################################################
#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/tnt)


INSTALL(FILES
	blas.h          
	cmat.h     
	fmat.h     
	index.h   
	qr.h        
	stpwatch.h  
	tnt.h      
	transv.h  
	vecadaptor.h
	cholesky.h      
	fortran.h  
	lapack.h  
	region1d.h  
	subscrpt.h  
	tntmath.h  
	triang.h  
	vec.h
	fcscmat.h  
	fspvec.h   
	lu.h      
	region2d.h  
	tntcmplx.h  
	tntreqs.h  
	trislv.h  
	version.h
    DESTINATION  ${INCLUDE_DIR}/TNT
)

