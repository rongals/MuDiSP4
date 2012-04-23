//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#ifndef __MUDISP3_PERFEVAL_H 
#define __MUDISP3_PERFEVAL_H

#include "mudisp.h"
#include <deque>
#include "gsl_mudisp.h"

////////
////////   Here put your personal includes
////////


class PerfEval : public Block {
private:

////////   Parameters instances

  IntParam shift, ignore, errtarget;
  FloatParam value;
  StringParam fname;

////////   Local Attributes

  unsigned int errcount, lasterrs;
  unsigned int bitcount;
  unsigned int ignorecount;

  int _stop_criteria;

  deque <gsl_vector_uint_class> shreg;

public:

////////   InPorts and OutPorts

  InPort <gsl_vector_uint_class> in1;
  InPort <gsl_vector_uint_class> in2;
  
  PerfEval():Block("PerfEval")
    
////////  parameters initializazion
	    ,_stop_criteria(0)
	    ,shift("Shift1",0,"input 1 delay")
	    ,ignore("Ignore",0,"leading inputs to ignore (0=shift)")
	    ,value("Value",0.0,"index value")
	    ,fname("OutFile","cout")
	    ,errtarget("ErrTarget",10,"target number of errors")
    {

//////// local parameter registration

      AddParameter(shift);
      AddParameter(value);
      AddParameter(ignore);
      AddParameter(fname);
      AddParameter(errtarget);

    }

  void Setup();
  void Run();
  void Finish();

  int StopCriteria();
  
  
};

#endif /* __MUDISP_BITBER_H  */

