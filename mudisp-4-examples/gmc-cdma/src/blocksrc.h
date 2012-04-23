//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#ifndef __MUDISP2_BLOCKSRC_H 
#define __MUDISP2_BLOCKSRC_H

#include "mudisp.h"
#include "gsl/gsl_rng.h"
#include "gsl_mudisp.h"

////////
////////   Here put your personal includes
////////


class BlockSource : public Block {
private:

////////   Parameters instances

  IntParam N;

////////   Local Attributes

  gsl_rng * ran; 

public:

////////   InPorts and OutPorts

  OutPort <gsl_vector_uint_class> vout1;  



  BlockSource():Block("BlockSource")
    
////////  parameters initializazion
	       ,N("UncodedLen",11,"dataword length")

    {

//////// local parameter registration
      AddParameter(N);

    }

  void Setup();
  void Run();
  
};

#endif /* __MUDISP_MYBLOCK_H  */

