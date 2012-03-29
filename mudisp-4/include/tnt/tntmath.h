// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology



// Header file for scalar math functions

#ifndef TNTMATH_H
#define TNTMATH_H

// conventional functions required by several matrix algorithms


/*!  @namespace TNT  Template Numerical Toolkit 
 * @brief  Template Numerical Toolkit (TNT) for Linear Algebra
 * @author R. Pozo
 * @version BETA
 * @note  Copyright  Mathematical and Computational Sciences Division National Institute of Standards and Technology
* @ingroup  matrix 
*  */
namespace TNT 
{
/*! @fn inline double abs(double t)
 * @brief Valore Assoluto
 * @param t valore in ingresso
 * @return Valore assoluto
 * @ingroup  matrix 
 */
inline double abs(double t)
{
    return ( t > 0 ? t : -t);
}

/*! @fn inline double min(double a, double b)
 * @brief Restituisce il minore 
 * @param a valore in ingresso
 * @param b valore in ingresso
 * @return Valore Minore tra i due parametri
 * @ingroup  matrix 
 */
inline double min(double a, double b)
{
    return (a < b ? a : b);
}

/*! @fn inline double max(double a, double b)
 * @brief Restituisce il massimo 
 * @param a valore in ingresso
 * @param b valore in ingresso
 * @return Valore massimo tra i due parametri
 * @ingroup  matrix 
 */
inline double max(double a, double b)
{
    return (a > b ? a : b);
}
/*! @fn inline float abs(float t)
 * @brief Valore Assoluto
 * @param t valore in ingresso
 * @return Valore assoluto
 * @ingroup  matrix 
 */
inline float abs(float t)
{
    return ( t > 0 ? t : -t);
}

/*! @fn inline float min(float a, float b)
 * @brief Restituisce il minore 
 * @param a valore in ingresso
 * @param b valore in ingresso
 * @return Valore Minore tra i due parametri
 * @ingroup  matrix 
 */
inline float min(float a, float b)
{
    return (a < b ? a : b);
}

/*! @fn inline float max(float a, float b)
 * @brief Restituisce il massimo 
 * @param a valore in ingresso
 * @param b valore in ingresso
 * @return Valore massimo tra i due parametri
 * @ingroup  matrix 
 */
inline float max(float a, float b)
{
    return (a > b ? a : b);
}

/*! @fn inline double sign(double a)
 * @brief Segno
 * @param a valore in ingresso
 * @return Segno del parametro
 * @ingroup  matrix 
 */
inline double sign(double a)
{
    return (a > 0 ? 1 : -1);
}


/*! @fn inline float sign(float a)
 * @brief Segno
 * @param a valore in ingresso
 * @return Segno del parametro
 * @ingroup  matrix 
 */
inline float sign(float a)
{
    return (a > 0 ? 1 : -1);
}

} /* namespace TNT */


#endif
// TNTMATH_H
