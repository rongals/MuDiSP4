// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// Matrix Transpose Views

#ifndef TRANSV_H
#define TRANSV_H

#include <iostream>
#include <cassert>
#include "vec.h"

namespace TNT
{
/** @class Transpose_View transv.h <tnt/transv.h>
 * @brief Matrix Transpose Views
 * @tparam Array2D 2D Vector Class Object
 * @ingroup matrix */
template <class Array2D>
class Transpose_View
{
    protected:
		
		/** @var Array2D &  A_
		 * @brief Array Contenitor Variable
		 * */
        const Array2D &  A_;

    public:

	/** @typedef typename Array2D::element_type T
	 * @brief Define template class element
	 * */
        typedef typename Array2D::element_type T;
        
     /** @typedef         T   value_type
      * @brief Defines element value type for class
      * */   
        typedef         T   value_type;
           
     /** @typedef         T   element_type
      * @brief Defines Template type element for class
      * */   
        typedef         T   element_type;
           
     /** @typedef         T*  pointer
      * @brief Define pointer type for class
      * */   
        typedef         T*  pointer;
           
     /** @typedef         T*  iterator
      * @brief Define Iterator pointer for class
      * */   
        typedef         T*  iterator;
           
     /** @typedef         T&  reference
      * @brief Define reference to a pointer
      * */   
        typedef         T&  reference;
           
     /** @typedef const   T*  const_iterator
      * @brief Define constant iterator pointer
      * */   
        typedef const   T*  const_iterator;
           
     /** @typedef const   T&  const_reference
      * @brief Define reference to constant iterator pointer
      * */   
        typedef const   T&  const_reference;

	/** @fn const Array2D & array()  const 
	 * @brief Method for get array (or Matrix)
	 * @return Class or Array
	 * */
        const Array2D & array()  const { return A_; }
        
    /** @fn Subscript num_rows() const 
     * @brief Method for scan number of rows
     * @return Numbers of rows
     * */    
        Subscript num_rows() const { return A_.num_cols();}
    /** @fn Subscript num_cols() const 
     * @brief Method for scan number of columns
     * @return Numbers of columns 
     * */      
        Subscript num_cols() const { return A_.num_rows();}
        
  /** @fn Subscript lbound() const
  * @brief Method for read Matrix left Bound
  * @return Matrix lext Bound
  * */        
        Subscript lbound() const { return A_.lbound(); }

/** @fn Subscript dim(Subscript i) const 
 * @brief Method for read Matrix Dimension
 * @param i Subscrit limit 
 * @return Dimension of matrix in row or vector
 * */            
        Subscript dim(Subscript i) const
        {
#ifdef TNT_BOUNDS_CHECK
            assert( A_.lbound() <= i);
            assert( i<= A_.lbound()+1);
#endif
            if (i== A_.lbound())
                return num_rows();
            else
                return num_cols();
        }

/** @fn Transpose_View(const Transpose_View<Array2D> &A) : A_(A.A_)
 * @brief Class Constructor
 * @param A Matrix tu transpose
 * */ 
        Transpose_View(const Transpose_View<Array2D> &A) : A_(A.A_) {};
        
        
 /** @fn  Transpose_View(const Array2D &A) : A_(A)
 * @brief Class Constructor
 * @param A Matrix tu transpose
 * */ 
   Transpose_View(const Array2D &A) : A_(A) {};


 /** @fn inline const typename Array2D::element_type & operator()( Subscript i, Subscript j) const
  * @brief Return Element of Array or Matrix
  * @param i Row Index
  * @param j Column Index
  * @return \f$A(i,j)\f$
  * */
            
        inline const typename Array2D::element_type & operator()(
            Subscript i, Subscript j) const
        {
#ifdef TNT_BOUNDS_CHECK
        assert(lbound()<=i);
        assert(i<=A_.num_cols() + lbound() - 1);
        assert(lbound()<=j);
        assert(j<=A_.num_rows() + lbound() - 1);
#endif

            return A_(j,i);
        }


};

/** @fn template <class Matrix> Transpose_View<Matrix> Transpose_view(const Matrix &A)
 * @brief Method to get Transpose View of Matrix
 * @param A Matrix to transpose
 * @return Transpose Matrix
 * */
template <class Matrix>
Transpose_View<Matrix> Transpose_view(const Matrix &A)
{
    return Transpose_View<Matrix>(A);
}

/** @fn template <class Matrix, class T> Vector<T> matmult( const Transpose_View<Matrix> & A, const Vector<T> &B)
* @brief Cross Product function
* @param A Input Matrix
* @param B Input Vector
* @tparam Matrix Template Class Matrix
* @tparam T Templase class for result
* @return \f$A\times B \f$
* */
template <class Matrix, class T>
Vector<T> matmult(
    const Transpose_View<Matrix> & A, 
    const Vector<T> &B)
{
    Subscript  M = A.num_rows();
    Subscript  N = A.num_cols();

    assert(B.dim() == N);

    Vector<T> x(N);

    Subscript i, j;
    T tmp = 0;

    for (i=1; i<=M; i++)
    {
        tmp = 0;
        for (j=1; j<=N; j++)
            tmp += A(i,j) * B(j);
        x(i) = tmp;
    }

    return x;
}

/** @fn template <class Matrix, class T> inline Vector<T> operator*(const Transpose_View<Matrix> & A, const Vector<T> &B)
* @brief Cross Product Operator for Class
* @param A Input Matrix
* @param B Input Vector
* @tparam Matrix Template Class Matrix
* @tparam T Templase class for result
* @return \f$A\times B \f$
* */
template <class Matrix, class T>
inline Vector<T> operator*(const Transpose_View<Matrix> & A, const Vector<T> &B)
{
    return matmult(A,B);
}


/** @fn template <class Matrix> std::ostream& operator<<(std::ostream &s, const Transpose_View<Matrix> &A)
 * @brief Standard output Operator for class
 * @param s Standard output
 * @param A Matrix to stream Out
 * @tparam Matrix Object Class
 * @return Stream Matrix to Standard Out
 * */
template <class Matrix>
std::ostream& operator<<(std::ostream &s, const Transpose_View<Matrix> &A)
{
    Subscript M=A.num_rows();
    Subscript N=A.num_cols();

    Subscript start = A.lbound();
    Subscript Mend = M + A.lbound() - 1;
    Subscript Nend = N + A.lbound() - 1;

    s << M << "  " << N << endl;
    for (Subscript i=start; i<=Mend; i++)
    {
        for (Subscript j=start; j<=Nend; j++)
        {
            s << A(i,j) << " ";
        }
        s << endl;
    }


    return s;
}

} // namespace TNT

#endif
// TRANSV_H
