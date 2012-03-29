// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// C compatible matrix: row-oriented, 0-based [i][j] and 1-based (i,j) indexing
//

#ifndef CMAT_H
#define CMAT_H

#include "subscrpt.h"
#include "vec.h"
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <sstream>
#ifdef TNT_USE_REGIONS
#include "region2d.h"
#endif

namespace TNT
{
/*! @class Matrix cmat.h <tnt/cmat.h>
 * @tparam T 
 * @brief Algebrical Manipulation on Matrix
 * @ingroup matrix
 * */
template <typename T> class Matrix;

/*! @fn  template <typename T> std::istream & operator>>(std::istream &, Matrix<T> &);
 * @brief Operatore di assegnazione
 * @tparam Matrix
 * @tparam T 
 * @ingroup matrix
 * */
template <typename T> std::istream & operator>>(std::istream &, Matrix<T> &);


template <class T>
class Matrix 
{


  public:
 /*! @typedef  Subscript size_type 
  * @brief Define element position in Matrixr & Vector
  * */
    typedef Subscript   size_type;  
    /*! @typedef  T value_type 
    * @brief Define Template element Value in Matrixr & Vector
    * */ 
    typedef         T   value_type;
    /*! @typedef  T element_type 
    * @brief Define Template element Type in Matrixr & Vector
    * */ 
    typedef         T   element_type;
    /*! @typedef  T* pointer 
    * @brief Define pointer for class
    * */ 
    typedef         T*  pointer;
    /*! @typedef  T* iterator
    * @brief Define pointer iterator for class
    * */ 
    typedef         T*  iterator;
    
    /*! @typedef  T&  reference
    * @brief Define reference for class pointer
    * */ 
    typedef         T&  reference;
    
    /*! @typedef  T*  const_iterator 
    * @brief Define pointer iterator for const class
    * */ 
    typedef const   T*  const_iterator;
    
    /*! @typedef  T&  const_reference
    * @brief Define reference for class pointer
    * */ 
    typedef const   T&  const_reference;

/*! @fn Subscript lbound() const
 * @brief Define inferior limit for Subscript */
    Subscript lbound() const { return 1;}
 
  protected:
    /** @var Subscript m_
    * @brief global dimensions, columns number
    * */
    Subscript m_;
    
    /** @var Subscript n_
    * @brief global dimensions, rows number
    * */
    Subscript n_;
    
    /** @var Subscript mn_
    * @brief global dimensions, total size
    * */
    Subscript mn_;      // total size
    
    /** @var T* v_
     * @brief Define vector pointer to data */
    T* v_;        
    
    /** @var T** row_
     * @brief Define vector pointer to row data */          
    T** row_;   
    /*! \var T* vm1_ 
     * @brief   These point to the same data, but are 1-based */      
    T* vm1_ ;       // these point to the same data, but are 1-based 
    /*! \var T** rowm1_ 
     * @brief   These point to the same data, but are 1-based */    
    T** rowm1_;

    // internal helper function to create the array
    // of row pointers
/*! @fn void initialize(Subscript M, Subscript N)
 * @brief Internal helper function to create the array of row pointers
 * @param M Number of Columns
 * @param N Number of Rows
 */
    void initialize(Subscript M, Subscript N)
    {
        mn_ = M*N;
        m_ = M;
        n_ = N;

        v_ = new T[mn_]; 
        row_ = new T*[M];
        rowm1_ = new T*[M];

        assert(v_  != NULL);
        assert(row_  != NULL);
        assert(rowm1_ != NULL);

        T* p = v_;              
        vm1_ = v_ - 1;
        for (Subscript i=0; i<M; i++)
        {
            row_[i] = p;
            rowm1_[i] = p-1;
            p += N ;
            
        }

        rowm1_ -- ;     // compensate for 1-based offset
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
        if (v_ != NULL) delete [] (v_);     
        if (row_ != NULL) delete [] (row_);

        /* return rowm1_ back to original value */
        rowm1_ ++;
        if (rowm1_ != NULL ) delete [] (rowm1_);
    }


  public:
/*! @fn  operator T**()
 * @brief Read the value of parameter row_
 * @return Matrix element
 * */
    operator T**(){ return  row_; }

/*!
 * @fn  operator T**() const 
 * @brief Read the value of parameter row_
 * @return  Matrix element
 * */
    operator T**() const { return row_; }
    
/*! @fn Subscript size() const 
 * @brief Method for read Matrix Size
 * @return Matrix dimension
 */
    Subscript size() const { return mn_; }

