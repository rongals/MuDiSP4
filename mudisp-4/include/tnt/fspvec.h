// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


//  Templated sparse vector (Fortran conventions).
//  Used primarily to interface with Fortran sparse matrix libaries.
//  (CANNOT BE USED AS AN STL CONTAINER.)


#ifndef FSPVEC_H
#define FSPVEC_H

#include "tnt.h"
#include "vec.h"
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <sstream>

using namespace std;

namespace TNT
{

/** @class  Fortran_Sparse_Vector  fspvec.h <tnt/fspvec.h>
 * @brief    Templated sparse vector (Fortran conventions). Used primarily to interface with Fortran sparse matrix libaries.
 * @warning CANNOT BE USED AS AN STL CONTAINER.
 * @tparam  T template type
 * @ingroup matrix
 * */
template <class T>
class Fortran_Sparse_Vector 
{


  public:
   /*! @typedef Subscript   size_type;
    * @brief Index size_type*/
    typedef Subscript   size_type;
  /*! @typedef T   value_type
   * @brief define template type for type of value */
    typedef         T   value_type;
     /*! @typedef T   element_type
     * @brief Define template type  to declarate type of Object*/
    typedef         T   element_type;
    /*! @typedef  T* pointer
     * @brief define template pointer */     
    typedef         T*  pointer;
    /*! @typedef T* iterator
     * @brief Define template pointer iterator*/    
    typedef         T*  iterator;
    /*! @typedef T& reference
     * @brief Define tempate to alias reference */       
    typedef         T&  reference;
    /*! @typedef T* const_iterator 
     * @brief Define template constant iterator for class */       
    typedef const   T*  const_iterator;
	/*! @typedef const T& const_reference 
	 * @brief Define template constant reference for class */    
    typedef const   T&  const_reference;

 /*! @fn     Subscript lbound() const 
  * @return Inferior Limit for Subscript Index
  * */
    Subscript lbound() const { return 1;}
 
  protected:
  /** @var Vector<T>   val_
   * @brief Define  Vector Containder
   * */
    Vector<T>   val_;
    
  /** @var   Vector<Subscript> index_
   * @brief Define a vector of index elements
   * */
    Vector<Subscript> index_;
	/** @var Subscript dim_
	 * @brief Prescribed dimension
	 * */
    Subscript dim_;                  // prescribed dimension


  public:

    // size and shape information
    /** @fn Subscript dim() const
     * @brief Method to get vector dimension
     * @return Vector dimension in \a Subscript
     * */
    Subscript dim() const { return dim_; }
    
    /** @fn Subscript num_nonzeros() const
     * @brief Method to get vector elements
     * @return Vector elements in \a Subscript
     * */
    Subscript num_nonzeros() const { return val_.dim(); }

    // access
	/** @fn T& val(Subscript i)
	 * @brief  Method to read element of vector
	 * @param i index of element
	 * @return value of elements indexed 
	 * */
    T& val(Subscript i) { return val_(i); }
 	
 	/** @fn  const T& val(Subscript i) const
	 * @brief  Method to read element of vector
	 * @param i index of element
	 * @return value of elements indexed 
	 * */   
    const T& val(Subscript i) const { return val_(i); }

    /** @fn  Subscript &index(Subscript i)
     * @brief Method for get current index vector index
     * @param i index
     * @return */
    Subscript &index(Subscript i) { return index_(i); }
    
        /** @fn  const Subscript &index(Subscript i) const
     * @brief Method for get current index vector index
     * @param i index
     * @return */
    const Subscript &index(Subscript i) const { return index_(i); }

    // constructors
	/** @fn Fortran_Sparse_Vector() : val_(), index_(), dim_(0) 
	 * @brief Class constructor 
	 * */
    Fortran_Sparse_Vector() : val_(), index_(), dim_(0)  {};
    /** @fn Fortran_Sparse_Vector(Subscript N, Subscript nz) : val_(nz), index_(nz), dim_(N) 
	 * @brief Class constructor 
	 * @param N Vector dimension
	 * @param nz Numer of elements
	 * */
    Fortran_Sparse_Vector(Subscript N, Subscript nz) : val_(nz), 
            index_(nz), dim_(N)  {};
	/** @fn Fortran_Sparse_Vector(Subscript N, Subscript nz, const T *values, const Subscript *indices): val_(nz, values), index_(nz, indices), dim_(N)
	 * @brief Class constructor,  initialize whit parameter value
	 * @param N Vector dimension
	 * @param nz Numer of elements
	 * @param values pointer at value to inizialied Vector
	 * @param indices pointer to indices elements
	 * */
    Fortran_Sparse_Vector(Subscript N, Subscript nz, const T *values,
        const Subscript *indices): val_(nz, values), index_(nz, indices),
            dim_(N) {}

