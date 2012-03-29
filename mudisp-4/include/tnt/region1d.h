// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology



#ifndef REGION1D_H
#define REGION1D_H


#include "subscrpt.h"
#include "index.h"
#include <iostream>
#include <cassert>

namespace TNT
{

/*! @class TNT::const_Region1D region1d.h <tnt/region1d.h>
 * @brief Classe per la gestione di Array monodimensionali 
 * @ingroup matrix */
template <class Array1D>
class const_Region1D;
/*! @class TNT::Region1D region1d.h <tnt/region1d.h>
 * @brief Classe per la gestione di Array monodimensionali 
 * @ingroup matrix */
template <class Array1D>
class Region1D
{
    protected:
	/*! @var Array1D &  A_
	 * @brief Define a contenitor parameter A_ 
	 * */
        Array1D &  A_;
        /*! @var  Subscript offset_
         * @brief offset parameter (0-base)
         * */ 
        Subscript offset_;          // 0-based
        /*! @var Subscript dim_
         * @brief Array size
         * */
        Subscript dim_;
/*! @typedef typename Array1D::element_type T
 * @brief Define template element type
 * */
        typedef typename Array1D::element_type T;

    public:
    /*! @fn const Array1D & array()  const 
     * @return A_ parameter
     * */
        const Array1D & array()  const { return A_; }
    /*! @fn Subscript offset() const
     * @return offset_ parameter
     * */
        Subscript offset() const { return offset_;}
     /*! @fn   Subscript dim() const
      * @return dim_ parameter
      * */
        Subscript dim() const { return dim_; }
	/*! @fn Subscript offset(Subscript i) const
	 * @brief Check if offset class parameter is equal to pass parameter
	 * @param i New class offset
	 * */
        Subscript offset(Subscript i) const
        {
#ifdef TNT_BOUNDS_CHECK
            assert(i==TNT_BASE_OFFSET);
#endif
            return offset_;
        }

	/*! @fn Subscript dim(Subscript i) const
	 * @brief Check if offset dimension parameter is equal to pass parameter
	 * @param i New class offset
	 * @return offset_ value
	 * */
        Subscript dim(Subscript i) const
        {
#ifdef TNT_BOUNDS_CHECK
            assert(i== TNT_BASE_OFFSET);
#endif
            return offset_;
        }

/*! @fn Region1D(Array1D &A, Subscript i1, Subscript i2) 
 * @brief Class Constructor
 * @param A Array Vector to be copied in class Container
 * @param i1 Array First Element Index
 * @param i2 Array Last Element Index
 * */
        Region1D(Array1D &A, Subscript i1, Subscript i2) : A_(A)
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET <= i1 );
            assert(i2 <= A.dim() + (TNT_BASE_OFFSET-1));
            assert(i1 <= i2);
#endif
            offset_ = i1 - TNT_BASE_OFFSET;
            dim_ = i2-i1 + 1;
        }

/*! @fn Region1D(Array1D &A, const Index1D &I) : A_(A)
 * @brief Class Constructor
 * @param A Array Vector to be copied in class Container
 * @param I Subscript Element range with  First&Last Index  Element
 * */
        Region1D(Array1D &A, const Index1D &I) : A_(A)
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET <=I.lbound());
            assert(I.ubound() <= A.dim() + (TNT_BASE_OFFSET-1));
            assert(I.lbound() <= I.ubound());
#endif
            offset_ = I.lbound() - TNT_BASE_OFFSET;
            dim_ = I.ubound() - I.lbound() + 1;
        }

/*! @fn Region1D(Region1D<Array1D>  &A, Subscript i1, Subscript i2) 
 * @brief Class Constructor
 * @param A Array Vector to be copied in class Container
 * @param i1  First Index  Element
 * @param i2  Last Index  Element
 * */
        Region1D(Region1D<Array1D> &A, Subscript i1, Subscript i2) :
                A_(A.A_)
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET <= i1 );
            assert(i2 <= A.dim() + (TNT_BASE_OFFSET - 1));
            assert(i1 <= i2);
