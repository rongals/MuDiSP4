//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "awgn.h"
#include "gsl/gsl_randist.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"

void AWGN::Setup() {

//////// initialization of dynamic data structures
  gsl_rng_env_setup();
  noisestd = sqrt( noisevar() );
  ran = gsl_rng_alloc( gsl_rng_default );

  cout << BlockName << ".gsl_rng_default_seed=" << gsl_rng_default_seed << endl;

//////// rate declaration for ports


}


void AWGN::Run() {

  /// fetch data objects

  gsl_vector_complex tmp =  in[0].GetDataObj();

  for (int j = 1; j< Nin; j++) {
     gsl_vector_complex obj =  in[j].GetDataObj();
     for (int i=0; i<obj.size; i++) {
       gsl_complex sum= gsl_complex_add(gsl_vector_complex_get(&obj,i),
					gsl_vector_complex_get(&tmp,i));
       gsl_vector_complex_set(&tmp,i,sum);
     }
  }

  for (int i=0; i< tmp.size; i++) {
    gsl_complex noisesample = gsl_complex_rect( gsl_ran_gaussian(ran,noisestd),
						gsl_ran_gaussian(ran,noisestd));
    gsl_complex ctmp = gsl_complex_add(gsl_vector_complex_get(&tmp,i),noisesample);
    gsl_vector_complex_set(&tmp,i,ctmp);
  }
  
//////// production of data

  vout1.DeliverDataObj( tmp );

}