    // constructors
/*! \fn   Matrix() : m_(0), n_(0), mn_(0), v_(0), row_(0), vm1_(0), rowm1_(0) {};
 * @brief Costructor and inizialized the class Object
 */
    Matrix() : m_(0), n_(0), mn_(0), v_(0), row_(0), vm1_(0), rowm1_(0) {};

/*! @overload  Matrix(const Matrix<T> &A)
 * \tparam A Matrix to be copy to ney Object
 * */
    Matrix(const Matrix<T> &A)
    {
        initialize(A.m_, A.n_);
        copy(A.v_);
    }
    
/*! @overload  Matrix(Subscript M, Subscript N, const T& value = T(0))
 * \param M number of Columns
 * \param N number of Rows
 * \param value Value to be inizialized (default set to 0)
 * */
    Matrix(Subscript M, Subscript N, const T& value = T(0))
    {
        initialize(M,N);
        set(value);
    }
/*! @overload  Matrix(Subscript M, Subscript N, const T* v)
 * \param M number of column
 * \param N number of Row
 * \param v Value to be copyed in the new object
 * */
    Matrix(Subscript M, Subscript N, const T* v)
    {
        initialize(M,N);
        copy(v);
    }
/*! @overload  Matrix(Subscript M, Subscript N, char *s)
 * @param M number of column
 * @param N number of Row
 * @param s Char string to be copyed in the new object
 * */
    Matrix(Subscript M, Subscript N, char *s)
    {
        initialize(M,N);
        std::istringstream ins(s);

        Subscript i, j;

        for (i=0; i<M; i++)
            for (j=0; j<N; j++)
                ins >> row_[i][j];
    }

    // destructor
    //
  /*! @fn ~Matrix()
   * \brief Desctructor
   * */
    ~Matrix()
    {
        destroy();
    }


    // reallocating
    //
    /*! @fn Matrix<T>& newsize(Subscript M, Subscript N)
     * @brief Method for Reallocating Matrix
     * @param M numbers of M
     * @param N numbers row
     * @return reallocating object
     * */
    Matrix<T>& newsize(Subscript M, Subscript N)
    {
        if (num_rows() == M && num_cols() == N)
            return *this;

        destroy();
        initialize(M,N);
        
        return *this;
    }




    // assignments
    //
    /*! @fn  Matrix<T>& operator=(const Matrix<T> &A)
     * @brief assignments operator
     * @tparam A Matrix to be assign
     * @return Object assigned or re-alloc
     * */
    Matrix<T>& operator=(const Matrix<T> &A)
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
 
     /*! @fn  Matrix<T>& operator=(const T& scalar)
     * @brief assignments operator
     * @param scalar A Scalar value to be assign
     * @return Object assigned or re-alloc
     * */       
    Matrix<T>& operator=(const T& scalar)
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
  * @return Number of cols
  * */
    Subscript num_cols() const { return n_; }



/*! @fn inline T* operator[](Subscript i)
 * @param i Row Vector Index
 * @return Row Vector \a i 
 * */
    inline T* operator[](Subscript i)
    {
#ifdef TNT_BOUNDS_CHECK
        assert(0<=i);
        assert(i < m_) ;
#endif
        return row_[i];
    }

/*! @fn inline T* operator[](Subscript i) const
 * @param i Row Vector Index
 * @return Row Vector \a i 
 * */
    inline const T* operator[](Subscript i) const
    {
#ifdef TNT_BOUNDS_CHECK
        assert(0<=i);
        assert(i < m_) ;
#endif
        return row_[i];
    }

/*! @fn inline reference operator()(Subscript i)
 * @param i Row Vector Index
 * @return reference Row Vector
 * */
    inline reference operator()(Subscript i)
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(1<=i);
        assert(i <= mn_) ;
#endif
        return vm1_[i]; 
    }

/*! @fn inline reference operator()(Subscript i) const
 * @param i Row Vector Index
 * @return reference Row Vector
 * */
    inline const_reference operator()(Subscript i) const
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(1<=i);
        assert(i <= mn_) ;
#endif
        return vm1_[i]; 
    }

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
        return  rowm1_[i][j]; 
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
        return rowm1_[i][j]; 
    }

