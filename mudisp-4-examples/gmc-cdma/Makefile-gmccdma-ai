#
# INeSiS Makefile
#

##########################################
#  SOAR support
SOARHOME=/usr/local/sandbox_ronga/SOAR
SOARLIBS=-L $(SOARHOME)/bin -lSoar
#

##########################################
#  C++  COMPILER
#CC = g++ -DSHOW_MATRIX
CC = g++ -I $(SOARHOME)/bin/include


##########################################
# MuDiSP4 SIMULATOR PATH
MDSP4_SRC=/usr/local/mudisp3
MDSP3_SRC=$(MDSP4_SRC)


##########################################
# USER PROJECT NAME (es. iir) 
USERPROJECT = gmc-cdma-ai

##########################################
# USER SOURCES DIR (es. SRC SRC1 SRC2) 
USERDIRS = src src_gmccdma_ai




##########################################
# USER SOURCES FILES

USERSRC = mblockuser.cpp \
	mbitsrc.cpp \
	maiallocator.cpp \
	mcpmpcoeffs.cpp \
	mcpmpchan.cpp \
	mblockrec.cpp \
	gsllapack.cpp \
	mbitber.cpp \
	propagation.cpp


##########################################
# DEBUG
#DEBUG = true;

##########################################
# SIMULATOR LIBS
#
# add the following for LAPACK Support
# (requires lapack package installed)
# 	-lg2c \
# 	-llapack
#
# add the following for GSL Support
# (requires gsl package installed)
#	-lgsl \
#	-lgslcblas
#
#
# add the following for SOAR support
# SOARHOME=/usr/local/sandbox_ronga/SoarSuite-9.3.1-linux-x86_64
# SOARINCLUDE=$(SOARHOME)/include
# SOARLIBS="-L $(SOARHOME)/lib -lClientSML -lConnectionSML -lElementXML"
#
#
LIBS = -L/usr/local/libboost/lib\
	-lgsl \
	-lgslcblas \
	-llapack \
	$(SOARLIBS)


##########################################
##########################################
# DO NOT MODIFY BELOW
#

USERSRC += system.cpp	\
	main.cpp

include $(MDSP4_SRC)/lib/Makefile.sim

##########################################
##########################################
# DEMO Targets 
#
demosim:
	rm -f ./results/*.dat 
	./launcher ./cdma_verdu_awgn params/cdma_blind_awgn_63c2u_00*
	./launcher ./cdma_verdu_awgn params/cdma_mf_awgn_63c2u_00*

demoplot:
	analyzer/2ublind.gnu

democlean: distclean
	rm -f cdma_verdu_awgn

##########################################
##########################################
# gmc-cdma Targets 
#
buildparams:
	./tools/param_exploder params/gmccdma_fca.txt params/ebno.txt
	./tools/param_exploder params/gmccdma_dca.txt params/ebno.txt
	./tools/param_exploder params/gmccdma_swp.txt params/ebno.txt
	./tools/param_exploder params/gmccdma_ovl.txt params/ebno.txt

launchall:
	./launcher ./gmc-cdma-fca 	params/gmccdma_fca_*.txt
	./launcher ./gmc-cdma-bestcarr 	params/gmccdma_dca_*.txt
	./launcher ./gmc-cdma-swap 	params/gmccdma_swp_*.txt
	./launcher ./gmc-cdma-overlap 	params/gmccdma_ovl_*.txt
