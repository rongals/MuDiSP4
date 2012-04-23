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

  Connect(blocksource1,blocksource1.vout1,convencoder1,convencoder1.vin1);
  Connect(convencoder1,convencoder1.vout1,pskmapper1,pskmapper1.vin1);
  Connect(pskmapper1,pskmapper1.vout1,blockawgn1,blockawgn1.vin[0]);
  Connect(blockawgn1,blockawgn1.vout1,softdemapper1,softdemapper1.vin1);
  Connect(softdemapper1,softdemapper1.vout1,viterbidecoder1,viterbidecoder1.vin1);
  Connect(viterbidecoder1,viterbidecoder1.vout1,perfeval1,perfeval1.in2);
  Connect(blocksource1,blocksource1.vout1,perfeval1,perfeval1.in1);


  ////////
  ////////
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////

  
  // do not touch this
  Block::Build();
}

void System::Setup(){


//   // Noise Variance Setup
  double ebnol=pow(10.0,(EbNodB()/10.0));
  double esnol=ebnol * Nb() * CodeRate();


//   //
//   // variance_I = variance_Q = variance_N * 0.5
//   //
//   //              (N+L) * sigmax^2 * tr( theta^H theta )
//   // variance_N = ---------------------------------------   
//   //                 N * ebnol * K * Nb
//   //
//   //
//   //                       (N+L)
//   // variance_N = ------------------------   
//   //                 N * ebnol * Nb
//   //

  blockawgn1.SetParameter("Variance",1.0/(2.0*esnol));

  softdemapper1.SetParameter("EsNo",esnol);

  perfeval1.SetParameter("Value",EbNodB());



}


//
// You shoud not modify this ...
//
void System::Run(){
  for (unsigned int i=0; i<_runs() && (! perfeval1.StopCriteria());i++)
      Block::Run();
}
