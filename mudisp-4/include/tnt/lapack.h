// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// Header file for Fortran Lapack

#ifndef LAPACK_H
#define LAPACK_H

// This file incomplete and included here to only demonstrate the
// basic framework for linking with the Fortran Lapack routines.

#include "fortran.h"
#include "vec.h"
#include "fmat.h"
#include "cmat.h"
#include <complex>


using namespace std;
using namespace TNT;

/** @typedef Fortran_double* fda_
 * @brief (in/out) double precision array
 * */
typedef Fortran_double* fda_;        // (in/out) double precision array

/** @typedef const Fortran_double* cfda_ 
 *  @brief (in) double precsion array */
typedef const Fortran_double* cfda_; // (in) double precsion array

/** @typedef Fortran_double* fd_        
 * @brief (in/out)  single double precision 
 * */
typedef Fortran_double* fd_;        // (in/out)  single double precision

/** @typedef const Fortran_double* cfd_
 * @brief (in) single double precision 
 * */
typedef const Fortran_double* cfd_; // (in) single double precision

/** @typedef Fortran_float* ffa_        
 * @brief (in/out) float precision array
 * */
typedef Fortran_float* ffa_;        // (in/out) float precision array

/** @typedef const Fortran_float* cffa_
 * @brief (in) float precsion array
 * */
typedef const Fortran_float* cffa_; // (in) float precsion array

/** @typedef Fortran_float* ff_        
 * @brief (in/out)  single float precision 
 * */ 
typedef Fortran_float* ff_;         // (in/out)  single float precision

/** @typedef const Fortran_float* cff_
 *  @brief (in) single float precision */
typedef const Fortran_float* cff_;  // (in) single float precision

/** @typedef Fortran_integer* fia_        
 * @brief (in/out)  single integer array */

typedef Fortran_integer* fia_;          // (in/out)  single integer array

/** @typedef const Fortran_integer* cfia_ 
 * @brief (in) single integer array */
typedef const Fortran_integer* cfia_;   // (in) single integer array

/** @typedef Fortran_integer* fi_
 * @brief (in/out)  single integer
 * */
typedef Fortran_integer* fi_;           // (in/out)  single integer

/** @typedef const Fortran_integer* cfi_
 * @brief (in) single integer */
typedef const Fortran_integer* cfi_;    // (in) single integer

/** typedef Fortran_complex16* fc16a_
 * @brief (in/out) complex array 
 * */
typedef Fortran_complex16* fc16a_;       // (in/out) complex array 

/** @typedef const Fortran_complex16* cfc16a_ 
 * @brief (in) complex array */
typedef const Fortran_complex16* cfc16a_; // (in) complex array


/** typedef char * fch_
 * @brief (in/out) single character */
typedef char * fch_;                // (in/out) single character

/** @typedef char * cfch_
 * @brief (in) single character */
typedef char * cfch_;               // (in) single character

#define F77_DGESV   dgesv_
#define F77_DGELS   dgels_
#define F77_DSYEV   dsyev_
#define F77_DGEEV   dgeev_
#define F77_ZGETRF  zgetrf_
#define F77_ZGETRI  zgetri_

/** @extern "C" 
 * @brief  Pure C function 
 * */
extern "C"
{

  // linear equations (general) using LU factorizaiton
  //
  
  
  void F77_DGESV(cfi_ N, cfi_ nrhs, fda_ A, cfi_ lda,
		 fia_ ipiv, fda_ b, cfi_ ldb, fi_ info);

  // solve linear least squares using QR or LU factorization
  //
  void F77_DGELS(cfch_ trans, cfi_ M, 
		 cfi_ N, cfi_ nrhs, fda_ A, cfi_ lda, fda_ B, cfi_ ldb, fda_ work, 
		 cfi_ lwork, fi_ info);

  // solve symmetric eigenvalues
  //
  void F77_DSYEV( cfch_ jobz, cfch_ uplo, cfi_ N, fda_  A, cfi_ lda, 
		  fda_ W, fda_ work, cfi_ lwork, fi_ info);

  // solve unsymmetric eigenvalues
  //
  void F77_DGEEV(cfch_ jobvl, cfch_ jobvr, cfi_ N, fda_ A, cfi_ lda,
		 fda_ wr, fda_ wi, fda_ vl, cfi_ ldvl, fda_ vr, 
		 cfi_ ldvr, fda_ work, cfi_ lwork, fi_ info);


  // LU decomposition of MxN matrix
  //
  void F77_ZGETRF(cfi_ M, cfi_ N, fc16a_ A, cfi_ lda, fia_ ipiv,
		  fi_ info);

  // Inversion of MxM matrix using LU decomposition ZGETRF
  //
  void F77_ZGETRI(cfi_ M, fc16a_ A, cfi_ lda, fia_ ipiv, fc16a_ work,
		  cfi_ lwork, fi_ info);
  
}



/** @fn Vector<double> Lapack_LU_linear_solve(const Fortran_Matrix<double> &A, const Vector<double> &b)
 * @brief Solve linear equations using LU decomposition width Fortran Code
 * @tparam A linear coefficient matrix 
 * @tparam b knows term vector
 * @return System solution for decomposition LU Vector
 * @ingroup matrix
*/
// solve linear equations using LU factorization

Vector<double> Lapack_LU_linear_solve(const Fortran_Matrix<double> &A,
				      const Vector<double> &b)
{
  const Fortran_integer one=1;
  Subscript M=A.num_rows();
  Subscript N=A.num_cols();

  Fortran_Matrix<double> Tmp(A);
  Vector<double> x(b);
  Vector<Fortran_integer> index(M);
  Fortran_integer info = 0;

  F77_DGESV(&N, &one, &Tmp(1,1), &M, &index(1), &x(1), &M, &info);    

  if (info != 0) return Vector<double>(0);
  else
    return x;
}

