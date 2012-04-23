//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "delay.h"


void Delay::Setup() {

  //////// initialization of dynamic data structures

  for (int i = 0; i< shift(); i++)
    shreg.push_back(0);

  //////// rate declaration for ports
  

}


void Delay::Run() {

  unsigned int t,tt;
  
  tt=in1.GetDataObj();
  
  shreg.push_back(tt);
  tt=shreg.front();
  shreg.pop_front();
  
  //////// production of data
  
  out1.DeliverDataObj(tt);
}
