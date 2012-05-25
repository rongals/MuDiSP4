//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "bitsrc.h"


void BitSource::Setup() {

//////// initialization of dynamic data structures

  ran = gsl_rng_alloc( gsl_rng_default );
  gsl_rng_set(ran,BitSource::InstID);

//////// rate declaration for ports



}


void BitSource::Run() {


//////// production of data

  out1.DeliverDataObj( gsl_rng_uniform_int(ran,2) );

}


void BitSource::Finish() {
  gsl_rng_free( ran );
}
