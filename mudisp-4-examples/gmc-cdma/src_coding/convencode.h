//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP3_CONVENCODER_H 
#define __MUDISP3_CONVENCODER_H

#include "mudisp.h"
#include "gsl/gsl_matrix.h"
#include "gsl_mudisp.h"



////////
////////   Here put your personal includes
////////


class ConvEncoder : public Block {
 private:

  ////////   Parameters instances
  
  StringParam GPfn;
  IntParam CType;
  

  ////////   Local Attributes
  
  gsl_matrix *gp_mat;
  gsl_vector_uint_class initVecClass;

public:

////////   InPorts and OutPorts

  InPort < gsl_vector_uint_class > vin1;
  OutPort < gsl_vector_uint_class > vout1;  


  ConvEncoder():Block("ConvEncoder")
    
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