#endif
                    //     (old-offset)        (new-offset)
                    //
            offset_ =  (i1 - TNT_BASE_OFFSET) + A.offset_;
            dim_ = i2-i1 + 1;
        }

/*! @fn Region1D<Array1D> operator()(Subscript i1, Subscript i2)
 * @brief Operator for read Array Region Value
 * @param i1  First Index  Element
 * @param i2  Last Index  Element
 * @return Array region from \f$i_1\f$ to \f$i_2\f$
 * */
        Region1D<Array1D> operator()(Subscript i1, Subscript i2)
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET <= i1);
            assert(i2 <= dim() + (TNT_BASE_OFFSET -1));
            assert(i1 <= i2);
#endif
                    // offset_ is 0-based, so no need for
                    //  ( - TNT_BASE_OFFSET)
                    //
            return Region1D<Array1D>(A_, i1+offset_,
                    offset_ + i2);
        }

/*! @fn Region1D<Array1D> operator()(const Index1D &I)
 * @brief Operator for read Array Region Value
 * @param I Subscript Element range with  First&Last Index  Element
 * @return Array region 
 * */
        Region1D<Array1D> operator()(const Index1D &I)
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET<=I.lbound());
            assert(I.ubound() <= dim() + (TNT_BASE_OFFSET-1));
            assert(I.lbound() <= I.ubound());
#endif
            return Region1D<Array1D>(A_, I.lbound()+offset_,
                offset_ + I.ubound());
        }



/*! @fn T & operator()(Subscript i)
 * @brief Operator for read Array Value
 * @param i Array Index  Element to read
 * @return \f$A(i)\f$
 * */
        T & operator()(Subscript i)
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET <= i);
            assert(i <=  dim() + (TNT_BASE_OFFSET-1));
#endif
            return A_(i+offset_);
        }

/*! @fn const T & operator() (Subscript i) const 
 * @brief Operator for read Array Value
 * @param i Array Index  Element to read
 * @return \f$A(i)\f$
 * */
        const T & operator() (Subscript i) const
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET <= i);
            assert(i <= dim() + (TNT_BASE_OFFSET-1));
#endif
            return A_(i+offset_);
        }


/** @fn  Region1D<Array1D> & operator=(const Region1D<Array1D> &R)
 * @brief Assign Operator
 * @param R Vector value to be assign to Array 
 * @tparam Array1D 
 * @return Reference pointer to refres Array with assign values of R
 * */ 
        Region1D<Array1D> & operator=(const Region1D<Array1D> &R)
        {
            // make sure both sides conform
            assert(dim() == R.dim());

            Subscript N = dim();
            Subscript i;
            Subscript istart = TNT_BASE_OFFSET;
            Subscript iend = istart + N-1;

            for (i=istart; i<=iend; i++)
                (*this)(i) = R(i);

            return *this;
        }


/** @fn  Region1D<Array1D> & operator=(const const_Region1D<Array1D> &R)
 * @brief Assign Operator
 * @param R Vector value to be assign to Array 
 * @tparam Array1D 
 * @return Reference pointer to refres Array with assign values of R
 * */ 
        Region1D<Array1D> & operator=(const const_Region1D<Array1D> &R)
        {
            // make sure both sides conform
            assert(dim() == R.dim());

            Subscript N = dim();
            Subscript i;
            Subscript istart = TNT_BASE_OFFSET;
            Subscript iend = istart + N-1;

            for (i=istart; i<=iend; i++)
                (*this)(i) = R(i);

            return *this;

        }

/** @fn Region1D<Array1D> & operator=(const T& t)
 * @brief Operator. Assign parameter value at current index element of Array
 * @param t value to be assign
 * @tparam Array1D  
 * @return Reference pointer to update array
 * */
        Region1D<Array1D> & operator=(const T& t)
        {
            Subscript N=dim();
            Subscript i;
            Subscript istart = TNT_BASE_OFFSET;
            Subscript iend = istart + N-1;

            for (i=istart; i<= iend; i++)
                (*this)(i) = t;

            return *this;

        }

