//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "mcpmpcoeffs.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_matrix.h"
#include "gmccdma.h"
#include "gsl/gsl_randist.h"

extern void gsl_matrix_complex_show(gsl_matrix_complex *);

void MCPMPCoeffs::Setup() {

  //////// initialization of dynamic data structures

  gsl_rng_env_setup();
  cout << BlockName << ".gsl_rng_default_seed=" << gsl_rng_default_seed << endl;

  //
  //  ch matrix structure
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

  _M = M();
  ch = gsl_matrix_complex_alloc(_M*_M,N());
  ran = gsl_rng_alloc( gsl_rng_default );

  double pwr=0.0;
  for (int j=0; j<L(); j++) {
    pwr += exp(-j*2.0/PTau());
  }
  
  gain=sqrt(1.0/(pwr*(Ricean()+1.0)));
  gainrice=sqrt(Ricean()/(Ricean()+1.0));
  
  //////// rate declaration for ports


}


void MCPMPCoeffs::Run() {

  
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
  gsl_complex chcoeff;
  gsl_matrix_complex_set_all(ch,z);
  for (int i=0; i<_M; i++) {
    for (int ii=0;ii<_M;ii++) {
      for (int j=0; j<L(); j++) {
	double coeffstd=gain*exp(-j/PTau())/sqrt(2.0);
	if (j==0) { 
	  chcoeff = gsl_complex_rect( gsl_ran_gaussian(ran,coeffstd)+gainrice,
				      gsl_ran_gaussian(ran,coeffstd));
	} else { 
	  chcoeff = gsl_complex_rect( gsl_ran_gaussian(ran,coeffstd),
						  gsl_ran_gaussian(ran,coeffstd));
	} // if

	gsl_matrix_complex_set(ch,i*_M+ii,j,chcoeff);	

      } // j loop
    } // ii loop
  } // i loop
  
//   cout << "channel:" << endl;
//   gsl_matrix_complex_show(ch);
 
  //////// production of data
  mout1.DeliverDataObj( *ch );

}

