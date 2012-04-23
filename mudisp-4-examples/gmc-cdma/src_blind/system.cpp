//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 

#include "system.h"

System::~System(){
  delete [] users;
  delete [] bitsources;
}

void System::Build() {

  ////////////////////////////////////////////////////////
  //////// Here there are the Connect instructions and  
  //////// any other stuff to be executed once before the 
  //////// parameters reading.
  ////////

  for (int i=0; i<USERS; i++) {
    Connect(bitsources[i],bitsources[i].out1,users[i],users[i].in1);
    Connect(users[i],users[i].vout1, awgn1, awgn1.in[i]);
  }

  Connect(awgn1, awgn1.vout1, corr1, corr1.vin1);
  Connect(bitsources[0],bitsources[0].out1,bitber1,bitber1.in1);
  Connect(corr1, corr1.out1, bitber1, bitber1.in2);

  ////////
  ////////
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////

  
  // do not touch this
  Block::Build();
}

void System::Setup(){

  double ebnol=pow(10.0,(ebno()/10.0));

  SetSeed(seed(),2*seed()-1);

  awgn1.SetParameter("Variance",1.0/(2.0*ebnol));

  bitber1.SetParameter("Shift1",Nb()*(1+2*D()));
  bitber1.SetParameter("Value",ebno());

  users[0].SetParameter("CodeNum",1);
  users[0].SetParameter("Delay",0);

  for (int i=1; i<USERS; i++) {
    users[i].SetParameter("CodeNum",i+1);
    users[i].SetParameter("Delay",int(rndUniformInt(-G()*S()+1,G()*S())));
  }

}


//
// You shoud not modify this ...
//
void System::Run(){
    for (unsigned int i=0; i<_runs();i++)
      Block::Run();
}
