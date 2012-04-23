//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "blocksrc.h"


void BlockSource::Setup() {

//////// initialization of dynamic data structures

  ran = gsl_rng_alloc( gsl_rng_default );
  gsl_rng_set(ran,BlockSource::InstID);

//////// rate declaration for ports



}


void BlockSource::Run() {

  gsl_vector_uint *vout = gsl_vector_uint_alloc(N());

  for (int i=0;i<N();i++)
    gsl_vector_uint_set(vout,i,gsl_rng_uniform_int(ran,2));
    

//////// production of data

  vout1.DeliverDataObj( gsl_vector_uint_class(vout) );

  gsl_vector_uint_free(vout);

}

