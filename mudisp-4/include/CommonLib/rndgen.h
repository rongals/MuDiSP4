/*!
  \file rndgen.h
  @brief Random Generation 
*/

/*
 $Id: rndgen.h,v 1.1.1.1 2002/09/11 07:18:36 ronga Exp $ 
*/

//
// MuDiSP3 (Multirate Digital Signal Processor 3)
// Copyright 1999, Luca Simone Ronga
//
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


#include <stdlib.h>
#include <math.h>

#include <CommonLib/randlib/randlib.h>

#ifndef ___RNDGEN_H___
#define ___RNDGEN_H___

/** @fn void SetSeed(int seed1, int seed2)
 * @brief Set seen value for rundom generation
 * @param seed1 First seed value
 * @param seed2 Second seed value
 * */
void SetSeed(int seed1, int seed2);

////////////////////////////////////////////////////////////////////////////////
// generic functions w/o initalizers 
/** @fn double rndUniformDouble(double low=0.0, double high=1.0);
 * @brief Generic functions w/o initalizers
 * @param low Lower value of distribution
 * @param high Higher value of distribution 
 * @return New random value from Uniform distribution
 * */ 
double rndUniformDouble(double low=0.0, double high=1.0);

/** @fn long int rndUniformInt(long int low=0, long int high=1)
 * @brief Generic functions w/o initalizers
 * @param low Lower value of distribution
 * @param high Higher value of distribution 
 * @return New random value from Uniform distribution
 * */ 
long int rndUniformInt(long int low=0, long int high=1);

/** @fn double rndExponential(double M)
 * @brief Random Exponential Distribution Random Generator
 * @param M Mean value of distribution
 * */
double rndExponential(double M);

/** @fn double rndGaussian(double M, double sigma);
 * @brief Random Gaussian Distribution Random Generator
 * @param M Mean value of Distribution
 * @param sigma Standard Deviation of Distribution
 * */
double rndGaussian(double M, double sigma);

/** @fn unsigned int rndGeometric0(double M)
 * @brief Random Geometric zero mean Distribution Random Generator
 * @param M Mean value of Distribution
 * */
unsigned int rndGeometric0(double M);

/** @fn unsigned int rndGeometric1(double M)
 * @brief Random Geometric one mean Distribution Random Generator
 * @param M Mean value of Distribution
 * */
unsigned int rndGeometric1(double M);


/***********************************************************************
*                                                                      *
*   Negative Binomial distribution                                     *       *                                                                      *
*                                                                      *
*     p(k) = (k + r -1 ) * p^r * (1-p)^k                               *
*            (    k    )                                               *
*                                                                      *
*     mean     = r*(1-p)/p                                             *
*     variance = r*(1-p)/p^2                                           *
*                                                                      *
***********************************************************************/
/** @fn long int NegativeBinomial(long r, float p)
 * @brief Negative Binomial distribution
 * \f[ p(k) = \binom{k + r -1 }{k} * p^r * (1-p)^k  \f]
 *  mean     \f$ = r*(1-p)/p \f$ \n
 *  variance = \f$ r*(1-p)/p^2 \f$
 * @param r Order
 * @param p Probability 
 * @return New random value generate from binomial distribution
 * */
long int NegativeBinomial(long r, float p);


/***********************************************************************
*                                                                      *
*   Pareto distribution                                                *
*                                                                      *
*     p(x) = (shape * location^shape)/(x^(shape+1))                    *
*     P(x) = 1 - (location/x)^shape                                    *
*     mean     = location * shape / (shape - 1)                        *
*     variance = location^2 * shape / ( (shape -2) * (shape - 1)^2 )   *
*                                                                      *
***********************************************************************/
/** @fn double rndPareto(double location, double shape);
 * @brief Pareto distribution
 * \f[   p(x) = \dfrac{shape * location^{shape}}{x^{shape+1}}        \f]
*  \f[   P(x) = 1 - \left(\frac{location}{x}\right)^{shape} \f]                          
*     mean     \f$ = location * \dfrac{shape}{shape - 1} \f$               \n        
*     variance = \f$ \dfrac{location^2 * shape }{ (shape -2) * (shape - 1)^2 } \f$   
* @param location distribution location
* @param shape Distribution shape
* @return New random value generate from Pareto distribution
*  */
double rndPareto(double location, double shape);
	

