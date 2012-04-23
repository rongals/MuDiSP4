//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP2_BLOCKAWGN_H 
#define __MUDISP2_BLOCKAWGN_H

#include "mudisp.h"
#include "gsl_mudisp.h"
#include "gsl/gsl_rng.h"

////////
////////   Here put your personal includes
////////


class BlockAWGN : public Block {
 private:

  ////////   Parameters instances
  
  FloatParam noisevar;
  
  ////////   Local Attributes
  
  unsigned int Nin;
  double noisestd;

  gsl_rng * ran; 

  
public:

////////   InPorts and OutPorts

  InPort < gsl_vector_complex_class > *vin;
  OutPort < gsl_vector_complex_class > vout1;  



  BlockAWGN(int numin = 1 ):Block("BlockAWGN")
    
////////  parameters initializazion
    ,Nin(numin)
    ,noisevar("Variance",0.5,"gaussian noise variance (linear)")

    {

      //////// local parameter registration
      AddParameter(noisevar);
      vin=new InPort < gsl_vector_complex_class >[numin];

    }

  ~BlockAWGN() {
    delete [] vin;
  }

  void Setup();
  void Run();


  
};

#endif /* __MUDISP_MYBLOCK_H  */

