//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP3_SOFTDEMAPPER_H 
#define __MUDISP3_SOFTDEMAPPER_H

#include "mudisp.h"
#include "gsl_mudisp.h"
#include "gsl/gsl_complex_math.h"


////////
////////   Here put your personal includes
////////


class SoftDemapper : public Block {
 private:

  ////////   Parameters instances
  
  IntParam Nb,LMAP;
  FloatParam EsNo;
  

  ////////   Local Attributes
  
  unsigned int symbol_time, Ns, symbol_id;
  double symbol_arg;

  gsl_complex  phshift,  phincr,  new_symbol;
  gsl_vector_uint * gray_encoding; 

  static float (*max_star[])(float, float);

  
public:

////////   InPorts and OutPorts

  InPort < gsl_vector_complex_class > vin1;
  OutPort < gsl_vector_class > vout1;  


  SoftDemapper():Block("SoftDemapper")
    
	     ////////  parameters initializazion

		,Nb("BitSymb",2,"bits per symbol (PSK mapping)")
		,EsNo("EsNo",1,"EsNo linear")
		,LMAP("LogMAPType",0,"0=lin 1=max 2=const 3=8approx 4=exact")
    
  {
    
    //////// local parameter registration

    AddParameter(Nb);
    AddParameter(EsNo);
    AddParameter(LMAP);
    
    
    
  }
  
  void Setup();
  void Run();


  
};

#endif /* __MUDISP_MYBLOCK_H  */

