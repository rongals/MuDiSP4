//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP2_MCPMPCOEFFS_H 
#define __MUDISP2_MCPMPCOEFFS_H

#include "mudisp.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_rng.h"


////////
////////   Here put your personal includes
////////


class MCPMPCoeffs : public Block {
 private:

  ////////   Parameters instances

  IntParam N,L,M; 
  FloatParam PTau;
  FloatParam Ricean;


  ////////   Local Attributes

  double gain, gainrice;
  gsl_matrix_complex *ch;
  gsl_rng * ran; 
  unsigned int _M;

public:

////////   InPorts and OutPorts

  OutPort < gsl_matrix_complex > mout1;  // complex mat MxMxN 



  MCPMPCoeffs():Block("MCPMPCoeffs")
    ////////  parameters initializazion
    ,M("NumUsers",2,"number of users")
    ,N("Carriers",16,"number of carriers")
    ,L("ChanTaps",2,"number of channel taps")
    ,PTau("PowerTau",1.0,"decay factor of delay spread")
    ,Ricean("RiceFact",0.0,"Ricean factor of channel")
    {

      //////// local parameter registration
      AddParameter(N);
      AddParameter(M);
      AddParameter(L);
      AddParameter(PTau);
      AddParameter(Ricean);

    }

  ~MCPMPCoeffs() {
  }

  void Setup();
  void Run();
  void Finish();


  
};

#endif /* __MUDISP_MYBLOCK_H  */

