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
#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"


////////
////////   Here put your personal includes
////////

// Channel Matrix 
//

class MCPMPChan : public Block {
 private:

  ////////   Parameters instances

  IntParam N, M; 
  FloatParam noisevar;
    
  ////////   Local Attributes
 
  gsl_matrix_complex *user_chan, *outmat;

  double noisestd;
  gsl_rng * ran; 
 
  
public:

////////   InPorts and OutPorts

  InPort < gsl_matrix_complex > min1; // input signal matrix NxM complex
  InPort < gsl_matrix_complex > min2; // input channel coefficient matrix M**2 x N complex
  
  OutPort < gsl_matrix_complex > mout1;  // output signal matrix NxM



  MCPMPChan():Block("MCPMPChan")
    ////////  parameters initializazion
    ,M("NumUsers",2,"number of users")
    ,N("Carriers",16,"number of carriers")
    ,noisevar("Variance",0.5,"gaussian noise variance (linear)")
    {

      //////// local parameter registration
      AddParameter(N);
      AddParameter(M);
      AddParameter(noisevar);

    }

  ~MCPMPChan() {
  }

  void Setup();
  void Run();
  void Finish();


  
};

#endif /* __MUDISP_MYBLOCK_H  */

