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

#include "gmccdma.h"
#include "mblockuser.h"
#include "bitsrc.h"
#include "maiallocator.h"
#include "mcpmpcoeffs.h"

//#include "blockrec.h"
//#include "bitber.h"
//#include "awgn.h"
//#include "cpmpchan.h"



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
  ////////
  ////////
  ////////
  //
  //
  //


  IntParam K,J,N,Nb,M;
  FloatParam Ebno;


  MCPMPCoeffs mcoeffs1;

  MAIAllocator maialloc1;

  BitSource bitsource1;
  MBlockUser mblockuser1;

  //  MCPMPChan  mchannel1;

  //  MAWGN      mawgn1;
  //  MBlockRec  mblockrec1;
  //  BitBer     bitber1;

  /////////
  /////////
  //////////////////////////////////////////////////////
  //////////////////////////////////////////////////////

public:

  System():Block("System")
	  ,_runs("Iterations",1,"Main iterations")
	  ,Ebno("EbNo",3,"SNR (dB)")
	  ,K("SourceSymbs",2,"source symbols")
	  ,J("CodedSymbs",4,"coded symbols")
	  ,N("Carriers",32,"number of carriers")
	  ,Nb("BitSymb",1,"bits per symbol (PSK mapping)")
	  ,M("NumUsers",2,"number of users")
  {

    AddParameter(M);
    AddParameter(N);
    AddParameter(J);
    AddParameter(K);
    AddParameter(Nb);
    AddParameter(Ebno);    
    AddParameter(_runs);


    mblockuser1.ExportParameter("SourceSymbs");
    mblockuser1.ExportParameter("CodedSymbs");
    mblockuser1.ExportParameter("Carriers");
    mblockuser1.ExportParameter("BitSymb");
    mblockuser1.ExportParameter("NumUsers");
        
    /* mchannel1.ExportParameter("Carriers"); */
    /* mchannel1.ExportParameter("NumUsers"); */

    /* mblockrec1.ExportParameter("SourceSymbs"); */
    /* mblockrec1.ExportParameter("CodedSymbs"); */
    /* mblockrec1.ExportParameter("Carriers"); */
    /* mblockrec1.ExportParameter("BitSymb"); */
    /* mblockrec1.ExportParameter("NumUsers"); */

    /* mawgn1.ExportParameter("NumUsers"); */
    
    mcoeffs1.ExportParameter("Carriers");
    mcoeffs1.ExportParameter("NumUsers");

    maialloc1.ExportParameter("CodedSymbs");
    maialloc1.ExportParameter("Carriers");
    maialloc1.ExportParameter("NumUsers");
    

  }
  
  ~System();
  
  void Build();
  void Setup();
  void Run();
  
};

#endif /* __MUDISP_SYSTEM_H  */
