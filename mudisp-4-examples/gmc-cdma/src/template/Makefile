#
# INeSiS Makefile
#
##########################################
#  C++  COMPILER
#
#  (redefine this only if you encounter 
#  some problems)
#
#  CC = g++

##########################################
# MuDiSP SIMULATOR PATH
MDSP3_SRC=/usr/local/mudisp3

##########################################
# USER PROJECT NAME (es. iir) 
USERPROJECT = iir

##########################################
# USER SOURCES DIR (es. SRC SRC1 SRC2) 
USERDIRS = SRC SRC1 SRC2


##########################################
# USER SOURCES FILES
USERSRC = templ.cpp


##########################################
# DEBUG
#DEBUG = true;

##########################################
# SIMULATOR LIBS
LIBS = 	-lmudisp \
	-linesis \
	-lblocks


##########################################
##########################################
# DO NOT MODIFY BELOW
#

USERSRC += system.cpp	\
	main.cpp

include $(MDSP3_SRC)/lib/Makefile.sim
