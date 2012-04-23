//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP2_CPMPCHAN_H 
#define __MUDISP2_CPMPCHAN_H

#include "mudisp.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_matrix.h"


////////
////////   Here put your personal includes
////////

// Channel Matrix 
//

class CPMPChan : public Block {
 private:

  ////////   Parameters instances

  IntParam Cd, N; 
  
  ////////   Local Attributes
 
  gsl_matrix_complex *user_chan;
  
public:

////////   InPorts and OutPorts

  InPort < gsl_vector_complex > vin1;
  InPort < gsl_matrix_complex > min2;
  
  OutPort < gsl_vector_complex > vout1;  



  CPMPChan():Block("CPMPChan")
    ////////  parameters initializazion
    ,Cd("CodeNum",0,"channel number (code)")
    ,N("Carriers",16,"number of carriers")
    {

      //////// local parameter registration
      AddParameter(Cd);
      AddParameter(N);

    }

  ~CPMPChan() {
  }

  void Setup();
  void Run();


  
};

#endif /* __MUDISP_MYBLOCK_H  */

