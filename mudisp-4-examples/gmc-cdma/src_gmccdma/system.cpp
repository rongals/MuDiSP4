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

  Connect(coeffs1,coeffs1.mout1,alloc1,alloc1.min1);
  
  for (int i=0;i<USERS;i++) {
    Connect(coeffs1,coeffs1.mout1,channel[i],channel[i].min2);
    Connect(alloc1,alloc1.mout1,blockuser[i],blockuser[i].min2);
    Connect(bitsource[i],bitsource[i].out1,blockuser[i],blockuser[i].in1);
    Connect(blockuser[i],blockuser[i].vout1,channel[i],channel[i].vin1);
    Connect(channel[i],channel[i].vout1,awgn1,awgn1.in[i]);
  }   
  Connect(awgn1,awgn1.vout1,blockrec1,blockrec1.vin1);
  Connect(blockrec1,blockrec1.out1,bitber1,bitber1.in2);
  Connect(coeffs1,coeffs1.mout1,blockrec1,blockrec1.min2);
  Connect(alloc1,alloc1.mout1,blockrec1,blockrec1.min3);
  Connect(bitsource[0],bitsource[0].out1,bitber1,bitber1.in1);

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
  awgn1.SetParameter("Variance",1.0/(2.0*ebnol*Nb()));

  for (int i=0;i<USERS;i++) {
    channel[i].SetParameter("CodeNum",i);
    blockuser[i].SetParameter("CodeNum",i);
  }

    blockrec1.SetParameter("CodeNum",0);
    bitber1.SetParameter("Shift1",0);
    bitber1.SetParameter("Value",Ebno());
    


}


//
// You shoud not modify this ...
//
void System::Run(){
    for (unsigned int i=0; i<_runs();i++)
      Block::Run();
}
