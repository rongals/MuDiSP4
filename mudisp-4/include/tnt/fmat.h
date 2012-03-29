// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// Fortran-compatible matrix: column oriented, 1-based (i,j) indexing

#ifndef FMAT_H
#define FMAT_H

#include "subscrpt.h"
#include "vec.h"
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <sstream>
#ifdef TNT_USE_REGIONS
#include "region2d.h"
#endif

// simple 1-based, column oriented Matrix class

namespace TNT
{

/** @class Fortran_Matrix  fmat.h <tnt/fmat.h>
 * @brief Simple 1-based, column oriented Matrix class
 * @tparam T template Matrix
 * */
template <class T>
class Fortran_Matrix 
{


  public:
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
    /** @var T* v_
     * @brief Define vector pointer to data.  */
    T* v_;                  // these are adjusted to simulate 1-offset
  /** @var Subscript m_
    * @brief global dimensions, columns number */
    Subscript m_;
    /** @var Subscript n_
    * @brief global dimensions, rows number
    * */    
    Subscript n_;
    
    /** @var T** col_
     * @brief Define columns pointer to data.  */
    T** col_;           // these are adjusted to simulate 1-offset

    // internal helper function to create the array
    // of row pointers
/*! @fn void initialize(Subscript M, Subscript N)
 * @brief Internal helper function to create the array of row pointers
 * @param M Number of Columns
 * @param N Number of Rows
 */
    void initialize(Subscript M, Subscript N)
    {
        // adjust col_[] pointers so that they are 1-offset:
        //   col_[j][i] is really col_[j-1][i-1];
        //
        // v_[] is the internal contiguous array, it is still 0-offset
        //
        v_ = new T[M*N];
        col_ = new T*[N];

        assert(v_  != NULL);
        assert(col_ != NULL);


        m_ = M;
        n_ = N;
        T* p = v_ - 1;              
        for (Subscript i=0; i<N; i++)
        {
            col_[i] = p;
            p += M ;
            
        }
        col_ --; 
    }
 
