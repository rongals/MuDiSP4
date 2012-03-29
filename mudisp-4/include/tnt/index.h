// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// Vector/Matrix/Array Index Module  

#ifndef INDEX_H
#define INDEX_H

#include "subscrpt.h"

namespace TNT
{
/*! @class Index1D index.h <tnt/index.h>
 * @brief Index definiton class
 * @tparam lbound_ Lower index Limit parameter
 * @tparam ubounde_ Upper index Limit parameter
 * @ingroup matrix
 * */
class Index1D
{
	/*! @\property  Subscript lbound_
	 * Lower limit for Index */
    Subscript lbound_;
    /*! @\property  Subscript ubound_
	 * Upper limit for Index */
    Subscript ubound_;

    public:
    /*! @fn Subscript lbound() const
     * @brief return lower limit value
     * @return Lower limit
     * */
    Subscript lbound() const { return lbound_; }
        /*! @fn Subscript ubound() const
     * @brief return upper limit value
     * @return Upper limit
     * */
    Subscript ubound() const { return ubound_; }
   /*! @fn Index1D(const Index1D &D) 
    * @brief Class costructor
    * @param D Array Object passed to class
    **/
    Index1D(const Index1D &D) : lbound_(D.lbound_), ubound_(D.ubound_) {}
    /*! @fn  Index1D(Subscript i1, Subscript i2)
     * @brief Class Custructor
     * @param i1 first index
     * @param i2 second index
     * @return Object index limit
     * */
    Index1D(Subscript i1, Subscript i2) : lbound_(i1), ubound_(i2) {}

/*! @fn Index1D & operator=(const Index1D &D)
 * @brief Assign limit to class object index
 * @param D index type Object
 * */
    Index1D & operator=(const Index1D &D)
    {
        lbound_ = D.lbound_;
        ubound_ = D.ubound_;
        return *this;
    }

};

/*! @fn inline Index1D operator+(const Index1D &D, Subscript i)
 * @brief Operator to add \a i value to index lower/upper limit
 * @param D Index object 
 * @param i value to sum to Object class limit
 * @return new dimension  limit
 * */  
inline Index1D operator+(const Index1D &D, Subscript i)
{
    return Index1D(i+D.lbound(), i+D.ubound());
}

/*! @fn inline Index1D operator+(Subscript i, const Index1D &D)
 * @brief Operator to add \a D lower/upper limit to \a i parameter 
 * @param i value tu sub to Object class limit
 * @param D Index object 
 * @return new dimension limit
 * */
inline Index1D operator+(Subscript i, const Index1D &D)
{
    return Index1D(i+D.lbound(), i+D.ubound());
}

/*! @fn inline Index1D operator-(Index1D &D, Subscript i)
 * @brief Operator to sub \a i  value to index lower/upper limit \a D
 * @param D Index object 
 * @param i value tu sub to Object class limit
 * @return new dimension  limit
 * */

inline Index1D operator-(Index1D &D, Subscript i)
{
    return Index1D(D.lbound()-i, D.ubound()-i);
}
/*! @fn inline Index1D operator-(Subscript i, Index1D &D)
 * @brief Operator to sub \a D lower/upper limit  value to parameter /a i
 * @param i value tu sub to Object class limit
 * @param D Index object 
 * @return new dimension  limit
 * */
inline Index1D operator-(Subscript i, Index1D &D)
{
    return Index1D(i-D.lbound(), i-D.ubound());
}

} // namespace TNT

#endif

