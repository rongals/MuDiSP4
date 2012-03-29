// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


//  Templated compressed sparse column matrix (Fortran conventions).
//  uses 1-based offsets in storing row indices.
//  Used primarily to interface with Fortran sparse matrix libaries.
//  (CANNOT BE USED AS AN STL CONTAINER.)


#ifndef FCSCMAT_H
#define FCSCMAT_H

#include <iostream>
#include <cassert>
#include "tnt.h"
#include "vec.h"

using namespace std;

namespace TNT
{
/** @class Fortran_Sparse_Col_Matrix fcscmat.h <tnt/fgscmat.h>
 * @brief Templated compressed sparse column matrix (Fortran conventions).
 * Used primarily to interface with Fortran sparse matrix libaries.
 * @ingroup matrix 
 * */
template <class T>
class Fortran_Sparse_Col_Matrix 
{

   protected:
	/** @var Vector<T> val_ 
	 * @brief  data values (nz_ elements) 
	 * */
       Vector<T>           val_;       // data values (nz_ elements)
		/** @var Vector<Subscript>   rowind_
		 * @brief row_ind (nz_ elements) 
		 * */
       Vector<Subscript>   rowind_;    // row_ind (nz_ elements)
       /** @var Vector<Subscript>   colptr_
        * @brief col_ptr (n_+1 elements)
        * */
       Vector<Subscript>   colptr_;    // col_ptr (n_+1 elements)
		
		/** @var int nz_
		 * @brief number of nonzeros
		 * */
       int nz_;                   // number of nonzeros
       /** @var Subscript m_
        * @brief global dimensions, columns number
        * */
       Subscript m_;              // global dimensions
       /** @var Subscript n_
        * @brief global dimensions, rows number
        * */
       Subscript n_;
  
   public:

/** @fn   Fortran_Sparse_Col_Matrix(void)
 * @brief Class  constructor */
       Fortran_Sparse_Col_Matrix(void);
/** @overload   Fortran_Sparse_Col_Matrix(const Fortran_Sparse_Col_Matrix<T> &S)
 * @brief Class  constructor 
 * @param S Matrix copied to Class Conteiner*/       
       Fortran_Sparse_Col_Matrix(const Fortran_Sparse_Col_Matrix<T> &S)
        : val_(S.val_), rowind_(S.rowind_), colptr_(S.colptr_), nz_(S.nz_),
            m_(S.m_), n_(S.n_) {};
  /** @overload  Fortran_Sparse_Col_Matrix(Subscript M, Subscript N, Subscript nz, const T  *val,  const Subscript *r,  const Subscript *c) : val_(nz, val), rowind_(nz, r),  colptr_(N+1, c), nz_(nz), m_(M), n_(N)
 * @brief Class  constructor, Inizialized Matrix Numbe of rows and columns, index position and inizialization data value 
 * @param M Columns Index
 * @param N Rows index
 * @param nz Number of elements 
 * @param r Matrix Dimension, numer of rows
 * @param c Matrix dimension, number of columns
 * @param val Value to inizializate Matrix
 * 
 * */             
       Fortran_Sparse_Col_Matrix(Subscript M, Subscript N, 
            Subscript nz, const T  *val,  const Subscript *r, 
            const Subscript *c) : val_(nz, val), rowind_(nz, r), 
            colptr_(N+1, c), nz_(nz), m_(M), n_(N) {};

  /** @overload  Fortran_Sparse_Col_Matrix(Subscript M, Subscript N, Subscript nz, char *val,  char  *r,  char *c) : val_(nz, val), rowind_(nz, r), colptr_(N+1, c), nz_(nz), m_(M), n_(N) 
 * @brief Class  constructor, Inizialized Matrix Numbe of rows and columns, index position and inizialization ad string value
 * @param M Columns Index
 * @param N Rows index
 * @param nz Number of elements 
 * @param r Matrix Dimension, numer of rows
 * @param c Matrix dimension, number of columns
 * @param val String Value to inizializate Matrix
 * 
 * */   
       Fortran_Sparse_Col_Matrix(Subscript M, Subscript N, 
            Subscript nz, char *val,  char  *r, 
            char *c) : val_(nz, val), rowind_(nz, r), 
            colptr_(N+1, c), nz_(nz), m_(M), n_(N) {};

/**        Fortran_Sparse_Col_Matrix(Subscript M, Subscript N, Subscript nz, const T  *val, Subscript *r, Subscript *c) Fortran_Sparse_Col_Matrix(Subscript M, Subscript N, Subscript nz, const T  *val, Subscript *r, Subscript *c): val_(nz, val), rowind_(nz, r), colptr_(N+1, c), nz_(nz),  m_(M), n_(N) {};
    /**  @fn ~Fortran_Sparse_Col_Matrix()
     * @brief class destructor
     * */
      ~Fortran_Sparse_Col_Matrix() {};
        
