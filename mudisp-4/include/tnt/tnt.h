// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


#ifndef TNT_H
/*! @def TNT_H
 * @brief TNT general header file.  Defines default types  and conventions.
 * @ingroup matrix
 * */
#define TNT_H

//---------------------------------------------------------------------
//  tnt.h       TNT general header file.  Defines default types
//              and conventions.
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//  Include current version 
//---------------------------------------------------------------------
#include "version.h"

//---------------------------------------------------------------------
// Define the data type used for matrix and vector Subscripts.
// This will default to "int", but it can be overriden at compile time,
// e.g.
// 
//      g++ -DTNT_SUBSCRIPT_TYPE='unsinged long' ...
//
// See subscrpt.h for details.
//---------------------------------------------------------------------

#include "subscrpt.h"



//---------------------------------------------------------------------
// Define this macro if you want  TNT to ensure all refernces
// are within the bounds of the array.  This encurs a run-time
// overhead, of course, but is recommended while developing
// code.  It can be turned off for production runs.
// 
//       #define TNT_BOUNDS_CHECK
//---------------------------------------------------------------------
//
/*! @def TNT_BOUNDS_CHECK
 * @note  Define this macro if you want  TNT to ensure all refernces
 are within the bounds of the array.  This encurs a run-time
 overhead, of course, but is recommended while developing
 code.  It can be turned off for production runs.
 * @ingroup matrix
 * */
#define TNT_BOUNDS_CHECK
#ifdef TNT_NO_BOUNDS_CHECK
#undef TNT_BOUNDS_CHECK
#endif

//---------------------------------------------------------------------
// Define this macro if you want to utilize matrix and vector
// regions.  This is typically on, but you can save some
// compilation time by turning it off.  If you do this and
// attempt to use regions you will get an error message.
//
//       #define TNT_USE_REGIONS
//---------------------------------------------------------------------
//
/*! 
 * \def TNT_USE_REGIONS
 * \note Define this macro if you want to utilize matrix and vector
 regions.  This is typically on, but you can save some
 compilation time by turning it off.  If you do this and
 attempt to use regions you will get an error message.
 * @ingroup matrix
*/
#define TNT_USE_REGIONS

//---------------------------------------------------------------------
//  
//---------------------------------------------------------------------
// if your system doesn't have abs() min(), and max() uncoment the following
//---------------------------------------------------------------------
//
// 
//#define __NEED_ABS_MIN_MAX_

#include "tntmath.h"


#endif
// TNT_H
