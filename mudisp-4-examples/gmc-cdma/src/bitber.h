//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#ifndef __MUDISP2_BITBER_H 
#define __MUDISP2_BITBER_H

#include "mudisp.h"
#include <deque>


////////
////////   Here put your personal includes
////////


class BitBer : public Block {
private:

////////   Parameters instances

  IntParam shift, ignore;
  FloatParam value;
  StringParam fname;

////////   Local Attributes

  unsigned int errcount, lasterrs;
  unsigned int bitcount;
  unsigned int ignorecount;

  deque <unsigned int> shreg;

public:

////////   InPorts and OutPorts

  InPort <unsigned int> in1;
  InPort <unsigned int> in2;
  
  BitBer():Block("BitBer")
    
////////  parameters initializazion

    ,shift("Shift1",0,"input 1 delay")
    ,ignore("Ignore",0,"leading bits to ignore (0=shift)")
    ,value("Value",0.0,"index value")
    ,fname("OutFile","cout")

    {

//////// local parameter registration

      AddParameter(shift);
      AddParameter(value);
      AddParameter(ignore);
      AddParameter(fname);

    }

  void Setup();
  void Run();
  void Finish();
  
};

#endif /* __MUDISP_BITBER_H  */

