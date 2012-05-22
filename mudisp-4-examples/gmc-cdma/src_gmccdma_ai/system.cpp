//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 

#include "system.h"

System::~System(){
}

void System::Build() {

  ////////////////////////////////////////////////////////
  //////// Here there are the Connect instructions and  
  //////// any other stuff to be executed once before the 
  //////// parameters reading.
  ////////

  Connect(mcoeffs1,mcoeffs1.mout1,maialloc1,maialloc1.min1);
  // Connect(mcoeffs1,mcoeffs1.mout1,mchannel1,mchannel1.min2);

  Connect(maialloc1,maialloc1.mout1,mblockuser1,mblockuser1.min2);
  Connect(bitsource1,bitsource1.out1,mblockuser1,mblockuser1.in1);

  // Connect(mblockuser1,mblockuser1.vout1,mchannel1,mchannel1.vin1);
  // Connect(mchannel1,mchannel1.vout1,mawgn1,mawgn1.in1);  
  // Connect(mawgn1,mawgn1.vout1,mblockrec1,mblockrec1.vin1);
  // Connect(mblockrec1,mblockrec1.out1,bitber1,bitber1.in2);

  // Connect(coeffs1,coeffs1.mout1,blockrec1,blockrec1.min2);
  // Connect(aialloc1,aialloc1.mout1,blockrec1,blockrec1.min3);

  // Connect(bitsource1,bitsource1.out1,bitber1,bitber1.in1);

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

  //  awgn1.SetParameter("Variance",1.0/(2.0*ebnol*Nb()));


  //bitber1.SetParameter("Shift1",0);
  //bitber1.SetParameter("Value",Ebno());
    


}


//
// You shoud not modify this ...
//
void System::Run(){
    for (unsigned int i=0; i<_runs();i++)
      Block::Run();
}