   /** @fn     void copy(const T*  v) 
    * @brief method for copied Matrix \a v in the pointer \a v_
    * @tparam v Matrix to be copied
    * */  
    void copy(const T*  v)
    {
        Subscript N = m_ * n_;
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
    
/** @fn  void set(const T& val) 
 * @brief Method for set the Matrix elements value 
 * @tparam val Data value to be copied in the entire matrix
 * */ 
    void set(const T& val)
    {
        Subscript N = m_ * n_;
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
     * \brief Erase the Matrix element and  relase allocated memory
     * */
    void destroy()
    {     
        /* do nothing, if no memory has been previously allocated */
        if (v_ == NULL) return ;

        /* if we are here, then matrix was previously allocated */
        delete [] (v_);     
        col_ ++;                // changed back to 0-offset
        delete [] (col_);
    }


  public:
/*! @fn  T* begin()
 * @brief Read the value of parameter v_
 * @return Value of parameter \a v_
 * */
    T* begin() { return v_; }
 /*! @fn  const T* begin() const 
 * @brief Read the value of parameter v_
 * @return Value of parameter \a v_
 * */   
    const T* begin() const { return v_;}

/** @var T* end()
 * @brief \f$  v\_ + m\_*n\_ \f$*/
    T* end() { return v_ + m_*n_; }
/** @var const T* end() const 
 * @brief  \f$  v\_  + m\_*n\_ \f$*/    
    const T* end() const { return v_ + m_*n_; }


    // constructors
/** @fn Fortran_Matrix() : v_(0), m_(0), n_(0), col_(0)  
 *  * @brief Costructor and inizialized the class Object
 */
    Fortran_Matrix() : v_(0), m_(0), n_(0), col_(0)  {};
    
/**  @overload Fortran_Matrix(const Fortran_Matrix<T> &A)
 * @tparam A Matrix to be copy to ney Object
 * */    
    Fortran_Matrix(const Fortran_Matrix<T> &A)
    {
        initialize(A.m_, A.n_);
        copy(A.v_);
    }

/*! @overload Fortran_Matrix(Subscript M, Subscript N, const T& value = T(0))
 * \param M number of Columns
 * \param N number of Rows
 * \param value Value to be inizialized (default set to 0)
 * */
    Fortran_Matrix(Subscript M, Subscript N, const T& value = T(0))
    {
        initialize(M,N);
        set(value);
    }

/*! @overload Fortran_Matrix(Subscript M, Subscript N, const T* v)
 * \param M number of column
 * \param N number of Row
 * \param v Value to be copyed in the new object
 * */
    Fortran_Matrix(Subscript M, Subscript N, const T* v)
    {
        initialize(M,N);
        copy(v);
    }

/*! @overload  Fortran_Matrix(Subscript M, Subscript N, char *s)
 * @param M number of column
 * @param N number of Row
 * @param s Char string to be copyed in the new object
 * */
    Fortran_Matrix(Subscript M, Subscript N, char *s)
    {
        initialize(M,N);
        std::istringstream ins(s);

        Subscript i, j;

        for (i=1; i<=M; i++)
            for (j=1; j<=N; j++)
                ins >> (*this)(i,j);
    }

    // destructor
  /*! @fn ~Fortran_Matrix()
   * \brief Desctructor
   * */    
    ~Fortran_Matrix()
    {
        destroy();
    }


    // assignments
    //
    /*! @fn Fortran_Matrix<T>& operator=(const Fortran_Matrix<T> &A)
     * @brief assignments operator
     * @tparam A Matrix to be assign
     * @return Object assigned or re-alloc
     * */    
    Fortran_Matrix<T>& operator=(const Fortran_Matrix<T> &A)
    {
        if (v_ == A.v_)
            return *this;

        if (m_ == A.m_  && n_ == A.n_)      // no need to re-alloc
            copy(A.v_);

        else
        {
            destroy();
            initialize(A.m_, A.n_);
            copy(A.v_);
        }

        return *this;
    }
        
     /*! @fn  Fortran_Matrix<T>& operator=(const T& scalar)
     * @brief assignments operator
     * @param scalar A Scalar value to be assign
     * @return Object assigned or re-alloc
     * */           
    Fortran_Matrix<T>& operator=(const T& scalar)
    { 
        set(scalar); 
        return *this;
    }

/*! @fn Subscript dim(Subscript d) const 
 * @brief Method for get number of rows or columns 
 * @param d row or column specification
 * @return if \f$d=1\f$ return number of rows else if \f$d=2\f$ return number of columns, 0 other else. 
 * */
    Subscript dim(Subscript d) const 
    {
#ifdef TNT_BOUNDS_CHECK
       assert( d >= 1);
        assert( d <= 2);
#endif
        return (d==1) ? m_ : ((d==2) ? n_ : 0); 
    }

/*! @fn Subscript num_rows() const { return m_; }
 * @return Number of rows
 */
    Subscript num_rows() const { return m_; }
 /*! @fn  Subscript num_cols() const { return n_; }
  * @return Number of columns
  * */    
    Subscript num_cols() const { return n_; }

    /*! @fn Fortran_Matrix<T>& newsize(Subscript M, Subscript N)
     * @brief Method for Reallocating Matrix
     * @param M numbers of M
     * @param N numbers row
     * @return reallocating object
     * */
    Fortran_Matrix<T>& newsize(Subscript M, Subscript N)
    {
        if (num_rows() == M && num_cols() == N)
            return *this;

        destroy();
        initialize(M,N);

        return *this;
    }



    // 1-based element access
    //
/*! @fn  inline reference operator()(Subscript i, Subscript j)
 * @param i Row index
 * @param j Colums Index
 * @return Reference Element index by \a i \a j
 * */    
    inline reference operator()(Subscript i, Subscript j)
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(1<=i);
        assert(i <= m_) ;
        assert(1<=j);
        assert(j <= n_);
#endif
        return col_[j][i]; 
    }

/*! @fn inline reference operator()(Subscript i, Subscript j) const
 * @param i Row index
 * @param j Colums Index
 * @return Reference Element index by \a i \a j
 * */  
    inline const_reference operator() (Subscript i, Subscript j) const
    {
#ifdef TNT_BOUNDS_CHECK
        assert(1<=i);
        assert(i <= m_) ;
        assert(1<=j);
        assert(j <= n_);
#endif
        return col_[j][i]; 
    }

/*! @fn friend std::istream & operator>><T>(std::istream &s, Fortran_Matrix<T> &A);
 * @brief Data in from shell
 * @param s input stream
 * @param A Matrix Object
 * @return Stream Object Data from shell 
 * */ 
    friend std::istream& operator>> <T>(std::istream &s, Fortran_Matrix<T> &A);

#ifdef TNT_USE_REGIONS
/*! @typedef  egion2D<Fortran_Matrix<T> > Region
 * @brief Create Templeate Object to Manipulate Matrix 
 * */ 
    typedef Region2D<Fortran_Matrix<T> > Region;
    
  /*! @typedef  typedef const_Region2D< Fortran_Matrix<T> > const_Region
  * Create Templeate Object to Manipulate Constant Matrix 
  * */   
    typedef const_Region2D< Fortran_Matrix<T> > const_Region;

    /*! @fn Region operator()(const Index1D &I, const Index1D &J)
     * @param I Matrix Index to start manipulate
     * @param J Matrix Index to start manipulate
     * */
    Region operator()(const Index1D &I, const Index1D &J)
    {
        return Region(*this, I,J);
    }
    
 /*! @fn const_Region operator()(const Index1D &I, const Index1D &J) const
  * @param I Matrix Index (r/c) to start manipulate
  * @param J Matrix Index (r/c) to end manipulate
  * @return Region of Matrix
  * */
    const_Region operator()(const Index1D &I, const Index1D &J) const
    {
        return const_Region(*this, I,J);
    }

#endif


};


/* ***************************  I/O  ********************************/

/*! 
 * @fn template <class T> std::ostream& operator<<(std::ostream &s, const Fortran_Matrix<T> &A)
* @brief Output  Operator
* @param A Input Fortran_Matrix
* @param s Output operator stream
* @tparam T Class vector
* @return Stream data to standard output
* */
template <class T>
std::ostream& operator<<(std::ostream &s, const Fortran_Matrix<T> &A)
{
    Subscript M=A.num_rows();
    Subscript N=A.num_cols();

    s << M << " " << N << "\n";

    for (Subscript i=1; i<=M; i++)
    {
        for (Subscript j=1; j<=N; j++)
        {
            s << A(i,j) << " ";
        }
        s << "\n";
    }


    return s;
}

/*! @fn template <class T> std::istream& operator>>(std::istream &s, Fortran_Matrix<T> &A)
 * @brief Input operator for class
 * @param s Standard Input
 * @param A Matrix to store data input
 * @tparam T template data type parameter for class
 * */
template <class T>
std::istream& operator>>(std::istream &s, Fortran_Matrix<T> &A)
{

    Subscript M, N;

    s >> M >> N;

    if ( !(M == A.m_ && N == A.n_) )
    {
        A.destroy();
        A.initialize(M,N);
    }


    for (Subscript i=1; i<=M; i++)
        for (Subscript j=1; j<=N; j++)
        {
            s >>  A(i,j);
        }


    return s;
}

// *******************[ basic matrix algorithms ]***************************

/*! @fn template <class T> Fortran_Matrix<T> operator+(const Fortran_Matrix<T> &A, const Fortran_Matrix<T> &B)
    * @brief Matrix sum operator
    * @tparam A First Matrix
    * @tparam B Second Matrix
    * @return Matrix \f$A+B\f$
    * */
    
template <class T>
Fortran_Matrix<T> operator+(const Fortran_Matrix<T> &A, 
    const Fortran_Matrix<T> &B)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(M==B.num_rows());
    assert(N==B.num_cols());

