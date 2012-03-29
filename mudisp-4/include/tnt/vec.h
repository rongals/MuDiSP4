// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// Basic TNT  numerical vector (0-based [i] AND 1-based (i) indexing )
//

#ifndef VEC_H
#define VEC_H

#include "subscrpt.h"
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <sstream>

namespace TNT
{

  template <typename T> class Vector;
  
  /*! @fn  template <typename T> std::istream & operator>>(std::istream &, Vector<T> &)
   * @brief Operatura di lettura delle informazione dello standard input
   * */
  template <typename T> std::istream & operator>>(std::istream &, Vector<T> &);

/*! @class Vector vec.h <tnt/vec.h>
 * @brief Class to Manipulate Vector
 * @tparam T undefined Type
 * */
template <class T>
class Vector 
{


  public:
   /*! @typedef Subscript   size_type;
    * @brief Index size_type*/
    typedef Subscript   size_type;
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
    Subscript lbound() const { return 1;}
 
  protected:         
    /*! @var  T* v_
    * @brief is the internal contiguous array, it is still 0-offset
    * */
    T* v_;                  
    /*! @var  T* vm1_
    * @brief pointer adjustment for optimzied 1-offset indexing 
    * */
    T* vm1_;        // pointer adjustment for optimzied 1-offset indexing
    /*! \var Subscript n_
     * @brief index of element 
     * */
    Subscript n_;

/*! @fn void initialize(Subscript N)
 * @brief internal helper function to create the array of row pointers
 * @param N Number of elements to be inizialized
 * @ingroup matrix */
    void initialize(Subscript N)
    {
        assert(v_ == NULL);
        v_ = new T[N];
        assert(v_  != NULL);
        vm1_ = v_-1;
        n_ = N;
    }
   
   /*!@fn void copy(const T*  v)
    * @brief Copy costructor
    * @tparam *v Pointer to element to be copy
    * */
    void copy(const T*  v)
    {
        Subscript N = n_;
        Subscript i;

#ifdef TNT_UNROLL_LOOPS
        Subscript Nmod4 = N & 3;
        Subscript N4 = N - Nmod4;

        for (i=0; i<N4; i+=4)
        {
            v_[i] = v[i];
            v_[i+1] = v[i+1];
            v_[i+2] = v[i+2];
            v_[i+3] = v[i+3];
        }

        for (i=N4; i< N; i++)
            v_[i] = v[i];
#else

        for (i=0; i< N; i++)
            v_[i] = v[i];
#endif      
    }

/*! @fn void set(const T& val)
 * @brief Set value of Object 
 * @param val Value to be set
 * */
    void set(const T& val)
    {
        Subscript N = n_;
        Subscript i;

#ifdef TNT_UNROLL_LOOPS
        Subscript Nmod4 = N & 3;
        Subscript N4 = N - Nmod4;

        for (i=0; i<N4; i+=4)
        {
            v_[i] = val;
            v_[i+1] = val;
            v_[i+2] = val;
            v_[i+3] = val; 
        }

        for (i=N4; i< N; i++)
            v_[i] = val;
#else

        for (i=0; i< N; i++)
            v_[i] = val;
        
#endif      
    }
    

/*! @fn void destroy()
 * @brief Desroy the object and Relase  the Memory
 * */
    void destroy()
    {     
        /* do nothing, if no memory has been previously allocated */
        if (v_ == NULL) return ;

        /* if we are here, then matrix was previously allocated */
        delete [] (v_);     

        v_ = NULL;
        vm1_ = NULL;
    }


  public:

    // access
/*! @fn iterator begin()
 * @return pointer of the start element 
 * */ 
    iterator begin() { return v_;}
/*! @fn iterator end()
 * @return pointer of the last element 
 * */     
    iterator end()   { return v_ + n_; }
/*! @overload iterator begin() const
 * @return pointer of the start element 
 * */ 
    const iterator begin() const { return v_;}
/*! @overload iterator end() const
 * @return pointer of the last element 
 * */      
    const iterator end() const  { return v_ + n_; }

    // destructor
/*! @fn ~Vector() 
 * @brief Destructor
 * */
    ~Vector() 
    {
        destroy();
    }

