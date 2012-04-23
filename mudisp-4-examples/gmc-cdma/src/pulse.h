//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP2_PULSE_H 
#define __MUDISP2_PULSE_H

#include <math.h>

class TxPulse {

  double outgain;
  unsigned int length;
  
 public:

  TxPulse(unsigned int len=1): 
    outgain(1/sqrt(double(len))),
    length(len) {}

  double operator()(unsigned int t);
  
};


#endif