    Fortran_Matrix<T> tmp(M,N);
    Subscript i,j;

    for (i=1; i<=M; i++)
        for (j=1; j<=N; j++)
            tmp(i,j) = A(i,j) + B(i,j);

    return tmp;
}

/*! @fn template <class T> Fortran_Matrix<T> operator-(const Fortran_Matrix<T> &A, const Fortran_Matrix<T> &B)
    * @brief Matrix sub operator
    * @tparam A First Matrix
    * @tparam B Second Matrix
    * @return Matrix \f$ A-B \f$
    * */
template <class T>
Fortran_Matrix<T> operator-(const Fortran_Matrix<T> &A, 
    const Fortran_Matrix<T> &B)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(M==B.num_rows());
    assert(N==B.num_cols());

    Fortran_Matrix<T> tmp(M,N);
    Subscript i,j;

    for (i=1; i<=M; i++)
        for (j=1; j<=N; j++)
            tmp(i,j) = A(i,j) - B(i,j);

    return tmp;
}


/*! @fn template <class T> Fortran_Matrix<T> mult_element(const Fortran_Matrix<T> &A, const Fortran_Matrix<T> &B)
    * @brief Element-to-Element Moltiplication
    * @tparam A First Matrix
    * @tparam B Second Matrix
    * @return Matrix \f$A*.B\f$
    * */
// element-wise multiplication  (use matmult() below for matrix
// multiplication in the linear algebra sense.)
//
//
template <class T>
Fortran_Matrix<T> mult_element(const Fortran_Matrix<T> &A, 
    const Fortran_Matrix<T> &B)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(M==B.num_rows());
    assert(N==B.num_cols());

    Fortran_Matrix<T> tmp(M,N);
    Subscript i,j;

    for (i=1; i<=M; i++)
        for (j=1; j<=N; j++)
            tmp(i,j) = A(i,j) * B(i,j);

    return tmp;
}

/*! @fn template <class T> Fortran_Matrix<T> transpose(const Fortran_Matrix<T> &A)
* @brief Matrix Transpose
* @tparam A Input Matrix
* @return Transpose Matrix
* */
template <class T>
Fortran_Matrix<T> transpose(const Fortran_Matrix<T> &A)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    Fortran_Matrix<T> S(N,M);
    Subscript i, j;

    for (i=1; i<=M; i++)
        for (j=1; j<=N; j++)
            S(j,i) = A(i,j);

    return S;
}

