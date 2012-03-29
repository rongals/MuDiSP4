//
// MuDiSP3 (Multirate Digital Signal Processor 3)
// Copyright 1999,2005 Luca Simone Ronga
//
// Contributor: Simone Menci
// 
//  This file is part of MuDiSP3.
//
//  MuDiSP3 is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  MuDiSP3 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with MuDiSP3; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
/////////////////////////////////////////////////////////
//
//
//
//

#ifndef __FILTRO_H
#define __FILTRO_H

#include <iostream>
#include <fstream> 
#include <stdlib.h> 
#include <complex>

using namespace std;

//
// FILTER_DOUBLE
//	filter_blk
//

/*! @class Filter filter.h <library/base/filter.h>
 *  @brief  Implementation of FIR o IIR filter
 *  @author Luca Simone Ronga
 *  @version   1
 *  @date      1999-2011
 *  @ingroup filter_blk
 */ 

class Filter {
  public:
  /** @fn Filter()
   * @brief Class Constructor
   * */
    Filter() {};
    /** @fn virtual double filtra(const double &s)=0
     * @brief Service routine for Filter 
     * @param s Reference to coefficient pointer 
     * @return filter element
     * */
    virtual double filtra(const double &s)=0;
    virtual ~Filter(){};

  protected:
  /** @var double *coeff_b
   * @brief Define pointer to a vector of coefficient for filter
   * */
    /** @var double *coeff_a
   * @brief Define pointer to a Vector of coefficient for filter
   * */
    double *coeff_b, *coeff_a;
    
    /** @var double *mem_a
     * @brief Define a pointer to memory area for store coefficient or partial result of filtering operation
     * */ 
     
     /** @var double *mem_b
     * @brief Define a pointer to memory area for store coefficient or partial result of filtering operation
     * */ 
    double *mem_b, *mem_a;
};

/*! @class IIR filter.h <library/base/filter.h>
 *  @brief  Implementazione di un filtro  IIR aritmetica reale
 *  @author Luca Simone Ronga
 *  @version   1
 *  @date      1999-2011
 *  @ingroup filter_blk
 */ 
class IIR : public Filter {
  private:
  /** @var int lb
   * @brief Internal variable for filter lenth */
   /** @var int la
   * @brief Internal variable for filter lenth */
	int lb, la;

  public:
  /** @fn IIR()
   * @brief Constructor */
	IIR();
	/** @fn IIR(double *b, double *a, const int lb, const int la )
   * @brief Block Constructor 
   * @param *b Pointer to filter coefficient array b
   * @param *a Pointer to filter coefficient array a
   * @param lb Length of Filter coefficient vector 
   * @param la Length of Filter coefficient vector */
	IIR(double *b, double *a, const int lb, const int la );
 /** @fn IIR(const char *bfname, const char *afname)
   * @brief Block class constructor
   * @param bfname File path for filter coefficient array b 
   * @param afname File path  for filter coefficient array a
   * */
	IIR(const char *bfname, const char *afname);
	/** @fn double filtra(const double &val)
	 * @brief Filter function
	 * @param val Reference to value pointer
	 * @return Filter coefficient
	 * */
	double filtra(const double &val);
	~IIR();
};

/*! @class FIR filter.h <library/base/filter.h>
 *  @brief  Implementazione di un filtro FIR aritmetica reale
 *  @author Luca Simone Ronga
 *  @version   1
 *  @date      1999-2011
 *  @ingroup filter_blk
 */ 
class FIR : public Filter {
  private:
  /**@var int lb
   * *@brief Internal parameter for filter Length*/
	int lb;

  public:
  	/** @fn FIR()
   * @brief Block Constructor 
   *  */
	FIR();
		/** @fn FIR(double *b, const int b_length )
   * @brief Block Constructor 
   * @param *b Pointer to filter coefficient array b
   * @param b_length Length of Filter coefficient vector */
	FIR(double *b, const int b_length);
	/** @fn FIR(const char *bfname)
 * @brief Block class constructor
   * @param bfname File path for filter coefficient array b 
   * */
	FIR(const char *bfname);
   /** @fn double filtra(const double &val)
	 * @brief Filter function
	 * @param val Reference to value pointer
	 * @return Filter coefficient
	 * */
	double filtra(const double &val);
	~FIR();
};


