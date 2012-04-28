/*
** gsllapck.c
**
**
**
*/

//extern "C" {

#include <stdlib.h>
#include "gsllapack.h"

//
// nota in precedenti versioni del compilatore era g2c.h
//
#include <f2c.h>
#include "gsl/gsl_complex_math.h"


extern "C" int cgelss_(integer *m, 
		   integer *n, 
		   integer *nrhs, 
		   complex *a, 
		   integer *lda, 
		   complex *b, 
		   integer *ldb, 
		   real *s, 
		   real *rcond, 
		   integer *rank, 
		   complex *work, 
		   integer *lwork, 
		   real *rwork, 
		   integer *info);


int gsl_lapack_solveSVD_complex(gsl_vector_complex *vecb, 
				gsl_matrix_complex *mata,
				gsl_vector_complex *vecx) {

  int J=vecb->size;
  int K=vecx->size; /* A is (J x K) */
  int rval;

  /*
  **
  ** solve || Ax-b ||^2 with SVD 
  ** 
  */
  integer m = J;
  integer n = K;
  integer nrhs=1;
  complex *a;
  integer lda=J;
  complex *b;
  integer ldb=J;
  real *s;
  real rcond=-1;
  integer rank;
  complex *work;
  integer lwork=3*J;
  real *rwork;
  integer info;

 
  //
  // Allocation of temporary arrays
  //
  a = (complex *)malloc(sizeof(complex)*m*n);
  b = (complex *)malloc(sizeof(complex)*m);
  s = (real *)malloc(sizeof(real)*m);
  work = (complex *)malloc(sizeof(complex)*lwork);
  rwork = (real *)malloc(sizeof(real)*5*K);

  //
  // Load A matrix
  //

#ifdef SHOW_MATRIX
    printf("\n\n");
#endif

  for (int i=0; i<m; i++) {
    gsl_complex cc=gsl_vector_complex_get(vecb,i);
    b[i].r = GSL_REAL(cc);
    b[i].i = GSL_IMAG(cc);

#ifdef SHOW_MATRIX
       printf("%10.3f %10.3f\t\t",b[i].r,b[i].i);
#endif

    for (int j=0; j<n; j++) {
      gsl_complex c=gsl_matrix_complex_get(mata,i,j);
      a[j*m+i].r = GSL_REAL(c);
      a[j*m+i].i = GSL_IMAG(c);

#ifdef SHOW_MATRIX
            printf("%10.3f %10.3f\t",a[j*m+i].r,a[j*m+i].i);
#endif

    }

#ifdef SHOW_MATRIX
        printf("\n");
#endif

  }


  //
  // call CGELSS
  //
  rval = cgelss_(&m, 
		 &n, 
		 &nrhs, 
		 a, 
		 &lda, 
		 b, 
		 &ldb, 
		 s, 
		 &rcond, 
		 &rank, 
		 work, 
		 &lwork, 
		 rwork, 
		 &info);

#ifdef SHOW_MATRIX
    printf("\nRank A = %d\nS=\n",rank);
  for (int i=0; i<m; i++) {
    printf("%10.3f\n",s[i]);
  }
#endif


  //
  // Load matrix
  //
  for (int i=0; i<K; i++) {
    gsl_complex c=gsl_complex_rect(b[i].r,b[i].i);
    gsl_vector_complex_set(vecx,i,c);
  }


  free(a);
  free(b);
  free(s);
  free(work);
  free(rwork);
  

  return(rank);

}

// int gsl_lapack_pseudoinv_complex(gsl_matrix_complex *mata,
// 				gsl_matrix_complex *matia) {

//   int J=mata->size1;
//   int K=mata->size2; /* A is (J x K) */
//   int rval;

//   /*
//   **
//   **
//   ** find Moore-Penrose generalized inverse of a
//   ** USING LAPACK
//   */
//   char jobu="A";
//   char jobvt="A";
//   integer m = J;
//   integer n = K;
//   complex *a;
//   integer lda=J;
//   real *s;
//   complex *u;
//   integer ldu;
//   complex *vt;
//   integer ldvt;
//   complex *work;
//   integer lwork;
//   real *rwork;
//   integer info;

 
//   //
//   // Allocation of temporary arrays
//   //
//   a = (complex *)malloc(sizeof(complex)*m*n);
//   s = (real *)malloc(sizeof(real)*m);
//   u = (complex *)malloc(sizeof(complex)*ldu);
//   vt = (complex *)malloc(sizeof(complex)*ldvt);
//   work = (complex *)malloc(sizeof(complex)*lwork);
//   rwork = (real *)malloc(sizeof(real)*5*K);

//   //
//   // Load A matrix
//   //
//   printf("\n\n");
//   for (int i=0; i<J; i++) {
//     gsl_complex cc=gsl_vector_complex_get(vecb,i);
//     b[i].r = GSL_REAL(cc);
//     b[i].i = GSL_REAL(cc);
//     printf("%10.3f %10.3f\t\t",b[i].r,b[i].i);
//     for (int j=0; j<K; j++) {
//       gsl_complex c=gsl_matrix_complex_get(mata,i,j);
//       a[j*K+i].r = GSL_REAL(c);
//       a[j*K+i].i = GSL_IMAG(c);
//       printf("%10.3f %10.3f\t",a[j*K+i].r,a[j*K+i].i);
//     }
//     printf("\n");
//   }


//   //
//   // call CGELSS
//   //
//   rval = cgelss_(&m, 
// 		 &n, 
// 		 &nrhs, 
// 		 a, 
// 		 &lda, 
// 		 b, 
// 		 &ldb, 
// 		 s, 
// 		 &rcond, 
// 		 &rank, 
// 		 work, 
// 		 &lwork, 
// 		 rwork, 
// 		 &info);

//     printf("\nRank A = %d\nS=\n",rank);
//   for (int i=0; i<K; i++) {
//     printf("%10.3f\n",s[i],s[i]);
//   }


//   //
//   // Load matrix
//   //
//   for (int i=0; i<K; i++) {
//     gsl_complex c=gsl_complex_rect(b[i].r,b[i].i);
//     gsl_vector_complex_set(vecx,i,c);
//   }


//   free(a);
//   free(s);
//   free(u);
//   free(vt)
//   free(work);
//   free(rwork);
  

//   return(rval);

// }



//} /* end extern "C" */


