//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#ifndef __MUDISP2_MBITSRC_H 
#define __MUDISP2_MBITSRC_H

#include "mudisp.h"
#include "gsl/gsl_rng.h"
#include "gsl/gsl_matrix.h"

////////
////////   Here put your personal includes
////////


class MBitSource : public Block {
private:

////////   Parameters instances

  IntParam M, bpu;

////////   Local Attributes

  gsl_matrix_uint *outmat;
  gsl_rng * ran; 

public:

////////   InPorts and OutPorts

  OutPort < gsl_matrix_uint > mout1; // M x nbu bits



  MBitSource():Block("MBitSource")
    
////////  parameters initializazion
    ,M("NumUsers",2,"number of users")
    ,bpu("BitUser",2,"number of bit per user ")
    {

//////// local parameter registration
      AddParameter(bpu);
      AddParameter(M);

    }

  void Setup();
  void Run();
  void Finish();
  
};

#endif /* __MUDISP_MYBLOCK_H  */

