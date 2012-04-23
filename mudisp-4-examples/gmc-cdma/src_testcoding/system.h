//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP2_SYSTEM_H
#define __MUDISP2_SYSTEM_H

#include "mudisp.h"

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////   Here put your personal includes
////////

#include "convencode.h"
#include "blocksrc.h"
#include "pskmapper.h"
#include "blockawgn.h"
#include "softdemapper.h"
#include "viterbidecoder.h"
#include "perfeval.h"


////////
////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

class System : public Block {
private:
  IntParam _runs;
  
  //////////////////////////////////////////////////////
  ////////// ////////////////////////////////////////////
  ///////// your personal system wide parameters go here
  ///////// along with the block instances of your system
  ///////// 
  /////////

  
  StringParam GPfn;
  IntParam CType;
  IntParam Nb;
  FloatParam EbNodB, CodeRate;


  BlockSource blocksource1;
  ConvEncoder convencoder1;
  PSKMapper pskmapper1;
  BlockAWGN blockawgn1;
  SoftDemapper softdemapper1;
  ViterbiDecoder viterbidecoder1;
  PerfEval perfeval1;


  /////////
  /////////
  //////////////////////////////////////////////////////
  //////////////////////////////////////////////////////

public:

  System():Block("System")
 	  ,_runs("Iterations",1,"Main iterations")
          ,blockawgn1(USERS)
	  ,GPfn("GenPolyFN","files/gsl_matrix_genpoly_12k7.dat")
	  ,CType("CodingType",0,"outer (en)decoder: 0=RSC 1=NSC")
	  ,Nb("BitSymb",2,"bits per symbol (PSK mapping)")
	  ,EbNodB("EbNo_db",10,"Eb/No in dB")
	  ,CodeRate("CodeRate",0.5,"coding rate")
 {

    AddParameter(_runs);
    AddParameter(GPfn);
    AddParameter(CType);
    AddParameter(Nb);
    AddParameter(EbNodB);
    AddParameter(CodeRate);


    convencoder1.ExportParameter("GenPolyFN");
    viterbidecoder1.ExportParameter("GenPolyFN");

    convencoder1.ExportParameter("CodingType");
    viterbidecoder1.ExportParameter("CodingType");

    pskmapper1.ExportParameter("BitSymb");
    softdemapper1.ExportParameter("BitSymb");



  }
  
  ~System();
  
  void Build();
  void Setup();
  void Run();
  
};

#endif /* __MUDISP_SYSTEM_H  */