/**************************************************************************
*                                                                         *
*   Weibull distribution                                                  *
*                                                                         *
*        l = location, sh = shape, sc = scale, Gamma() = Gamma function   *
*     p(x) = sh/sc * ((x-l)/sc)^(sh-1) * e^(-((x-l)/sc)^sh)               *
*     P(x) = 1 - e^(-((x-l)/sc)^sh)                                       *
*     mean     = l + sc*Gamma((sh+1)/sh)                                  *
*     variance = sc^2 * ( Gamma((sh+2)/sh) - Gamma^2((sh+1)/sh) )         *
*                                                                         *
**************************************************************************/
/** @fn double rndWeibull(double location, double scale, double shape)
 * @brief Weibull distribution. \n                                                
 *     l = location, \n sh = shape, \n  sc = scale, \n  Gamma() = Gamma function    \n
*     \f$ p(x) = 	\frac{sh}{sc} * \left(\frac{x-l}{sc}\right)^{sh-1} * e^{-(\frac{x-l}{sc})^{sh}}              \f$ \n
*    \f$ P(x) = 1 - e^{-(\frac{x-l}{sc})^{sh}}                                       \f$ \n
*     mean     \f$ = l + sc*Gamma(\frac{sh+1}{sh})                                  \f$ \n
*    variance = \f$ sc^2 * \left( Gamma\left(\frac{sh+2}{sh} \right) - Gamma^2\left(\frac{sh+1}{sh}\right) \right)         \f$ \n
* @param location distribution location
* @param scale distribution scale
* @param shape distribuion scale
* @return New random value generate from Weibull distribution
* */ 

double rndWeibull(double location, double scale, double shape);


////////////////////////////////////////////////////////////////////////////////
//
/** @class UniformDoubleRndGen rndgen.h <common/rndgen.h>
 * @brief  Define Uniform Distribution for double precision random generation
 * @ingroup rand */
class UniformDoubleRndGen
{
protected:
/** @var double low
 * @brief Lower value of distribution*/

/** @var double high
 * @brief Higher value of distribution*/

  double low, high;

public:
/** @fn   UniformDoubleRndGen(double _low=0, double _high=1) : low(_low), high(_high) 
 * @brief Class constructor
 * @param _low  inizialized lower value of distribution
 * @param _high inizialized higher value of distribution
 * */
  UniformDoubleRndGen(double _low=0, double _high=1) : low(_low), high(_high) {};
  /** @fn double get()
 * @brief Get value for random genrator 
 * @return New random value from uniform generator*/
  double get() { return genunf(low, high); };
};

////////////////////////////////////////////////////////////////////////////////
//

/** @class UniformIntRndGen rndgen.h <common/rndgen.h>
 * @brief  Define Uniform Distribution for natural random generation
 * @ingroup rand */
class UniformIntRndGen
{
protected:
/** @var long int low
 * @brief Lower value of distribution*/

/** @var long int high
 * @brief Higher value of distribution*/

  long int low, high;

public:
/** @fn UniformIntRndGen(long int _low=0, long int _high=1) : low(_low), high(_high) 
 * @brief Class constructor
 * @param _low  inizialized lower value of distribution
 * @param _high inizialized higher value of distribution
 * */
  UniformIntRndGen(long int _low=0, long int _high=1) : low(_low), high(_high) {};
  
  /** @fn long int get()
 * @brief Get value for random genrator 
 * @return New random value from uniform generator*/

  long int get() { return ignuin(low, high); };
};

////////////////////////////////////////////////////////////////////////////////
//
/** @class ExponentialRndGen rndgen.h <common/rndgen.h>
 * @brief Exponential Distribution function for double precision random generator
 * */
class ExponentialRndGen
{
protected:
/** @var double mean 
 * @brief Mean value of distribution
 * */
  double mean;

public:
/** @fn ExponentialRndGen(double _mean=0) : mean(_mean)
 * @brief Class constructor
 * @param _mean Mean value to inizialized (default 0)
 * */
  ExponentialRndGen(double _mean=0) : mean(_mean) {};
  /** @fn double get()
 * @brief Get value for random genrator 
 * @return New random value*/
  double get() { return genexp(mean); };
};

////////////////////////////////////////////////////////////////////////////////
//
/** @class GaussianRndGen rndgen.h <common/rndgen.h>
 * @brief Gaussian distribution for generate random value
 * */
class GaussianRndGen
{
protected:
/** @var double mean
 * @brief mean value of distribution */
/** @var double sigma
 * @brief Standard deviation of distribution */
  double mean, sigma;

public:
/** @fn GaussianRndGen(double _mean=0, double _sigma=0) : mean(_mean), sigma(_sigma)
 * @brief Class Construction
 * @param _mean Mean value to inizialized distribution
 * @param _sigma Standard deviation to inizialized distribution
 * */
  GaussianRndGen(double _mean=0, double _sigma=0) : mean(_mean), sigma(_sigma) {};
  /** @fn double get()
 * @brief Get value for random genrator 
 * @return New random value*/
  double get() { return gennor(mean, sigma); };
};

////////////////////////////////////////////////////////////////////////////////
//

/** @class Geometric0RndGen rndgen.h <common/rndgen.h>
 * @brief Geometrical zero  mean distribution
 * */
