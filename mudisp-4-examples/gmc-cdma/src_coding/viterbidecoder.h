//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP3_VITERBIDECODER_H 
#define __MUDISP3_VITERBIDECODER_H

#include "mudisp.h"
#include "gsl_mudisp.h"
#include "gsl/gsl_complex_math.h"


////////
////////   Here put your personal includes
////////


class ViterbiDecoder : public Block {
 private:

  ////////   Parameters instances
  
  StringParam GPfn;
  IntParam CType;
  

  ////////   Local Attributes
  
  gsl_matrix *gp_mat;
  
  ////////   Local Attributes
  
public:

////////   InPorts and OutPorts

  InPort < gsl_vector_class > vin1;
  OutPort < gsl_vector_uint_class > vout1;  


  ViterbiDecoder():Block("ViterbiDecoder")
    
	     ////////  parameters initializazion

	       ,GPfn("GenPolyFN","files/gsl_matrix_genpoly_12k7.dat","polinomial matrix")
	       ,CType("CodingType",0,"outer (en)decoder: 0=RSC 1=NSC")

    
  {
    
    //////// local parameter registration
    
    AddParameter(GPfn);
    AddParameter(CType);
 
  }
  
  void Setup();
  void Run();


  
};

#endif /* __MUDISP_MYBLOCK_H  */

