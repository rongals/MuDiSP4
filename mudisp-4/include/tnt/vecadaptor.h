// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


#ifndef VECADAPTOR_H
#define VECADAPTOR_H

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cassert>

#include "subscrpt.h"

#ifdef TNT_USE_REGIONS
#include "region1d.h"
#endif

namespace TNT
{
/*! @class TNT::Vector_Adaptor vecadaptor.h <tnt/vecadaptor.h>
 *  @brief Minimal Subset of service for STL vector class
 * @note    see "tntreq.h" for TNT requirements for underlying vector
  class.  This need NOT be the STL vector<> class, but a subset
  that provides minimal services.
 * @par
  This is a container adaptor that provides the following services.

      -  adds 1-offset operator() access ([] is always 0 offset)
      -  adds TNT_BOUNDS_CHECK to () and []
      -  adds initialization from strings, e.g.  "1.0 2.0 3.0";
      -  adds newsize(N) function (does not preserve previous values)
      -  adds dim() and dim(1)
      -  adds free() function to release memory used by vector
      -  adds regions, e.g. A(Index(1,10)) = ... 
      -  add getVector() method to return adapted container
      -  adds simple I/O for ostreams
 * @tparam class BBvec 
      *  @ingroup marix */
template <class BBVec>
class Vector_Adaptor
{

  public:
  /*! @typedef typename BBVec::value_type T 
   * @brief Define type BBVec */
    typedef   typename BBVec::value_type T;
    /*! @typedef T   value_type
     * @brief define template type for type of value
     * */
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
    Subscript lbound() const { return 1; }

  protected:
  /*! @var BBVec v_ 
   * @brief Class Object element*/
    BBVec v_;
    /*! @var T* vm1_
     * @brief Pointer to vector 
     * */
    T* vm1_;

  public:
/*! @fn Subscript size() const 
 * @return number of elements
 * */
    Subscript size() const { return v_.size(); }

    // These were removed so that the ANSI C++ valarray class
    // would work as a possible storage container.
    //
    //
    //iterator begin() { return v_.begin();}
    //iterator begin() { return &v_[0];}
    //
    //iterator end()   { return v_.end(); }
    //iterator end()   { return &v_[0] + v_.size(); }
    //
    //const_iterator begin() const { return v_.begin();}
    //const_iterator begin() const { return &v_[0];}
    //
    //const_iterator end()  const { return v_.end(); }
    //const_iterator end()  const { return &v_[0] + v_.size(); }

/*! @fn  BBVec& getVector()
 * @brief Method to return vector
 * @return Vector
 * */
    BBVec& getVector() { return v_; }
    /** @fn Subscript dim() const
     * @brief Method to get vector dimension
     * @return Vector dimension in \a Subscript
     * */
    Subscript dim() const { return v_.size(); }
     /** @fn Subscript dim(Subscript i)
     * @brief Method to get vector dimension from index parameter
     * @param i starting index
     * @return Vector dimension  from index to end
     * */
    Subscript dim(Subscript i)
    {
#ifdef TNT_BOUNDS_CHECK
        assert(i==TNT_BASE_OFFSET);
#endif
        return (i==TNT_BASE_OFFSET ? v_.size() : 0 );
    }
    /** @fn Vector_Adaptor() : v_() 
     * @brief Costructor for the class
     * */
    Vector_Adaptor() : v_() {};
     /** @overload Vector_Adaptor(const Vector_Adaptor<BBVec> &A) : v_(A.v_)
     * @brief Costructor for the class
     * */
    Vector_Adaptor(const Vector_Adaptor<BBVec> &A) : v_(A.v_) 
    { 
        vm1_ = ( v_.size() > 0 ? &(v_[0]) -1 : NULL); 

    } 
     /** @overload Vector_Adaptor(Subscript N, char *s) : v_(N)
     * @brief Costructor for the class
     * @param N Number of elements
     * @param s String of char
     * */

    Vector_Adaptor(Subscript N, /*const*/ char *s) : v_(N) 
    {
        istringstream ins(s);
        for (Subscript i=0; i<N; i++)
            ins >> v_[i] ;

        vm1_ = ( v_.size() > 0 ? &(v_[0]) -1 : NULL); 
    }; 