/*! @fn friend std::istream & operator>><T>(std::istream &s, Matrix<T> &A);
 * @param s input stream
 * @param A Matrix Object
 * @return Stream Object Data from shell 
 * */ 
        
        friend std::istream & operator>><T>(std::istream &s, Matrix<T> &A);


#ifdef TNT_USE_REGIONS
/*! @typedef  Region
 * Create Templeate Object to Manipulate Matrix 
 * */  
    typedef Region2D<Matrix<T> > Region;
    
    /*! @fn Region operator()(const Index1D &I, const Index1D &J)
     * @param I Matrix Index to start manipulate
     * @param J Matrix Index to end manipulate
     * @return Region of Matrix
     * */

    Region operator()(const Index1D &I, const Index1D &J)
    {
        return Region(*this, I,J);
    }

 /*! @typedef  typedef const_Region2D< Matrix<T> > const_Region
  * Create Templeate Object to Manipulate Constant Matrix 
  * */
    typedef const_Region2D< Matrix<T> > const_Region;
    
 /*! @fn Region operator()(const Index1D &I, const Index1D &J)
  * @param I Matrix Index to start manipulate
  * @param J Matrix Index to end manipulate
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
 * @fn template <class T> std::ostream& operator<<(std::ostream &s, const Matrix<T> &A)
* @brief Output  Operator
* @param A Input Matrix
* @param s Output operator stream
* @tparam T Class vector
* @return Stream data to standard output
* */
template <class T>
std::ostream& operator<<(std::ostream &s, const Matrix<T> &A)
{
    Subscript M=A.num_rows();
    Subscript N=A.num_cols();

    s << M << " " << N << "\n";

    for (Subscript i=0; i<M; i++)
    {
        for (Subscript j=0; j<N; j++)
        {
            s << A[i][j] << " ";
        }
        s << "\n";
    }


    return s;
}

/*! @fn template <class T> std::istream& operator>>(std::istream &s, Matrix<T> &A)
 * @brief Input operator for class
 * @param s Standard Input
 * @param A Matrix to store data input
 * @tparam T template data type parameter for class
 * */
template <class T>
std::istream& operator>>(std::istream &s, Matrix<T> &A)
{

    Subscript M, N;

    s >> M >> N;

    if ( !(M == A.m_ && N == A.n_) )
    {
        A.destroy();
        A.initialize(M,N);
    }


    for (Subscript i=0; i<M; i++)
        for (Subscript j=0; j<N; j++)
        {
            s >>  A[i][j];
        }


    return s;
}

// *******************[ basic matrix algorithms ]***************************

/*! @fn template <class T> Matrix<T> operator+(const Matrix<T> &A, const Matrix<T> &B)
    * @brief Matrix sum operator
    * @tparam A First Matrix
    * @tparam B Second Matrix
    * @return Matrix \f$A+B\f$
    * */
template <class T>
Matrix<T> operator+(const Matrix<T> &A, 
    const Matrix<T> &B)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(M==B.num_rows());
    assert(N==B.num_cols());

    Matrix<T> tmp(M,N);
    Subscript i,j;

    for (i=0; i<M; i++)
        for (j=0; j<N; j++)
            tmp[i][j] = A[i][j] + B[i][j];

    return tmp;
}

/*! @fn template <class T> Matrix<T> operator-(const Matrix<T> &A, const Matrix<T> &B)
    * @brief Matrix sub operator
    * @tparam A First Matrix
    * @tparam B Second Matrix
    * @return Matrix \f$ A-B \f$
    * */
template <class T>
Matrix<T> operator-(const Matrix<T> &A, 
    const Matrix<T> &B)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(M==B.num_rows());
    assert(N==B.num_cols());

    Matrix<T> tmp(M,N);
    Subscript i,j;

    for (i=0; i<M; i++)
        for (j=0; j<N; j++)
            tmp[i][j] = A[i][j] - B[i][j];

    return tmp;
}

/*! @fn template <class T> Matrix<T> mult_element(const Matrix<T> &A, const Matrix<T> &B)
    * @brief Element-to-Element Moltiplication
    * @tparam A First Matrix
    * @tparam B Second Matrix
    * @return Matrix \f$A*.B\f$
    * */
template <class T>
Matrix<T> mult_element(const Matrix<T> &A, 
    const Matrix<T> &B)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(M==B.num_rows());
    assert(N==B.num_cols());

    Matrix<T> tmp(M,N);
    Subscript i,j;

    for (i=0; i<M; i++)
        for (j=0; j<N; j++)
            tmp[i][j] = A[i][j] * B[i][j];

    return tmp;
}

