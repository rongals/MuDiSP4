// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// Header file to define C/Fortran conventions (Platform specific)

#ifndef FORTRAN_H
#define FORTRAN_H

// help map between C/C++ data types and Fortran types


namespace TNT
{
	/*! @typedef int     Fortran_integer
	 * \@brief map between C/C++ data types and Fortran types
	 * */
    typedef int     Fortran_integer;
    /*! @typedef int     Fortran_float
	 * \@brief map between C/C++ data types and Fortran types
	 * */
    typedef float   Fortran_float;
    /*! @typedef int     Fortran_double
	 * \@brief map between C/C++ data types and Fortran types
	 * */
    typedef double  Fortran_double;

/*! @struct Fortran_complex16 fortran.h <tnt/fortran.h>
 * @brief  map between C/C++ data complex and Fortran complex
 * */

    struct Fortran_complex16 {
		/*! @var double RE
		 * @brief real part */
		  double RE; 
		/*! @var double IM
		 * @brief image part */
		 double IM;
    };

}

#ifndef TNT_SUBSCRIPT_TYPE
#define TNT_SUBSCRIPT_TYPE TNT::Fortran_integer
#endif


#endif
// FORTRAN_H
