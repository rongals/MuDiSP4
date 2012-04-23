//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "cpmpchan.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_blas.h"

extern void gsl_matrix_complex_show(gsl_matrix_complex *);


void CPMPChan::Setup() {

//////// initialization of dynamic data structures

  user_chan = gsl_matrix_complex_calloc(N(),N());

//////// rate declaration for ports


}


void CPMPChan::Run() {

  /// fetch data objects
  gsl_vector_complex invec =  vin1.GetDataObj();
  gsl_matrix_complex cmat  =  min2.GetDataObj();

  //
  //
  gsl_vector_complex *tmp = gsl_vector_complex_alloc(N());
  
  //
  //
  // extract the user channel matrix
  //
  //
  for (int i=0; i<N(); i++) {
    gsl_complex h = gsl_matrix_complex_get(&cmat,Cd(),(N()-i)%N());
    for (int j=0; j<N(); j++) {
      gsl_matrix_complex_set(user_chan,j,(j+i) % N(),h);
    }
  }


//    cout << "User " << Cd() << " channel:" << endl;
//    gsl_matrix_complex_show(user_chan);

  //
  //
  // compute the output vector
  //
  //   
  gsl_blas_zgemv(CblasNoTrans, 
		 gsl_complex_rect(1.0,0), 
		 user_chan, 
		 &invec,
		 gsl_complex_rect(0,0),
		 tmp);

//   cout << "User " << Cd() << " trasmitted vector:" << endl;
//   gsl_vector_complex_fprintf(stdout,&invec,"%f");
  
//   cout << "User " << Cd() << " received vector:" << endl;
//   gsl_vector_complex_fprintf(stdout,tmp,"%f");


  //////// production of data
  vout1.DeliverDataObj( *tmp );

}

