//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#ifndef __MUDISP2_MBITBER_H 
#define __MUDISP2_MBITBER_H

#include "mudisp.h"
#include <deque>
#include "gsl/gsl_vector.h"


////////
////////   Here put your personal includes
////////


class MBitBer : public Block {
private:

////////   Parameters instances

  IntParam shift, ignore, M, bpu;
  FloatParam value;
  StringParam fname;

////////   Local Attributes

  gsl_vector_uint *errcount, *lasterrs;
  gsl_vector_uint *bitcount;
  

  unsigned int ignorecount, framepos, framelenght;

  deque <unsigned int> shreg; // delay before demultiplexing 

public:

////////   InPorts and OutPorts

  InPort <unsigned int> in1; // reference stream
  InPort <unsigned int> in2; // received stream 
  
  MBitBer():Block("MBitBer")
    
////////  parameters initializazion

    ,shift("Shift1",0,"input 1 delay")
    ,ignore("Ignore",0,"leading bits to ignore (0=shift)")
    ,value("Value",0.0,"index value")
    ,fname("OutFile","cout")
    ,M("NumUsers",2,"number of users")
    ,bpu("BitUser",2,"number of bit per user ")
    {

//////// local parameter registration

      AddParameter(shift);
      AddParameter(value);
      AddParameter(ignore);
      AddParameter(fname);
      AddParameter(M);
      AddParameter(bpu);
    }

  void Setup();
  void Run();
  void Finish();
  
};

#endif /* __MUDISP_MBITBER_H  */