     /** @overload Vector_Adaptor(Subscript N, const T& value = T()) : v_(N)
     * @brief Costructor for the class, and inizialization
     * @param N Number of elements
     * @param value  Inizializzation value
     * */
    Vector_Adaptor(Subscript N, const T& value = T()) : v_(N)
    {
        for (Subscript i=0; i<N; i++)
             v_[i]  = value;
        
        vm1_ = ( v_.size() > 0 ? &(v_[0]) -1 : NULL); 
    }

   /** @overload Vector_Adaptor(Subscript N, const T* values) : v_(N)
     * @brief Costructor for the class, and inizialization
     * @param N Number of elements
     * @param values  pointer to inizializzation values.
     * */
    Vector_Adaptor(Subscript N, const T* values) : v_(N)
    {
        for (Subscript i=0; i<N; i++)
             v_[i]  = values[i];
        vm1_ = ( v_.size() > 0 ? &(v_[0]) -1 : NULL); 
    } 
    
    /** @overload Vector_Adaptor(const BBVec & A) : v_(A) 
     * @brief Costructor for the class, and inizialization
     * @param A Vector to be converted
     * */ 
    Vector_Adaptor(const BBVec & A) : v_(A) 
    {
        vm1_ = ( v_.size() > 0 ? &(v_[0]) -1 : NULL); 
    }

    // NOTE: this assumes that BBVec(0) constructor creates an 
    //  null vector that does not take up space...  It would be
    //  great to require that BBVec have a corresponding free()
    //  function, but in particular STL vectors do not.
    //
    
    /** Vector_Adaptor<BBVec>& free()
     * @brief this assumes that BBVec(0) constructor creates an null 
     * vector that does not take up space...  It would be great to 
     * require that BBVec have a corresponding free() function, but in 
     * particular STL vectors do not.
     * */
    Vector_Adaptor<BBVec>& free()
    {
        return *this = Vector_Adaptor<BBVec>(0);
    }

	/** @fn  Vector_Adaptor<BBVec>& operator=(const Vector_Adaptor<BBVec> &A) 
	 * @brief Assign Operator
	 * @param A Vector to be assigned
	 * */
    Vector_Adaptor<BBVec>& operator=(const Vector_Adaptor<BBVec> &A) 
    { 
        v_ = A.v_ ; 
        vm1_ = ( v_.size() > 0 ? &(v_[0]) -1 : NULL); 
        return *this;
    }

	/** @fn  Vector_Adaptor<BBVec>& newsize(Subscript N)
	 * @brief Resize the Vector
	 * @param N New size
	 * @note this is not as efficient as it could be
         but to retain compatiblity with STL interface
         we cannot assume underlying implementation
         has a newsize() function.
	 * */
    Vector_Adaptor<BBVec>& newsize(Subscript N)
    {
        // NOTE: this is not as efficient as it could be
        // but to retain compatiblity with STL interface
        // we cannot assume underlying implementation
        // has a newsize() function.

        return *this = Vector_Adaptor<BBVec>(N);

    }
	
	/** @fn  Vector_Adaptor<BBVec>& operator=(const T &a) 
	 * @brief Assign Operator
	 * @param a pointer to value to be assigned
	 * */
    Vector_Adaptor<BBVec>& operator=(const T &a) 
    {
        Subscript i;
        Subscript N = v_.size();    
        for (i=0; i<N; i++)
            v_[i] = a;

        return *this;
    }
	
    /** @fn  Vector_Adaptor<BBVec>& resize(Subscript N) 
	 * @brief Resize the Vector
	 * @param N New size
	 * @note this is not as efficient as it could be
         but to retain compatiblity with STL interface
         we cannot assume underlying implementation
         has a newsize() function.
	 * */
    Vector_Adaptor<BBVec>& resize(Subscript N) 
    { 
        if (N == size()) return *this;

        Vector_Adaptor<BBVec> tmp(N);
        Subscript n =  (N < size() ? N : size());  // min(N, size());
        Subscript i;

        for (i=0; i<n; i++)
            tmp[i] = v_[i];
            

        return (*this = tmp);

    }