    // constructors
/*! @fn Vector() : v_(0), vm1_(0), n_(0)  
 * @brief Class Constructor and Iniz.*/
    Vector() : v_(0), vm1_(0), n_(0)  {};

/*! @overload Vector(const Vector<T> &A) : v_(0), vm1_(0), n_(0)
 * @brief Create a new Object width passed Vector 
 * @tparam A Object Element to be copied in the new Object
 * */
    Vector(const Vector<T> &A) : v_(0), vm1_(0), n_(0)
    {
        initialize(A.n_);
        copy(A.v_);
    }

/*! @overload Vector(Subscript N, const T& value = T(0)) :  v_(0), vm1_(0), n_(0)
 * @brief Create a new vector on \a N elements set to 0 (or a \a T Value)
 * @param value Value to inizialized the new vector (default as 0)
 * @param N Number of elements to allocate
 * */
    Vector(Subscript N, const T& value = T(0)) :  v_(0), vm1_(0), n_(0)
    {
        initialize(N);
        set(value);
    }

/*! @overload  Vector(Subscript N, const T* v) :  v_(0), vm1_(0), n_(0)
 * @brief Create a new vector of \a elements and inizializad this width value
 * of passed Vector V
 * @param v Vector width value to be passed to a new vector
 * @param N Number of elements to be passed
 * */
    Vector(Subscript N, const T* v) :  v_(0), vm1_(0), n_(0)
    {
        initialize(N);
        copy(v);
    }

/*! @overload  Vector(Subscript N, char *s) :  v_(0), vm1_(0), n_(0)
 * @brief Create a new vector by copy a string of char
 * @param N Number of elements to be passed
 * @param *s pointer to chat string
 * */
    Vector(Subscript N, char *s) :  v_(0), vm1_(0), n_(0)
    {
        initialize(N);
        std::istringstream ins(s);

        Subscript i;

        for (i=0; i<N; i++)
                ins >> v_[i];
    }


    // methods
    // 
  /*! @fn    Vector<T>& newsize(Subscript N)
   * @brief Resize Vector Number of elementes 
   * @param N Number of elements in the new Object
   * @note The resize vector will be empty, all data will be erase.
   * @return A re-size Vector Object
   * */
    Vector<T>& newsize(Subscript N)
    {
        if (n_ == N) return *this;

        destroy();
        initialize(N);

        return *this;
    }


    // assignments
    //
    /*! @fn   Vector<T>& operator=(const Vector<T> &A)
     * @brief Assignements operator
     * @tparam A Vectot to be assigned to Object
     * @return Vector Object
     * */
    Vector<T>& operator=(const Vector<T> &A)
    {
        if (v_ == A.v_)
            return *this;

        if (n_ == A.n_)         // no need to re-alloc
            copy(A.v_);

        else
        {
            destroy();
            initialize(A.n_);
            copy(A.v_);
        }

        return *this;
    }
    
/*! @fn Vector<T>& operator=(const T& scalar) 
 * @brief Operator to assign  scalar value \a scalar to Vector Elements 
 * @param scalar value to be stored in the Vector
 * @return Modified Vector*/        
    Vector<T>& operator=(const T& scalar)
    { 
        set(scalar);  
        return *this;
    }

/*! @fn Subscript dim() const
 * @brief Vector Dimension
 * @return Number of elements
 * */
    Subscript dim() const 
    {
        return  n_; 
    }


/*! @fn Subscript size() const
 * @brief Vector Dimension
 * @return Number of elements
 * */
    Subscript size() const 
    {
        return  n_; 
    }


/*! @fn inline reference operator()(Subscript i)
 * @brief Return element refered to index \a i
 * @param i Index reference
 * @return pointer to the element index by \a i*/
    inline reference operator()(Subscript i)
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(1<=i);
        assert(i <= n_) ;
#endif
        return vm1_[i]; 
    }

/*! @fn inline const_reference operator()(Subscript i) const
 * @brief Return element refered to index \a i
 * @param i Index reference
 * @return pointer to the element index by \a i*/
    inline const_reference operator() (Subscript i) const
    {
#ifdef TNT_BOUNDS_CHECK
        assert(1<=i);
        assert(i <= n_) ;
#endif
        return vm1_[i]; 
    }

/*! @fn inline reference operator[](Subscript i)
 * @brief Return element refered to index \a i
 * @param i Index reference
 * @return pointer to the element index by \a i*/
    inline reference operator[](Subscript i)
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(0<=i);
        assert(i < n_) ;
#endif
        return v_[i]; 
    }