	/** @fn Fortran_Sparse_Vector(const Fortran_Sparse_Vector<T> &S): val_(S.val_), index_(S.index_), dim_(S.dim_) {}
	 * @brief Class constructor initialize from existing Vector
	 * @tparam S pre-existing Vector pointer be passed to new Object
	 * */
    Fortran_Sparse_Vector(const Fortran_Sparse_Vector<T> &S): 
        val_(S.val_), index_(S.index_), dim_(S.dim_) {}

    // initialize from string, e.g.
    //
    //  Fortran_Sparse_Vector<T> A(N, 2, "1.0 2.1", "1 3");
    //
    
  	/** @fn Fortran_Sparse_Vector(Subscript N, Subscript nz, char *v, char *ind) : val_(nz, v), index_(nz, ind), dim_(N)
	 * @brief Class constructor  initialize from string
	 * @param N Vector dimension
	 * @param nz Numer of elements
	 * @param v pointer at value to inizialied Vector
	 * @param ind pointer to indices elements
	 * */  
    Fortran_Sparse_Vector(Subscript N, Subscript nz, char *v,
        char *ind) : val_(nz, v), index_(nz, ind), dim_(N) {}
    
    // assignments

/** @fn Fortran_Sparse_Vector<T> & newsize(Subscript N, Subscript nz)
 * @brief Method for resize vector
 * @param N  Number vector Dimension
 * @param nz Number of elements in new resize vector
 * @return Refresh vector 
 * */ 
    Fortran_Sparse_Vector<T> & newsize(Subscript N, Subscript nz)
    {
        val_.newsize(nz);
        index_.newsize(nz);
        dim_ = N;
        return *this;
    }

/** @fn Fortran_Sparse_Vector<T> & operator=( const Fortran_Sparse_Vector<T> &A)
 * @brief Assignment Operator
 * @param A Fortran Input vector 
 * @return Refresh vector with new data value
 * */ 
    Fortran_Sparse_Vector<T> & operator=( const Fortran_Sparse_Vector<T> &A)
    {
        val_ = A.val_;
        index_ = A.index_;
        dim_ = A.dim_;

        return *this;
    }

    // methods

/** @fn  friend istream& operator>> (istream &s, Fortran_Sparse_Vector<T> &A)
 * @brief Operator to stream in from standard in
 * @param s Standard Input
 * @param A Vector read in from standard Input
 *  @return Data stream in to vector from standard Input
 * */
    friend istream& operator>> (istream &s, Fortran_Sparse_Vector<T> &A);

};


/* ***************************  I/O  ********************************/

/** @fn template <class T> ostream& operator<<(ostream &s, const Fortran_Sparse_Vector<T> &A)
 * @brief Operator to stream out to standard out
 * @param s Standard Out
 * @param A Vector to be print in standard out
 * */
template <class T>
ostream& operator<<(ostream &s, const Fortran_Sparse_Vector<T> &A)
{
    // output format is :   N nz val1 ind1 val2 ind2 ... 
    Subscript nz=A.num_nonzeros();

    s <<  A.dim() << " " << nz << endl;

    for (Subscript i=1; i<=nz; i++)
        s   << A.val(i) << "  " << A.index(i) << endl;
    s << endl;

    return s;
}

/** @fn template <class T> istream& operator>>(istream &s, Fortran_Sparse_Vector<T> &A)
 * @brief Operator to stream in from standard in
 * @param s Standard Input
 * @param A Vector read in from standard Input
 * @tparam T template type parameter
 *  @return Data stream in to vector from standard Input
 * */
 
template <class T>
istream& operator>>(istream &s, Fortran_Sparse_Vector<T> &A)
{
    // output format is :   N nz val1 ind1 val2 ind2 ... 

    Subscript N;
    Subscript nz;

    s >> N >> nz;

    A.newsize(N, nz);

    for (Subscript i=1; i<=nz; i++)
            s >>  A.val(i) >> A.index(i);


    return s;
}

} // namespace TNT

#endif
// FSPVEC_H