	/** @fn  reference operator()(Subscript i)
	 * @brief Return elements on index parameter
	 * @param i Vector index 
	 * @return Vector element  \f$ A(i)\f$
	 * */
    reference operator()(Subscript i)
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(1<=i);
        assert(i<=dim());
#endif
        return vm1_[i]; 
    }

	/** @fn  const_reference operator()(Subscript i) const
	 * @brief Return elements on index parameter
	 * @param i Vector index 
	 * @return Vector element  \f$ A(i)\f$
	 * */
    const_reference operator()(Subscript i) const
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(1<=i);
        assert(i<=dim());
#endif
        return vm1_[i]; 
    }

	/** @fn  reference operator[](Subscript i)
	 * @brief Return elements on index parameter
	 * @param i Vector index 
	 * @return Vector element  \f$ A(i)\f$
	 * */
    reference operator[](Subscript i)
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(0<=i);
        assert(i<dim());
#endif
        return v_[i]; 
    }

	/** @fn  const_reference operator[](Subscript i) const
	 * @brief Return elements on index parameter
	 * @param i Vector index 
	 * @return Vector element  \f$ A(i)\f$
	 * */
    const_reference operator[](Subscript i) const
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(0<=i);
        assert(i<dim());
#endif
        return v_[i]; 
    }


#ifdef TNT_USE_REGIONS
    // "index-aware" features, all of these are 1-based offsets
	/** @typedef typedef Region1D<Vector_Adaptor<BBVec> > Region
	 * @brief index-aware" features, all of these are 1-based offsets
	 * */
    typedef Region1D<Vector_Adaptor<BBVec> > Region;
	
	/** @typedef const_Region1D< Vector_Adaptor<BBVec> > const_Region
	 * @brief index-aware" features, all of these are 1-based offsets
	 * */
    typedef const_Region1D< Vector_Adaptor<BBVec> > const_Region;
	
	/** @fn Region operator()(const Index1D &I)
	 * @brief Return region  of vector
	 * @param I Index element of vector
	 * @return Region of Vector*/
    Region operator()(const Index1D &I)
    {   return Region(*this, I); }
	
	/** @fn Region operator()(const Subscript i1, Subscript i2)
	 * @brief Return region of  of vector
	 * @param i1 Index start element of vector
	 * @param i2 Index  end element of vector
	 * @return Region of vector from \a i1 to \a i2
	 * */
    Region operator()(const Subscript i1, Subscript i2)
    {   return Region(*this, i1, i2); }

 	/** @fn const_Region operator()(const Index1D &I)
	 * @brief Return region  of vector
	 * @param I Index element of vector
	 * @return Region of Vector*/   
    const_Region operator()(const Index1D &I) const
    {   return const_Region(*this, I); }

	/** @fn const_Region operator()(const Subscript i1, Subscript i2)
	 * @brief Return region of  of vector
	 * @param i1 Index start element of vector
	 * @param i2 Index  end element of vector
	 * @return Region of vector from \a i1 to \a i2
	 * */
    const_Region operator()(const Subscript i1, Subscript i2) const
    {   return const_Region(*this, i1, i2); }
#endif
// TNT_USE_REGIONS


};

#include <iostream>

/** @fn template <class BBVec> std::ostream& operator<<(std::ostream &s, const Vector_Adaptor<BBVec> &A)
 * @brief Operator to stream out to standard out
 * @param s Standard Out
 * @param A Vector to be print in standard out
 * */
template <class BBVec>
std::ostream& operator<<(std::ostream &s, const Vector_Adaptor<BBVec> &A)
{
    Subscript M=A.size();

    s << M << endl;
    for (Subscript i=1; i<=M; i++)
            s << A(i) << endl;
    return s;
}

/** @fn template <class BBVec> std::istream& operator>>(std::istream &s, Vector_Adaptor<BBVec> &A)
 * @brief Operator to stream in from standard Input
 * @param s Standard Input
 * @param A Vector read in from standard in
 * */
template <class BBVec>
std::istream& operator>>(std::istream &s, Vector_Adaptor<BBVec> &A)
{
    Subscript N;
    
    s >> N;

    A.resize(N);

    for (Subscript i=1; i<=N; i++)
        s >> A(i);

    return s;
}

} // namespace TNT

#endif