/*! @fn inline const_reference operator[](Subscript i) const
 * @brief Return element refered to index \a i
 * @param i Index reference
 * @return pointer to the element index by \a i*/
    inline const_reference operator[](Subscript i) const
    {
#ifdef TNT_BOUNDS_CHECK
        assert(0<=i);
 
        assert(i < n_) ;
#endif
        return v_[i]; 
    }

/*! @fn friend std::istream & operator>> <T>(std::istream &s, Vector<T> &A);
 * @brief Operator to stream from standard input
 * @param &s standard input
 * @param &A Vector Object to store stadard input
 * */
    friend std::istream & operator>> <T>(std::istream &s, Vector<T> &A);

};


/* ***************************  I/O  ********************************/
/*! @fn template <class T> std::ostream& operator<<(std::ostream &s, const Vector<T> &A)
 * @brief Operator to stream data in the standard output
 * @tparam A Matrix to stream
 * @ s Standard Output
 * */
template <class T> std::ostream& operator<<(std::ostream &s, const Vector<T> &A)
{
    Subscript N=A.dim();

    s <<  N << std::endl;

    for (Subscript i=0; i<N; i++)
        s   << A[i] << " " << std::endl;
    s << std::endl;

    return s;
}

/*! @fn template <class T> std::istream & operator>>(std::istream &s, Vector<T> &A)
 * @brief Operator to stream dato from standard input
 * @param s standard input
 * @param A Vector Object to store data from standard output
 * @tparam T Template type for class
 * */
template <class T>
std::istream & operator>>(std::istream &s, Vector<T> &A)
{

    Subscript N;

    s >> N;

    if ( !(N == A.n_) )
    {
        A.destroy();
        A.initialize(N);
    }


    for (Subscript i=0; i<N; i++)
            s >>  A[i];


    return s;
}

// *******************[ basic matrix algorithms ]***************************
/*! @fn template <class T> Vector<T> operator+(const Vector<T> &A, const Vector<T> &B)
 * @brief Operator Vector addiction. 
 * @tparam A Firt member
 * @tparam B Second member
 * @return Vector operation \f$  A +  B \f$
 * */

template <class T>
Vector<T> operator+(const Vector<T> &A, 
    const Vector<T> &B)
{
    Subscript N = A.dim();

    assert(N==B.dim());

    Vector<T> tmp(N);
    Subscript i;

    for (i=0; i<N; i++)
            tmp[i] = A[i] + B[i];

    return tmp;
}
/*! @fn template <class T> Vector<T> operator-(const Vector<T> &A, const Vector<T> &B)
 * @brief Operator Vector subtraction. 
 * @tparam A Firt member.
 * @tparam B Second member 
 @return Vector operation \f$  A -  B \f$
 * */
template <class T>
Vector<T> operator-(const Vector<T> &A, 
    const Vector<T> &B)
{
    Subscript N = A.dim();

    assert(N==B.dim());

    Vector<T> tmp(N);
    Subscript i;

    for (i=0; i<N; i++)
            tmp[i] = A[i] - B[i];

    return tmp;
}
/*! @fn template <class T> Vector<T> operator*(const Vector<T> &A, const Vector<T> &B)
 * @brief Operator Vector Cross Moltiplication. 
 * @tparam A Firt member
 * @tparam B Second member 
 * @return Vector operation \f$  A \times B \f$
 * */
template <class T>
Vector<T> operator*(const Vector<T> &A, 
    const Vector<T> &B)
{
    Subscript N = A.dim();

    assert(N==B.dim());

    Vector<T> tmp(N);
    Subscript i;

    for (i=0; i<N; i++)
            tmp[i] = A[i] * B[i];

    return tmp;
}

/*! @fn template <class T> T dot_prod(const Vector<T> &A, const Vector<T> &B)
 * @brief Dot product from vector \a A and \a B
 * @tparam A Vector A
 * @tparam B Vector B
 * @return product \f$ A.B \f$
 * */
template <class T> T dot_prod(const Vector<T> &A, const Vector<T> &B)
{
    Subscript N = A.dim();
    assert(N == B.dim());

    Subscript i;
    T sum = 0;

    for (i=0; i<N; i++)
        sum += A[i] * B[i];

    return sum;
}

}   /* namespace TNT */

#endif
// VEC_H
