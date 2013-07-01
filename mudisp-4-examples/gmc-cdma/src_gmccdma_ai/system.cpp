//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 

#include "system.h"
#include "propagation.h"
#include "gsl/gsl_sf_log.h"
#include <math.h>

System::~System(){
}

void System::Build() {

  ////////////////////////////////////////////////////////
  //////// Here there are the Connect instructions and  
  //////// any other stuff to be executed once before the 
  //////// parameters reading.
  ////////

  // uncomment to send instantaneous channel coefficients to allocator
  Connect(mcoeffs1,mcoeffs1.mout1,maialloc1,maialloc1.min1);

  // uncomment to send channel powers to allocator
  //Connect(mcoeffs1,mcoeffs1.mout2,maialloc1,maialloc1.min1);

  Connect(mcoeffs1,mcoeffs1.mout1,mchannel1,mchannel1.min2);

  Connect(maialloc1,maialloc1.mout1,mblockuser1,mblockuser1.min2);
  Connect(maialloc1,maialloc1.mout2,mblockuser1,mblockuser1.min3);
  Connect(mbitsource1,mbitsource1.mout1,mblockuser1,mblockuser1.min1);

  Connect(mblockuser1,mblockuser1.mout1,mchannel1,mchannel1.min1);

  Connect(mchannel1,mchannel1.mout1,mblockrec1,mblockrec1.min1);
  Connect(mcoeffs1,mcoeffs1.mout1,mblockrec1,mblockrec1.min2);
  Connect(maialloc1,maialloc1.mout1,mblockrec1,mblockrec1.min3);

  Connect(mblockrec1,mblockrec1.mout1,mbitber1,mbitber1.min2);

  Connect(mbitsource1,mbitsource1.mout1,mbitber1,mbitber1.min1);

  gsl_vector_uint zero;
  DConnect(mbitber1,mbitber1.vout1,maialloc1,maialloc1.vin2,zero);

  ////////
  ////////
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////

  
  // do not touch this
  Block::Build();
}

void System::Setup(){


  // Noise Variance Setup
  double ebnol=pow(10.0,(Ebno()/10.0));

  //
  // variance_I = variance_Q = variance_N * 0.5
  //
  //              (N+L) * sigmax^2 * tr( theta^H theta )
  // variance_N = ---------------------------------------   
  //                 N * ebnol * K * Nb
  //
  //
  //                       (N+L)
  // variance_N = ------------------------   
  //                 N * ebnol * Nb
  //

  //
  // ebnol = Eb / No = Es / (Nb No)
  //
  // without pathloss --> Es = 1
  //
  // ebnol = 1 / (Nb No)
  // No = 1 / (ebnol Nb) --> noisevar_I/Q = No/2 = 1 / (2 ebnol Nb)
  //
  //
  // with patloss at dref --> Es = 1/ploss(dref)
  //
  // noisevar_I/Q = No/2 = 1 / (2 ebnol Nb ploss(refdist))
  // No [dB] = -Ebno -Nb -ploss(refdist)


  double Nodb = -Ebno() - mudisp::lintodb(Nb()) - mudisp::OkumuraHataCitydB(RDist(),1500);
  double noisevar = 0.5*mudisp::dbtolin(Nodb);

  cout << "NodB (dB and linear) = " << Nodb << " , " << noisevar << endl;

  mchannel1.SetParameter("Variance",0.5*noisevar);

//  mchannel1.SetParameter("Variance",1.0/(2.0*ebnol*Nb()));


  mbitber1.SetParameter("Value",Ebno());
  mbitber1.SetParameter("BitUser",K()*Nb());
    
  mbitsource1.SetParameter("BitUser",K()*Nb());


}


//
// You shoud not modify this ...
//
void System::Run(){
    for (unsigned int i=0; (i < _runs()) && (! mbitber1.getStopFlag()) ;i++)
      Block::Run();
}