//
// FILTER_COMPLEX
//
/*! @class FilterCx filter.h "library/base/filter.h"
 *  @brief  Implementazione di un filtro FIR e IIR in aritmetica complessa
 *  @author Luca Simone Ronga
 *  @version   1
 *  @date      1999-2011
 *  @ingroup filter_blk
 */ 
class FilterCx {
  public:
  /** @fn FilterCx()
   * @brief Constructor for class 
   * */
    FilterCx() {};
    /** @fn virtual complex < double > filtracx(const complex < double > &s)
     * @brief Filter calculation routine
     * @param s Complex coefficient passed to filter
     * @return filtered coefficient
     * */
    virtual complex < double > filtracx(const complex < double > &s)=0;
    virtual ~FilterCx(){};

  protected:
    /** @var double *coeff_b
   * @brief Define pointer to a vector of coefficient for filter
   * */
    /** @var double *coeff_a
   * @brief Define pointer to a Vector of coefficient for filter
   * */
	double *coeff_b, *coeff_a;
	
	    /** @var complex <double> *mem_bx
     * @brief Define a pointer to memory area for store coefficient or partial result of filtering operation
     * */ 
     
     /** @var complex <double> *mem_ax
     * @brief Define a pointer to memory area for store coefficient or partial result of filtering operation
     * */ 
	complex < double > *mem_bx, *mem_ax;
};

/*! @class IIRCx filter.h <library/base/filter.h>
 *  @brief  Implementazione di un filtro IIR in aritmetica complessa
 *  @author Luca Simone Ronga
 *  @version   1
 *  @date      1999-2011
 *  @ingroup filter_blk
 */ 
class IIRCx : public FilterCx {
  private:
    /** @var int lb
   * @brief Internal variable for filter lenth */
   /** @var int la
   * @brief Internal variable for filter lenth */
	int lb, la;

  public:
  /** @fn IIRCx
   * @brief Class constructor 
   * */
	IIRCx(){};
		/** @fn IIRCx(double *b, double *a, const int lb, const int la )
   * @brief Block Constructor 
   * @param *b Pointer to filter coefficient array b
   * @param *a Pointer to filter coefficient array a
   * @param lb Length of Filter coefficient vector 
   * @param la Length of Filter coefficient vector */
	IIRCx(double *b, double *a, const int lb, const int la );
 /** @fn IIRCx(const char *bfname, const char *afname)
   * @brief Block class constructor
   * @param bfname File path for filter coefficient array b 
   * @param afname File path  for filter coefficient array a
   * */
	IIRCx(const char *bfname, const char *afname);
	
   /** @fn complex < double > filtracx(const complex < double > &val)
	 * @brief Filter function
	 * @param val Reference to value pointer
	 * @return Filter coefficient
	 * */
	 
	complex < double > filtracx(const complex < double > &val);
	~IIRCx();
};

/*! @class FIRCx filter.h <library/base/filter.h>
 *  @brief  Implementazione di un filtro FIR  in aritmetica complessa
 *  @author Luca Simone Ronga
 *  @version   1
 *  @date      1999-2011
 *  @ingroup filter_blk
 */ 
class FIRCx : public FilterCx {
  private:
    /** @var int lb
   * @brief Internal variable for filter lenth */
	int lb;

  public:
  /** @fn FIRCx()
   * @brief Class constructor 
   * */
	FIRCx(){};
 /** @fn FIRCx(double *b, const int b_length,)
   * @brief Block Constructor 
   * @param *b Pointer to filter coefficient array b
   * @param b_length Length of Filter coefficient vector 
   *  */
	FIRCx(double *b, const int b_length);
 /** @fn FIRCx(const char *bfname)
   * @brief Block class constructor
   * @param bfname File path for filter coefficient array b 
   * */
	FIRCx(const char *bfname);
   /** @fn complex < double > filtracx(const complex < double > &val)
	 * @brief Filter function
	 * @param val Reference to value pointer
	 * @return Filter coefficient
	 * */
    complex < double > filtracx(const complex < double > &val);
	~FIRCx();
};


#endif //  __FILTRO_H
