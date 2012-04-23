//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 

#include "system.h"


void System::Build() {

  ////////////////////////////////////////////////////////
  //////// Here there are the Connect instructions and  
  //////// any other stuff to be executed once before the 
  //////// parameters reading.
  ////////

  Connect(user1,user1.out1,writecx1,writecx1.in1);
  Connect(user1,user1.out2,writecx2,writecx2.in1);


  ////////
  ////////
  ////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////

  
  // do not touch this
  Block::Build();
}

//
// You shoud not modify this ...
//
void System::Run(){
  for (unsigned int i=0; i<_runs();i++)
    Block::Run();
}

