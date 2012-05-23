//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "mcpmpchan.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_blas.h"

extern void gsl_matrix_complex_show(gsl_matrix_complex *);


void MCPMPChan::Setup() {

//////// initialization of dynamic data structures

  outmat = gsl_matrix_complex_calloc(N(),M());
  user_chan = gsl_matrix_complex_calloc(N(),N());

//////// rate declaration for ports


}


void MCPMPChan::Run() {

  /// fetch data objects
  gsl_matrix_complex inmat =  min1.GetDataObj();
  gsl_matrix_complex cmat  =  min2.GetDataObj();

  // inmat : input signal matrix x(n) (NxM)
  //                              i
  // complex sample at time n from Tx number i

  // cmat : channel coeffs matrix h(n) (M**2xN)
  //                               ij 
  // cmat matrix structure
  //
  //   +-                 -+
  //   | h(0) . . . . h(n) |
  //   |  11           11  |
  //   |                   |
  //   | h(0) . . . . h(n) |
  //   |  12           12  |
  //   |                   |
  //   | h(0) . . . . h(n) |
  //   |  21           21  |
  //   |                   |
  //   | h(0) . . . . h(n) |
  //   |  22           22  |
  //   +-                 -+
  // 
  //   where h(n) represents the channel impulse response
  //          ij
  //
  //   at time n, from tx i to rx j
  //   the matrix has MxM rows and N comumns.
  //   The (i,j) channel is locater at row (i-1)*M+(j-1)
  //   with i,j in the range [1,M] and rows counting from 0
  //
  //
  gsl_vector_complex *tmp = gsl_vector_complex_alloc(N());
  gsl_matrix_complex_set_zero(outmat);

  for (int rx=0;rx<M();rx++) { //loop through Rx

    //
    // csubmat creates a view on cmat extracting the MxN submatrix for Rx number u
    //
    gsl_matrix_complex_const_view csubmat = gsl_matrix_complex_const_submatrix(&cmat,rx*M(),0,M(),N());

    //
    // cut a slice of outmat
    //
    gsl_vector_complex_view outvec = gsl_matrix_complex_column(outmat,rx);

    for (int tx=0;tx<M();tx++) { // loop through Tx

      //
      // input signal from tx
      //
      gsl_vector_complex_view x = gsl_matrix_complex_column(&inmat,tx);

      //
      //
      // extract the current tx-rx channel matrix
      //
      //
      for (int i=0; i<N(); i++) {
	gsl_complex h = gsl_matrix_complex_get(&csubmat.matrix,tx,(N()-i)%N());
	for (int j=0; j<N(); j++) {
	  gsl_matrix_complex_set(user_chan,j,(j+i) % N(),h);
	}
      }


      // cout << "Channel (" << tx << "-" << rx << "):" << endl;
      // gsl_matrix_complex_show(user_chan);
      
      //
      // compute the signal rx = H tx 
      //   
      gsl_blas_zgemv(CblasNoTrans, 
		     gsl_complex_rect(1.0,0), 
		     user_chan, 
		     &x.vector,
		     gsl_complex_rect(0,0),
		     tmp);

      //
      // sum for each tx
      //
      gsl_vector_complex_add(&outvec.vector,tmp);
      
    } // tx loop 
    
  } // rx loop
  
  cout << "received signals matrix (" << N() << "x" << M() << ")" << endl;
  gsl_matrix_complex_show(outmat);
  

  //////// production of data
  mout1.DeliverDataObj( *outmat );
  
}

