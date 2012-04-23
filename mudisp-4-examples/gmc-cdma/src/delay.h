//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#ifndef __MUDISP2_DELAY_H 
#define __MUDISP2_DELAY_H

#include "mudisp.h"
#include <deque>


////////
////////   Here put your personal includes
////////


class Delay : public Block {
private:

////////   Parameters instances

  IntParam shift;

////////   Local Attributes

  deque <unsigned int> shreg;

public:

////////   InPorts and OutPorts

  InPort <unsigned int> in1;
  OutPort <unsigned int> out1;
  
  Delay():Block("Delay")
    
////////  parameters initializazion

    ,shift("Shift",0,"input delay")

    {

//////// local parameter registration

      AddParameter(shift);


    }

  void Setup();
  void Run();

  
};

#endif /* __MUDISP_BITBER_H  */