       /**@fn  T &      val(Subscript i)
        * @brief Method for get calue of specific Index
        * @param i Index of value to be get
        * @return Value indexed */
       T &      val(Subscript i) { return val_(i); }
       
       /**@fn const T &      val(Subscript i) const
        * @brief Method for get calue of specific Index
        * @param i Index of value to be get
        * @return Value indexed */
       const T &      val(Subscript i) const { return val_(i); }

       /** @fn Subscript &   row_ind(Subscript i)
        * @brief Method for get rowind_ for \a i parameter
        * @param i Matrix Subscript parameter  
        * @return Pointer at rowind_(i) */   
       Subscript &   row_ind(Subscript i) { return rowind_(i); }
       
       /** @fn const Subscript &   row_ind(Subscript i) const 
        * @brief Method for get rowind_ for \a i parameter
        * @param i Matrix Subscript parameter  
        * @return Pointer at rowind_(i) */   
       const Subscript &   row_ind(Subscript i) const { return rowind_(i); }

       /** @fn Subscript  col_ptr(Subscript i) 
        * @brief Method for get colptr_ for \a i parameter
        * @param i Matrix Subscript parameter  
        * @return colptr_(i) */ 
        
       Subscript    col_ptr(Subscript i) { return colptr_(i);}
       /** @fn const Subscript    col_ptr(Subscript i) const
        * @brief Method for get colptr_ for \a i parameter
        * @param i Matrix Subscript parameter  
        * @return colptr_(i)*/ 
       const Subscript    col_ptr(Subscript i) const { return colptr_(i);}

 /*! @fn  Subscript num_cols() const { return m_; }
  * @return Number of columns
  * */    
       Subscript    num_cols() const { return m_;}
 /*! @fn  Subscript num_rows() const { return n_; }
  * @return Number of rows
  * */           
       Subscript    num_rows() const { return n_; }

 /** @fn Subscript dim(Subscript i) const 
 * @brief Method for read Matrix Dimension
 * @param i Subscrit limit 
 * @return Dimension of matrix in row or vector
 * */
       Subscript          dim(Subscript i) const 
       {
#ifdef TNT_BOUNDS_CHECK
            assert( 1 <= i );
            assert( i <= 2 );
#endif
            if (i==1) return m_;
            else if (i==2) return m_;
            else return 0;
        }

    /** @fn Subscript num_nonzeros() const
     * @brief Method to get vector elements
     * @return Vector elements in \a Subscript
     * */
       Subscript          num_nonzeros() const {return nz_;};
	/*! @fn Subscript lbound() const
	 * @brief Define inferior limit for Subscript */       
       Subscript          lbound() const {return 1;}


    /*! @fn  Fortran_Sparse_Col_Matrix& operator=(const  Fortran_Sparse_Col_Matrix &C)
     * @brief assignments operator
     * @tparam C Matrix to be assign
     * @return Object assigned or re-alloc
     * */
       Fortran_Sparse_Col_Matrix& operator=(const 
            Fortran_Sparse_Col_Matrix &C)
        {
            val_ = C.val_;
            rowind_ = C.rowind_;
            colptr_ = C.colptr_;
            nz_ = C.nz_;
            m_ = C.m_;
            n_ = C.n_;

            return *this;
        }
    /*! @fn Fortran_Sparse_Col_Matrix& newsize(Subscript M, Subscript N, Subscript nz)
     * @brief Method for Reallocating Matrix
     * @param M numbers of M
     * @param N numbers row
     * @param nz Numer of data elements  in the matriz
     * @return reallocating data object
     * */
       Fortran_Sparse_Col_Matrix& newsize(Subscript M, Subscript N, 
                Subscript nz)
        {
            val_.newsize(nz);
            rowind_.newsize(nz);
            colptr_.newsize(N+1);
            return *this;
        }
};

/** @fn template <class T> ostream& operator<<(ostream &s, const Fortran_Sparse_Col_Matrix<T> &A)
 * @brief Operator for stream data to standard output (shell)
 * @param s Standard Output  
 * @param A Matrix to be print in the standard out
 * @return 
*/
template <class T>
ostream& operator<<(ostream &s, const Fortran_Sparse_Col_Matrix<T> &A)
{
    Subscript M=A.num_rows();
    Subscript N=A.num_cols();

    s << M << " " << N << " " << A.num_nonzeros() <<  endl;


    for (Subscript k=1; k<=N; k++)
    {
        Subscript start = A.col_ptr(k);
        Subscript end = A.col_ptr(k+1);

        for (Subscript i= start; i<end; i++)
        {
            s << A.row_ind(i) << " " << k << " " << A.val(i) << endl;
        }
    }

    return s;
}



} // namespace TNT

#endif  
/* FCSCMAT_H */

