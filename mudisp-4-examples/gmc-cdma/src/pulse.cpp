//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "pulse.h"

double TxPulse::operator()(unsigned int t){
  return outgain;
}