/*! @fn  template <class T> inline Fortran_Matrix<T> matmult(const Fortran_Matrix<T>  &A, const Fortran_Matrix<T> &B)
* @brief Vectorial Cross Produc   
* @param A First member of product
* @param B Second member of product
* @tparam T Template element type
* @return \f[ \mathbf  A \times  \mathbf B \f]
* */    
template <class T>
inline Fortran_Matrix<T> matmult(const Fortran_Matrix<T>  &A, 
    const Fortran_Matrix<T> &B)
{

#ifdef TNT_BOUNDS_CHECK
    assert(A.num_cols() == B.num_rows());
#endif

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();
    Subscript K = B.num_cols();

    Fortran_Matrix<T> tmp(M,K);
    T sum;

    for (Subscript i=1; i<=M; i++)
    for (Subscript k=1; k<=K; k++)
    {
        sum = 0;
        for (Subscript j=1; j<=N; j++)
            sum = sum +  A(i,j) * B(j,k);

        tmp(i,k) = sum; 
    }

    return tmp;
}

/** @fn template <class T> inline Fortran_Matrix<T> operator*(const Fortran_Matrix<T> &A, const Fortran_Matrix<T> &B)
 * @brief Vectorial Cross Product Operator for Class
 * @param A First member of product
 * @param B Second member of product
 * @tparam T Template function
 * @return \f$ A \times B \f$ 
 * */
template <class T>
inline Fortran_Matrix<T> operator*(const Fortran_Matrix<T> &A, 
    const Fortran_Matrix<T> &B)
{
    return matmult(A,B);
}

/** @fn template <class T> inline int matmult(Fortran_Matrix<T>& C, const Fortran_Matrix<T>  &A, const Fortran_Matrix<T> &B)
 * @brief Cross Product function for class
 * @param A First member of Product
 * @param B Second member of Product
 * @param C Matrix to store operation result
 * @tparam T Template type 
 * @return \f$ \mathbf C = A \times B \f$ 
 * */
template <class T>
inline int matmult(Fortran_Matrix<T>& C, const Fortran_Matrix<T>  &A, 
    const Fortran_Matrix<T> &B)
{

    assert(A.num_cols() == B.num_rows());

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();
    Subscript K = B.num_cols();

    C.newsize(M,K);         // adjust shape of C, if necessary


    T sum; 

    const T* row_i;
    const T* col_k;

    for (Subscript i=1; i<=M; i++)
    {
        for (Subscript k=1; k<=K; k++)
        {
            row_i = &A(i,1);
            col_k = &B(1,k);
            sum = 0;
            for (Subscript j=1; j<=N; j++)
            {
                sum +=  *row_i * *col_k;
                row_i += M;
                col_k ++;
            }
        
            C(i,k) = sum; 
        }

    }

    return 0;
}

/** @fn template <class T> Vector<T> matmult(const Fortran_Matrix<T>  &A, const Vector<T> &x)
 * @brief Cross Product function for class
 * @param A First member of Product
 * @param x Second member of Product
 * @tparam T 
 * @return \f$ A \times x \f$ 
 * */
template <class T>
Vector<T> matmult(const Fortran_Matrix<T>  &A, const Vector<T> &x)
{

#ifdef TNT_BOUNDS_CHECK
    assert(A.num_cols() == x.dim());
#endif

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    Vector<T> tmp(M);
    T sum;

    for (Subscript i=1; i<=M; i++)
    {
        sum = 0;
        for (Subscript j=1; j<=N; j++)
            sum = sum +  A(i,j) * x(j);

        tmp(i) = sum; 
    }

    return tmp;
}

/** @fn template <class T> inline Vector<T> operator*(const Fortran_Matrix<T>  &A, const Vector<T> &x)
 * @brief Cross Product Operator for class
 * @param A First member of Product
 * @param x Second member of Product
 * @tparam T Template type 
 * @return \f$ A \times x \f$ 
 * */
 
template <class T>
inline Vector<T> operator*(const Fortran_Matrix<T>  &A, const Vector<T> &x)
{
    return matmult(A,x);
}

/** @fn template <class T> inline Fortran_Matrix<T> operator*(const Fortran_Matrix<T>  &A, const T &x)
 * @brief Algebrical Product Operator
 * @param A First member of Product
 * @param x Second member of Product
 * @tparam T Template type 
 * @return \f$ A \times x \f$ 
 * */
template <class T>
inline Fortran_Matrix<T> operator*(const Fortran_Matrix<T>  &A, const T &x)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    Subscript MN = M*N; 

    Fortran_Matrix<T> res(M,N);
    const T* a = A.begin();
    T* t = res.begin();
    T* tend = res.end();

    for (t=res.begin(); t < tend; t++, a++)
        *t = *a * x;

    return res;
} 

}  // namespace TNT
#endif
// FMAT_H
