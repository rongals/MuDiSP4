//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP2_MCPMPCHAN_H 
#define __MUDISP2_MCPMPCHAN_H

#include "mudisp.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_matrix.h"


////////
////////   Here put your personal includes
////////

// Channel Matrix 
//

class MCPMPChan : public Block {
 private:

  ////////   Parameters instances

  IntParam N, M; 
  
  ////////   Local Attributes
 
  gsl_matrix_complex *user_chan, *outmat;
 
  
public:

////////   InPorts and OutPorts

  InPort < gsl_matrix_complex > min1; // input signal matrix NxM complex
  InPort < gsl_matrix_complex > min2; // input channel coefficient matrix M**2 x N complex
  
  OutPort < gsl_matrix_complex > mout1;  // output signal matrix NxM



  MCPMPChan():Block("MCPMPChan")
    ////////  parameters initializazion
    ,M("NumUsers",2,"number of users")
    ,N("Carriers",16,"number of carriers")
    {

      //////// local parameter registration
      AddParameter(N);
      AddParameter(M);

    }

  ~MCPMPChan() {
  }

  void Setup();
  void Run();


  
};

#endif /* __MUDISP_MYBLOCK_H  */

