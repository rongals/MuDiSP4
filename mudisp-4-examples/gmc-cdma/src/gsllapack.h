/*
** gsllapack.h
**
**
**
**
*/

#ifndef MUDISP_GSLLAPACK_H
#define MUDISP_GSLLAPACK_H

#include "gsl/gsl_matrix.h"

int gsl_lapack_solveSVD_complex(gsl_vector_complex *vecb, 
				gsl_matrix_complex *mata,
				gsl_vector_complex *vecx);

#endif