/** @fn  Region1D<Array1D> & operator=(const Array1D &R)
 * @brief Assign Operator
 * @param R Vector value to be assign to Array 
 * @tparam Array1D 
 * @return Reference pointer to refres Array with assign values of R
 * */ 
        Region1D<Array1D> & operator=(const Array1D &R)
        {
            // make sure both sides conform
            Subscript N = dim();
            assert(dim() == R.dim());

            Subscript i;
            Subscript istart = TNT_BASE_OFFSET;
            Subscript iend = istart + N-1;

            for (i=istart; i<=iend; i++)
                (*this)(i) = R(i);

            return *this;

        }

};

/** @fn template <class Array1D> std::ostream& operator<<(std::ostream &s, Region1D<Array1D> &A) 
 * @brief Output Operatore
 * @param s Standard Out
 * @param A reference to Matrix pointer
 * @return Stream Matrix \f$ A\f$ on standard out
 * */
template <class Array1D>
std::ostream& operator<<(std::ostream &s, Region1D<Array1D> &A)
{
    Subscript N=A.dim();
    Subscript istart = TNT_BASE_OFFSET;
    Subscript iend = N - 1 + TNT_BASE_OFFSET;

    for (Subscript i=istart; i<=iend; i++)
        s << A(i) << endl;

    return s;
}


/*  ---------  class const_Region1D ------------ */

/** @class const_Region1D region1d.h <tnt/region1d.h>
 * @brief Define a vector region operation class 
 * @tparam  Array1D
 * @ingroup matrix */
template <class Array1D>
class const_Region1D
{
    protected:
	/*! @var const Array1D &  A_
	 * @brief Define a contenitor parameter A_ 
	 * */
        const Array1D &  A_;

        /*! @var  Subscript offset_
         * @brief offset parameter (0-base)
         * */ 
        Subscript offset_;          // 0-based
         /*! @var Subscript dim_
         * @brief Array size
         * */
        Subscript dim_;
/*! @typedef typename Array1D::element_type T
 * @brief Define template element type
 * */
       typedef typename Array1D::element_type T;

    public:
      /*! @fn const Array1D & array()  const 
     * @return A_ parameter
     * */  
        const Array1D & array()  const { return A_; }
    /*! @fn Subscript offset() const
     * @return offset_ parameter
     * */
        Subscript offset() const { return offset_;}
     /*! @fn   Subscript dim() const
      * @return dim_ parameter
      * */    
        Subscript dim() const { return dim_; }
	/*! @fn Subscript offset(Subscript i) const
	 * @brief Check if offset class parameter is equal to pass parameter
	 * @param i New class offset
	 * */
        Subscript offset(Subscript i) const
        {
#ifdef TNT_BOUNDS_CHECK
            assert(i==TNT_BASE_OFFSET);
#endif
            return offset_;
        }

	/*! @fn Subscript dim(Subscript i) const
	 * @brief Check if offset dimension parameter is equal to pass parameter
	 * @param i New class offset
	 * @return offset_ value
	 * */
        Subscript dim(Subscript i) const
        {
#ifdef TNT_BOUNDS_CHECK
            assert(i== TNT_BASE_OFFSET);
#endif
            return offset_;
        }

/*! @fn const_Region1D(const Array1D &A, Subscript i1, Subscript i2) 
 * @brief Class Constructor
 * @param A Array Vector to be copied in class Container
 * @param i1 Array First Element Index
 * @param i2 Array Last Element Index
 * */
        const_Region1D(const Array1D &A, Subscript i1, Subscript i2) : A_(A)
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET <= i1 );
            assert(i2 <= A.dim() + (TNT_BASE_OFFSET-1));
            assert(i1 <= i2);
