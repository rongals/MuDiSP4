//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "cpmpcoeffs.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_matrix.h"
#include "gmccdma.h"
#include "gsl/gsl_randist.h"

extern void gsl_matrix_complex_show(gsl_matrix_complex *);

void CPMPCoeffs::Setup() {

  //////// initialization of dynamic data structures

  gsl_rng_env_setup();
  cout << BlockName << ".gsl_rng_default_seed=" << gsl_rng_default_seed << endl;

  ch = gsl_matrix_complex_alloc(USERS,N());
  ran = gsl_rng_alloc( gsl_rng_default );

  double pwr=0.0;
  for (int j=0; j<L(); j++) {
    pwr += exp(-j*2.0/PTau());
  }
  
  gain=sqrt(1.0/(pwr*(Ricean()+1.0)));
  gainrice=sqrt(Ricean()/(Ricean()+1.0));
  
  //////// rate declaration for ports


}


void CPMPCoeffs::Run() {

  
  gsl_complex z = gsl_complex_rect(0,0);
  gsl_complex o = gsl_complex_rect(1,0);

  //
  //
  // produce the idiot channel matrix
  //
  //
//   for (int i=0; i<USERS; i++) {
//     for (int j=0; j<N(); j++) {
//       if (j==0)
// 	gsl_matrix_complex_set(ch,i,j,o);
//       else if (j==1)
// 	gsl_matrix_complex_set(ch,i,j,gsl_complex_polar(0.0,M_PI/3.0));
//       else
// 	gsl_matrix_complex_set(ch,i,j,z);

//     }
//   }



//
// Exponentially decaying power profile
//
//
  gsl_matrix_complex_set_all(ch,z);
  for (int i=0; i<USERS; i++) {
    for (int j=0; j<L(); j++) {
      double coeffstd=gain*exp(-j/PTau())/sqrt(2.0);
      if (j==0) { 
	gsl_complex chcoeff = gsl_complex_rect( gsl_ran_gaussian(ran,coeffstd)+gainrice,
						gsl_ran_gaussian(ran,coeffstd));
// 	gsl_complex chcoeffabs = gsl_complex_rect(gsl_complex_abs( chcoeff ),
// 						  0.0);
	gsl_matrix_complex_set(ch,i,j,chcoeff);	
      } else { 
	gsl_complex chcoeff = gsl_complex_rect( gsl_ran_gaussian(ran,coeffstd),
						gsl_ran_gaussian(ran,coeffstd));
// 	gsl_complex chcoeffabs = gsl_complex_rect(gsl_complex_abs( chcoeff ),
// 						  0.0);
	gsl_matrix_complex_set(ch,i,j,chcoeff);	
      }
    }
  }
  
//   cout << "channel:" << endl;
//   gsl_matrix_complex_show(ch);
 
  //////// production of data
  mout1.DeliverDataObj( *ch );

}