/*! @fn template <class T> Matrix<T> transpose(const Matrix<T> &A) 
* @brief Matrix Transpose
* @tparam A Input Matrix
* @return Transpose Matrix
* */
template <class T>
Matrix<T> transpose(const Matrix<T> &A)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    Matrix<T> S(N,M);
    Subscript i, j;

    for (i=0; i<M; i++)
        for (j=0; j<N; j++)
            S[j][i] = A[i][j];

    return S;
}


/*! @fn  template <class T> inline Matrix<T> matmult(const Matrix<T>  &A, const Matrix<T> &B)
* @brief Vectorial Cross Produc   
* @param A First member of product
* @param B Second member of product
* @tparam T Template element type
* @return \f[ \mathbf  A \times  B \f]
* */
template <class T>
inline Matrix<T> matmult(const Matrix<T>  &A, 
    const Matrix<T> &B)
{

#ifdef TNT_BOUNDS_CHECK
    assert(A.num_cols() == B.num_rows());
#endif

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();
    Subscript K = B.num_cols();

    Matrix<T> tmp(M,K);
    T sum;

    for (Subscript i=0; i<M; i++)
    for (Subscript k=0; k<K; k++)
    {
        sum = 0;
        for (Subscript j=0; j<N; j++)
            sum = sum +  A[i][j] * B[j][k];

        tmp[i][k] = sum; 
    }

    return tmp;
}

/** @fn template <class T> inline Matrix<T> operator*(const Matrix<T> &A, const Matrix<T> &B)
 * @brief Vectorial Cross Product Operator for Class
 * @param A First member of product
 * @param B Second member of product
 * @tparam T Template function
 * @return \f$ A \times B \f$ 
 * */
template <class T>
inline Matrix<T> operator*(const Matrix<T>  &A, 
    const Matrix<T> &B)
{
    return matmult(A,B);
}

/** @fn template <class T> inline int matmult(Matrix<T>& C, const Matrix<T>  &A, const Matrix<T> &B)
 * @brief Cross Product function for class
 * @param A First member of Product
 * @param B Second member of Product
 * @param C Matrix to store operation result
 * @tparam T Template type 
 * @return \f$ \mathbf C = A \times B \f$ 
 * */

template <class T>
inline int matmult(Matrix<T>& C, const Matrix<T>  &A, 
    const Matrix<T> &B)
{

    assert(A.num_cols() == B.num_rows());

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();
    Subscript K = B.num_cols();

    C.newsize(M,K);

    T sum;

    const T* row_i;
    const T* col_k;

    for (Subscript i=0; i<M; i++)
    for (Subscript k=0; k<K; k++)
    {
        row_i  = &(A[i][0]);
        col_k  = &(B[0][k]);
        sum = 0;
        for (Subscript j=0; j<N; j++)
        {
            sum  += *row_i * *col_k;
            row_i++;
            col_k += K;
        }
        C[i][k] = sum; 
    }

    return 0;
}

/** @fn template <class T> Vector<T> matmult(const Matrix<T>  &A, const Vector<T> &x)
 * @brief Cross Product function for class
 * @param A First member of Product
 * @param x Second member of Product
 * @tparam T 
 * @return \f$ A \times x \f$ 
 * */
template <class T>
Vector<T> matmult(const Matrix<T>  &A, const Vector<T> &x)
{

#ifdef TNT_BOUNDS_CHECK
    assert(A.num_cols() == x.dim());
#endif

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    Vector<T> tmp(M);
    T sum;

    for (Subscript i=0; i<M; i++)
    {
        sum = 0;
        const T* rowi = A[i];
        for (Subscript j=0; j<N; j++)
            sum = sum +  rowi[j] * x[j];

        tmp[i] = sum; 
    }

    return tmp;
}

/** @fn template <class T> inline Vector<T> operator*(const Matrix<T>  &A, const Vector<T> &x)
 * @brief Cross Product Operator for class
 * @param A First member of Product
 * @param x Second member of Product
 * @tparam T Template type 
 * @return \f$ A \times x \f$ 
 * */
 
template <class T>
inline Vector<T> operator*(const Matrix<T>  &A, const Vector<T> &x)
{
    return matmult(A,x);
}

} // namespace TNT

#endif
// CMAT_H