#endif
            offset_ = i1 - TNT_BASE_OFFSET;
            dim_ = i2-i1 + 1;
        }

/*! @fn const_Region1D(const Array1D &A, const Index1D &I) : A_(A)
 * @brief Class Constructor
 * @param A Array Vector to be copied in class Container
 * @param I Subscript Element range with  First&Last Index  Element
 * */
        const_Region1D(const Array1D &A, const Index1D &I) : A_(A)
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET <=I.lbound());
            assert(I.ubound() <= A.dim() + (TNT_BASE_OFFSET-1));
            assert(I.lbound() <= I.ubound());
#endif
            offset_ = I.lbound() - TNT_BASE_OFFSET;
            dim_ = I.ubound() - I.lbound() + 1;
        }
/*! @fn const_Region1D(const_Region1D<Array1D> &A, Subscript i1, Subscript i2) :  A_(A.A_)
 * @brief Class Constructor
 * @param A Array Vector to be copied in class Container
 * @param i1  First Index  Element
 * @param i2  Last Index  Element
 * */

        const_Region1D(const_Region1D<Array1D> &A, Subscript i1, Subscript i2) :
                A_(A.A_)
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET <= i1 );
            assert(i2 <= A.dim() + (TNT_BASE_OFFSET - 1));
            assert(i1 <= i2);
#endif
                    //     (old-offset)        (new-offset)
                    //
            offset_ =  (i1 - TNT_BASE_OFFSET) + A.offset_;
            dim_ = i2-i1 + 1;
        }

/*! @fn const_Region1D<Array1D> operator()(Subscript i1, Subscript i2)
 * @brief Operator for read Array Region Value
 * @param i1  First Index  Element
 * @param i2  Last Index  Element
 * @return Array region from \f$i_1\f$ to \f$i_2\f$
 * */
        const_Region1D<Array1D> operator()(Subscript i1, Subscript i2)
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET <= i1);
            assert(i2 <= dim() + (TNT_BASE_OFFSET -1));
            assert(i1 <= i2);
#endif
                    // offset_ is 0-based, so no need for
                    //  ( - TNT_BASE_OFFSET)
                    //
            return const_Region1D<Array1D>(A_, i1+offset_,
                    offset_ + i2);
        }

/*! @fn const_Region1D<Array1D> operator()(const Index1D &I)
 * @brief Operator for read Array Region Value
 * @param I Subscript Element range with  First&Last Index  Element
 * @return Array region 
 * */
        const_Region1D<Array1D> operator()(const Index1D &I)
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET<=I.lbound());
            assert(I.ubound() <= dim() + (TNT_BASE_OFFSET-1));
            assert(I.lbound() <= I.ubound());
#endif
            return const_Region1D<Array1D>(A_, I.lbound()+offset_,
                offset_ + I.ubound());
        }

/*! @fn const T & operator() (Subscript i) const 
 * @brief Operator for read Array Value
 * @param i Array Index  Element to read
 * @return \f$A(i)\f$
 * */
        const T & operator() (Subscript i) const
        {
#ifdef TNT_BOUNDS_CHECK
            assert(TNT_BASE_OFFSET <= i);
            assert(i <= dim() + (TNT_BASE_OFFSET-1));
#endif
            return A_(i+offset_);
        }




};

/*! @fn template <class Array1D> std::ostream& operator<<(std::ostream &s, const_Region1D<Array1D> &A)
 * @brief Stream the full Array in the standard out
* @param s standard out definition
* @param A Array  to write in the standard out
* @return standard out 
* @ingroup matrix
* */ 
template <class Array1D>
std::ostream& operator<<(std::ostream &s, const_Region1D<Array1D> &A)
{
    Subscript N=A.dim();

    for (Subscript i=1; i<=N; i++)
        s << A(i) << endl;

    return s;
}


} // namespace TNT

#endif
// const_Region1D_H