class Geometric0RndGen
{
protected:
/** @var double mean 
 * @brief Mean value of distribution
 * */
  double mean;
/** @var double log_mean 
 * @brief Mean value of distribution
 * */

  double log_mean;

public:
/** @fn Geometric0RndGen(double _mean=0)
 * @brief Class Construction
 * @param _mean Mean value to inizialized distribution
 * */
  Geometric0RndGen(double _mean=0) 
    : mean(_mean)
  {
    if( _mean > 0 )
      log_mean = log(_mean / (_mean + 1));
  };

/** @fn unsigned long int get() 
 * @brief Get value for random genrator 
 * @return New random value*/
  unsigned long int get() 
  { 
    return ((mean > 0) ? (unsigned long int)(floor(log(1.0 - ranf()) / log_mean)) : 0);
  };
};

////////////////////////////////////////////////////////////////////////////////
//

/** @class Geometric1RndGen rndgen.h <commo/rndgen.h>
 * @brief Geometrical One  mean distribution
 * */
class Geometric1RndGen
{
protected:
/** @var double mean 
 * @brief Mean value of distribution
 * */
  double mean;
  /** @var double log_mean 
 * @brief Log mean value of distribution
 * */
  double log_mean;

public:
/** @fn Geometric1RndGen(double _mean=0)
 * @brief Class Construction
 * @param _mean Mean value to inizialized distribution
 * */

  Geometric1RndGen(double _mean=1) 
    : mean(_mean)
  {
    if( _mean > 1 )
      log_mean = log((_mean -1) / _mean);
  };

/** @fn unsigned long int get() 
 * @brief Get value for random genrator 
 * @return New random value*/
  unsigned long int get() 
  { 
    return ((mean > 1) ? (unsigned long int)(floor(log(1.0 - ranf()) / log_mean)) + 1 : 1 );
  };
};

////////////////////////////////////////////////////////////////////////////////
//
/** @class ParetoRndGen rndgen.h <commo/rndgen.h>
 * @brief Pareto distribution
 * */
class ParetoRndGen
{
protected:
/** @var double location
 * @brief location of distribution
 * */
  double location;
  /** @var double shape
   * @brief shape parameter
   * */
  double shape;

public:
/** @fn ParetoRndGen(double _location=1, double _shape=1) 
    : location(_location), shape(_shape)
 * @brief Class Construction
 * @param _location Distribution Location (default = 1)
 * @param _shape Distribution Shape (default = 1)
 * */
  ParetoRndGen(double _location=1, double _shape=1) 
    : location(_location), shape(_shape)
  {};

  /** @fn double get()
 * @brief Get value for random genrator 
 * @return New random value*/
  double get() 
  { 
    return rndPareto(location, shape);
  };
};

////////////////////////////////////////////////////////////////////////////////
//

/** @class WeibullRndGen rndgen.h <common/rndgen.g>
 * @brief Weibull Random Generator
 * @ingroup rand
 * */
class WeibullRndGen
{
protected:
/** @var double location
 * @brief location of distribution
 * */
  double location;
  /** @var double scale
   * @brief scale value */
  double scale;
  /** @var double shape
   * @brief shape parameter
   * */
  double shape;

public:
/** @fn WeibullRndGen(double _location=1, double _scale=1, double _shape=1) 
    : location(_location), scale(_scale), shape(_shape)
    * @brief Class construction
    * @param _location Distribution location
    * @param _scale Scale parameter to inizialized class
    * @param _shape Shape paramiter to inizialized  class 
    * */
  WeibullRndGen(double _location=1, double _scale=1, double _shape=1) 
    : location(_location), scale(_scale), shape(_shape)
  {};
/** @fn double get()
 * @brief Get value for random genrator 
 * @return New random value*/
  double get() 
  { 
    return rndWeibull(location, scale, shape);
  };
};

#endif


/*
$Log: rndgen.h,v $
Revision 1.1.1.1  2002/09/11 07:18:36  ronga


Revision 1.6  2002/07/26 13:24:34  pecos
Added Negative Binomial distribution

Revision 1.1  2002/07/26 13:20:33  pecos
*** empty log message ***

Revision 1.5  2001/03/17 09:47:56  pecos
Added default params for Pareto and Weibull

Revision 1.4  2000/02/24 08:36:21  pecos
no message

Revision 1.3  1999/09/11 09:14:13  pecos
*** empty log message ***

Revision 1.2  1999/05/28 23:54:48  pecos
no message

Revision 1.1.1.1  1999/05/24 15:59:57  inesis
INeSiS Project

Revision 1.1.1.1  1999/05/24 15:17:20  inesis
INeSiS Project

Revision 1.2  1999/01/26 11:24:23  pecos
*** empty log message ***

Revision 1.2  1998/09/29 18:56:53  nanni
*** empty log message ***

Revision 1.1  1998/09/15 15:20:24  ronga
Initial revision

*/

