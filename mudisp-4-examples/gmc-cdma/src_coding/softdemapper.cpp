//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "softdemapper.h"
#include <iostream>
#include "gsl/gsl_math.h"
#include "maxstar.h"

float (* SoftDemapper::max_star[] )(float, float) =
	{ 
		max_star0, max_star1, max_star2, max_star3, max_star4
	};

//
//
// Vector uint display
//
//
// void gsl_vector_uint_show(gsl_vector_uint *vec) {
//   unsigned int N=vec->size;
//   for (int i=0; i<N; i++) {
//       unsigned int el = gsl_vector_uint_get(vec,i);
//       cout << setw(3) 
// 	   << el;
//     }
//     cout << endl;
// }
// //
// void gsl_vector_show(gsl_vector *vec) {
//   unsigned int N=vec->size;
//   for (int i=0; i<N; i++) {
//       double el = gsl_vector_get(vec,i);
//       cout << setw(15) 
// 	   << el << endl;
//     }
//     cout << endl;
// }
//
//
// Matrix complex display
//
//
// void gsl_matrix_complex_show(gsl_matrix_complex *mat) {
//   unsigned int N=mat->size1, M=mat->size2;
//   for (int i=0; i<N; i++) {
//     for (int j=0; j<M; j++) {
//       gsl_complex el = gsl_matrix_complex_get(mat,i,j);
//       cout << "(" 
// 	   << GSL_REAL(el) 
// 	   << ","
// 	   << GSL_IMAG(el) 
// 	   << ") ";
//     }
//     cout << endl;
//   }
//   cout << endl;
// }
//
//
// Vector complex display
//
//
// void gsl_vector_complex_show(gsl_vector_complex *vec) {
//   unsigned int N=vec->size;
//   for (int i=0; i<N; i++) {
//       gsl_complex el = gsl_vector_complex_get(vec,i);
//       cout << "(" 
// 	   << GSL_REAL(el) 
// 	   << ","
// 	   << GSL_IMAG(el) 
// 	   << ") ";
//     }
//     cout << endl;
// }

//
//
// Matrix uint display
//
//
// void gsl_matrix_uint_show(gsl_matrix_uint *mat) {
//   unsigned int N=mat->size1, M=mat->size2;
//   for (int i=0; i<N; i++) {
//     for (int j=0; j<M; j++) {
//       unsigned int el = gsl_matrix_uint_get(mat,i,j);
//       cout << setw(3) 
// 	   << el;
//     }
//     cout << endl;
//   }
//   cout << endl;
// }



void SoftDemapper::Setup() {

//////// initialization of dynamic data structures



  // number of symbols
  Ns=(1 << Nb());


  /// vector and matrix allocation 

  gray_encoding = gsl_vector_uint_alloc(Ns);

  symbol_arg = 2.0*double(M_PI/Ns);


///////// Gray Decoder SetUp

  for (unsigned int i=0; i<Ns; i++) {
    unsigned int tmp=0;
    for (unsigned int k=0; k<Nb(); k++) {
      unsigned int t=(1<<k), tt=2*t;
      tmp += t * (((t+i)/tt) % 2);
    }
    gsl_vector_uint_set(gray_encoding,tmp,i);
  }

//////// rate declaration for ports

}


void SoftDemapper::Run() {

  unsigned int nbits,nsymbs,count;


  /// fetch data objects
  gsl_vector_complex_class input = vin1.GetDataObj();	

  // number of input symbs
  nsymbs = input.vec->size;

  //  cout << "received " << nsymbs << " elements in vector." << endl;

  // number of output symbols
  nbits = nsymbs * Nb();


  gsl_vector *llr=gsl_vector_alloc(nbits);
  double *den = (double *)calloc( Nb(), sizeof(double) );
  double *num = (double *)calloc( Nb(), sizeof(double) );


  // determine symb_likelyhood
  for (int i=0;i<nsymbs;i++) {  // cycle through received symbols

    for (int k=0;k<Nb();k++) {
      num[k] = GSL_NEGINF;			
      den[k] = GSL_NEGINF;			
    }

    // the received symbol
    gsl_complex recsym = gsl_vector_complex_get(input.vec,i);

    
//       cout << "received symbol = (" 
// 	   << GSL_REAL(recsym) 
// 	   << ","
// 	   << GSL_IMAG(recsym) 
// 	   << ") " << endl;


    for (int j=0;j<Ns;j++) { // cycle through postulated symbol

      // gray encoded symbol id
      unsigned int symbol_id = gsl_vector_uint_get(gray_encoding,j);
      
      // complex symbol
      gsl_complex refsym = gsl_complex_polar(1.0,symbol_arg * symbol_id );

      // likelyhood metric
      double metric = gsl_complex_abs( gsl_complex_sub(refsym,recsym) );
      metric = -EsNo()*metric*metric;

      //gsl_matrix_complex_set(symb_likelihoods,j,i);


      //
      // HERE is available a metric for symb i and refsymb j
      //      
      int mask = 1 << Nb() - 1;
      
      for (int k=0;k<Nb();k++) {	/* loop over bits */
	if (mask&j) {
	  /* this bit is a one */
	  num[k] = ( *max_star[LMAP()] )( num[k], metric );
	} else {
	  /* this bit is a zero */
	  den[k] = ( *max_star[LMAP()] )( den[k], metric );
	}
	mask = mask >> 1;
      } //bits
    } // alphabet

    for (int k=0;k<Nb();k++) {
      gsl_vector_set(llr,Nb()*i+k,num[k] - den[k]);
    }    
  } // symbols
  

  gsl_vector_class outv(llr);
  
  //  outv.show();

  // output bitwise LLR
  vout1.DeliverDataObj(outv);
  
  gsl_vector_free(llr);


  // free dynamic structures
  free(num);
  free(den);
  
}



