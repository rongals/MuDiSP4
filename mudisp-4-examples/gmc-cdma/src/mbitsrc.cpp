//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "mbitsrc.h"


void MBitSource::Setup() {

//////// initialization of dynamic data structures

  outmat = gsl_matrix_uint_alloc(M(),bpu());

  ran = gsl_rng_alloc( gsl_rng_default );
  gsl_rng_set(ran,MBitSource::InstID);


//////// rate declaration for ports



}


void MBitSource::Run() {


//////// production of data
  for (int i=0;i<M();i++)
    for (int j=0;j<bpu();j++)
      gsl_matrix_uint_set(outmat,i,j,gsl_rng_uniform_int(ran,2));

  mout1.DeliverDataObj(*outmat);

}


void MBitSource::Finish() {
  gsl_rng_free( ran );
  gsl_matrix_uint_free( outmat );
}
