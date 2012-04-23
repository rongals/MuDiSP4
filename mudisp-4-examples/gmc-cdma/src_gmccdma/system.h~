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
#include "blockuser.h"
#include "bitsrc.h"
//#include "gslwrite.h"
#include "blockrec.h"
#include "bitber.h"
#include "awgn.h"
#include "cpmpchan.h"
#include "cpmpcoeffs.h"
#include "allocator.h"

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

  IntParam K,J,N,Nb;
  FloatParam Ebno;


  CPMPCoeffs coeffs1;

  Allocator alloc1;

  BitSource *bitsource;
  BlockUser *blockuser;
  CPMPChan  *channel;

  AWGN      awgn1;
  BlockRec  blockrec1;
  BitBer    bitber1;

  /////////
  /////////
  //////////////////////////////////////////////////////
  //////////////////////////////////////////////////////

public:

  System():Block("System")
	  ,awgn1(USERS)
	  ,_runs("Iterations",1,"Main iterations")
	  ,Ebno("EbNo",3,"SNR (dB)")
	  ,K("SourceSymbs",2,"source symbols")
	  ,J("CodedSymbs",4,"coded symbols")
	  ,N("Carriers",32,"number of carriers")
	  ,Nb("BitSymb",1,"bits per symbol (PSK mapping)")
  {


    AddParameter(N);
    AddParameter(J);
    AddParameter(K);
    AddParameter(Nb);
    AddParameter(Ebno);    
    AddParameter(_runs);

    bitsource = new BitSource[USERS];
    blockuser = new BlockUser[USERS];
    channel = new CPMPChan[USERS];


    for (int i=0; i<USERS; i++) {

      blockuser[i].ExportParameter("SourceSymbs");
      blockuser[i].ExportParameter("CodedSymbs");
      blockuser[i].ExportParameter("Carriers");
      blockuser[i].ExportParameter("BitSymb");
        
      channel[i].ExportParameter("Carriers");
    }

      blockrec1.ExportParameter("SourceSymbs");
      blockrec1.ExportParameter("CodedSymbs");
      blockrec1.ExportParameter("Carriers");
      blockrec1.ExportParameter("BitSymb");


    coeffs1.ExportParameter("Carriers");

    alloc1.ExportParameter("CodedSymbs");
    alloc1.ExportParameter("Carriers");


  }
  
  ~System();
  
  void Build();
  void Setup();
  void Run();
  
};

#endif /* __MUDISP_SYSTEM_H  */
