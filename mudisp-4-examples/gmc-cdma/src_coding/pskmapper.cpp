//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "pskmapper.h"
#include <iostream>

#define PI 3.1415926536


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



void PSKMapper::Setup() {

//////// initialization of dynamic data structures



  // number of symbols
  Ns=(1 << Nb());


  /// vector and matrix allocation 

  gray_encoding = gsl_vector_uint_alloc(Ns);

  symbol_arg = 2.0*double(M_PI/Ns);


  ///////// Gray Encoder SetUp

  for (unsigned int i=0; i<Ns; i++) {
    unsigned int tmp=0;
    for (unsigned int k=0; k<Nb(); k++) {
      unsigned int t=(1<<k), tt=2*t;
      tmp += t * (((t+i)/tt) % 2);
    }
    gsl_vector_uint_set(gray_encoding,i,tmp);
  }


//////// rate declaration for ports

}


void PSKMapper::Run() {

  unsigned int nbits,nsymbs,count;


  /// fetch data objects
  gsl_vector_uint_class input = vin1.GetDataObj();	

  // number of input bits
  nbits = input.vec->size;

  // number of output symbols
  nsymbs = nbits / Nb();

  gsl_vector_complex *tmp=gsl_vector_complex_alloc(nsymbs);

  count=0;

  for (int n=0; n<nsymbs; n++) {

    symbol_id=0;

    //////// I take Nb bits from input and map it in new_symbol

    for (int i=0;i<Nb();i++) {
      symbol_id = (symbol_id << 1);
      symbol_id += gsl_vector_uint_get(input.vec,count++);
    }



    new_symbol = gsl_complex_polar(1.0,
				   symbol_arg * 
				   double(gsl_vector_uint_get(gray_encoding, 
							      symbol_id)));

    gsl_vector_complex_set(tmp,n,new_symbol);
   }


  // show output
  //  gsl_vector_complex_show(tmp);

  // deliver data
  vout1.DeliverDataObj(gsl_vector_complex_class(tmp));

  gsl_vector_complex_free(tmp);
 
}



