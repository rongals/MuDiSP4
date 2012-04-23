//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP2_AWGN_H 
#define __MUDISP2_AWGN_H

#include "mudisp.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_rng.h"

////////
////////   Here put your personal includes
////////


class AWGN : public Block {
 private:

  ////////   Parameters instances
  
  FloatParam noisevar;
  
  ////////   Local Attributes
  
  unsigned int Nin;
  double noisestd;

  gsl_rng * ran; 

  
public:

////////   InPorts and OutPorts

  InPort < gsl_vector_complex > *in;
  OutPort < gsl_vector_complex > vout1;  



  AWGN(int numin = 1 ):Block("AWGN")
    
////////  parameters initializazion
    ,Nin(numin)
    ,noisevar("Variance",0.5,"gaussian noise variance (linear)")

    {

      //////// local parameter registration
      AddParameter(noisevar);
      in=new InPort < gsl_vector_complex >[numin];

    }

  ~AWGN() {
    delete [] in;
  }

  void Setup();
  void Run();


  
};

#endif /* __MUDISP_MYBLOCK_H  */

