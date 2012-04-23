//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP3_PSKMAPPER_H 
#define __MUDISP3_PSKMAPPER_H

#include "mudisp.h"
#include "gsl_mudisp.h"
#include "gsl/gsl_complex_math.h"


////////
////////   Here put your personal includes
////////


class PSKMapper : public Block {
 private:

  ////////   Parameters instances
  
   IntParam Nb;
 

  ////////   Local Attributes
  
  unsigned int symbol_time, Ns, symbol_id;
  double symbol_arg;

  gsl_complex  phshift,  phincr,  new_symbol;
  gsl_vector_uint * gray_encoding; 


  
public:

////////   InPorts and OutPorts

  InPort < gsl_vector_uint_class > vin1;
  OutPort < gsl_vector_complex_class > vout1;  


  PSKMapper():Block("PSKMapper")
    
	     ////////  parameters initializazion

    ,Nb("BitSymb",2,"bits per symbol (PSK mapping)")
    
    
  {
    
    //////// local parameter registration

    AddParameter(Nb);
    
    
    
  }
  
  void Setup();
  void Run();


  
};

#endif /* __MUDISP_MYBLOCK_H  */

