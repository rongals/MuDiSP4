//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#ifndef __MUDISP2_MBITBER_H 
#define __MUDISP2_MBITBER_H

#include "mudisp.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_matrix.h"


////////
////////   Here put your personal includes
////////


class MBitBer : public Block {
private:

////////   Parameters instances

  IntParam M, bpu, maxerrs;
  FloatParam value;
  StringParam fname;

////////   Local Attributes

  gsl_vector_uint *errcount, *lasterrs;
  gsl_vector_uint *bitcount, *dumperrs;
  

  unsigned int framecount;
  bool stopFlag;


public:

////////   InPorts and OutPorts

  InPort < gsl_matrix_uint > min1; // reference stream
  InPort < gsl_matrix_uint > min2; // received stream 

  OutPort < gsl_vector_uint > vout1; // receiver errors reports 

  
  MBitBer():Block("MBitBer")
    
////////  parameters initializazion
  	,stopFlag(false)
    ,framecount(0)
    ,value("Value",0.0,"index value")
    ,fname("OutFile","cout")
  	,maxerrs("MaxErrs",100,"max errs to stop (0=off)")
    ,M("NumUsers",2,"number of users")
    ,bpu("BitUser",2,"number of bit per user ")
    {

//////// local parameter registration

      AddParameter(value);
      AddParameter(fname);
      AddParameter(maxerrs);
      AddParameter(M);
      AddParameter(bpu);
    }

  void Setup();
  void Run();
  void Finish();
  bool getStopFlag();
  
};

#endif /* __MUDISP_MBITBER_H  */