// solve linear least squares problem using QR factorization
//

/** @fn Vector<double> Lapack_LLS_QR_linear_solve(const Fortran_Matrix<double> &A, const Vector<double> &b)
 * @brief Solve linear least squares problem using QR factorization
 * @tparam A coefficients Matrix
 * @tparam b know terms Vector
 * @return System equation solve vector 
 * */
Vector<double> Lapack_LLS_QR_linear_solve(const Fortran_Matrix<double> &A,
					  const Vector<double> &b)
{
  const Fortran_integer one=1;
  Subscript M=A.num_rows();
  Subscript N=A.num_cols();

  Fortran_Matrix<double> Tmp(A);
  Vector<double> x(b);
  Fortran_integer info = 0;

  char transp = 'N';
  Fortran_integer lwork = 5 * (M+N);      // temporary work space
  Vector<double> work(lwork);

  F77_DGELS(&transp, &M, &N, &one, &Tmp(1,1), &M, &x(1), &M,  &work(1),
	    &lwork, &info); 

  if (info != 0) return Vector<double>(0);
  else
    return x;
}

// *********************** Eigenvalue problems *******************

// solve symmetric eigenvalue problem (eigenvalues only)
//
/**  @fn Vector<double> Upper_symmetric_eigenvalue_solve(const Fortran_Matrix<double> &A) 
 * @brief Solve symmetric eigenvalue problem (eigenvalues only)
 * @tparam A (input) linear coefficient matrix
 * @return eigevalue for input upper symmetric matrix
 * @ingroup matrix */
Vector<double> Upper_symmetric_eigenvalue_solve(const Fortran_Matrix<double> &A)
{
  char jobz = 'N';
  char uplo = 'U';
  Subscript N = A.num_rows();

  assert(N == A.num_cols());

  Vector<double> eigvals(N);
  Fortran_integer worksize = 3*N;
  Fortran_integer info = 0;
  Vector<double> work(worksize);
  Fortran_Matrix<double> Tmp = A;

  F77_DSYEV(&jobz, &uplo, &N, &Tmp(1,1), &N, eigvals.begin(), work.begin(),
	    &worksize, &info);

  if (info != 0) return Vector<double>();
  else
    return eigvals;
}


/** @fn int eigenvalue_solve(const Fortran_Matrix<double> &A,  Vector<double> &wr, Vector<double> &wi)
 *  solve unsymmetric eigenvalue problems 
 * @tparam A unsymmetric coefficient  matrix
 * @tparam wr 
 * @tparam wi
 * @return 0 if succesful or 1 otherwise
 * @ingroup matrix */
int eigenvalue_solve(const Fortran_Matrix<double> &A, 
		     Vector<double> &wr, Vector<double> &wi)
{
  char jobvl = 'N';
  char jobvr = 'N';

  Fortran_integer N = A.num_rows();


  assert(N == A.num_cols());
    
  if (N<1) return 1;

  Fortran_Matrix<double> vl(1,N);  /* should be NxN ? ****/ 
  Fortran_Matrix<double> vr(1,N);  
  Fortran_integer one = 1;

  Fortran_integer worksize = 5*N;
  Fortran_integer info = 0;
  Vector<double> work(worksize, 0.0);
  Fortran_Matrix<double> Tmp = A;

  wr.newsize(N);
  wi.newsize(N);

//  void F77_DGEEV(cfch_ jobvl, cfch_ jobvr, cfi_ N, fda_ A, cfi_ lda,
//      fda_ wr, fda_ wi, fda_ vl, cfi_ ldvl, fda_ vr, 
//      cfi_ ldvr, fda_ work, cfi_ lwork, fi_ info);

  F77_DGEEV(&jobvl, &jobvr, &N, &Tmp(1,1), &N, &(wr(1)),
	    &(wi(1)), &(vl(1,1)), &one, &(vr(1,1)), &one,
	    &(work(1)), &worksize, &info);

  return (info==0 ? 0: 1);
}


//
// Complex matrix inversion using LU decomposition
//
Matrix <complex<double> > Lapack_LU_CMatInv(const Matrix <complex<double> > &A) 
{

  Subscript N=A.num_rows();
  Subscript M=A.num_cols();

  if (N != M) {
    cerr << "Lapack_LU_CMatInv: no square matrix in input." << endl;
    exit(0);
  }

  Fortran_complex16 zero={0.0,0.0};
  Fortran_Matrix < Fortran_complex16 > Tmp(N,N,zero);
  Vector<Fortran_integer> ipiv(N);
  Fortran_integer info, lwork=2*N;
  Vector< Fortran_complex16 > work(lwork,zero);
    
  for (int i=0; i<N; i++)
    for (int j=0; j<N; j++) {
      Tmp(i+1,j+1).RE = A[i][j].real();
      Tmp(i+1,j+1).IM = A[i][j].imag();
    }

  // fill up the Tmp

  F77_ZGETRF(&N, &N, &Tmp(1,1), &N, &ipiv(1), &info);

  if (info != 0) {
    cerr << "Lapack_LU_CMatInv: Decomposition failed" << endl;
    exit(0);
  }


  F77_ZGETRI( &N, &Tmp(1,1), &N, &ipiv(1), &work(1), &lwork, &info);

  if (info != 0) {
    cerr << "Lapack_LU_CMatInv: Inversion failed" << endl;
    exit(0);
  }

  Matrix <complex<double> > x(N,N);

  // fill x
  for (int i=0; i<N; i++)
    for (int j=0; j<N; j++) {
      x[i][j] =complex<double> ( Tmp(i+1,j+1).RE, Tmp(i+1,j+1).IM) ;
    }

  return x;

}



#endif
// LAPACK_H




