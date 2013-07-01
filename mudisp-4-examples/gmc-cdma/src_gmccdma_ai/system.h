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

/*
                                          +-----------+
                                          |mbitsource1|
                                          +-----------+
                                                |
                                                v
                     +-------------+      +-----------+
                 +-->|maiallocator1|-+-2->|mblockuser1|
                 |   +-------------+ |    +-----------+
                 |     ^             |          |
                 |     |             |          v
   +-----------+ |     |             |    +-----------+
   |mcpchcoeff1|++-------------------+-+->| mchannel1 |
   +-----------+       |             | |  +-----------+
         |             |             | |        |
         |             |             | |        v
         v             |             | |  +-----------+
     track.kml         |             | |  |  mawgn1   |
         |             |             | |  +-----------+
         v             |             | |        |
 +--------------+      |             | |        v
 | Google Earth |      |             | ..>+-----------+
 +--------------+      |             |...>|mblockrec11|
                       |                  +-----------+
                       |                        |
                       |                        v
                       |                  +-----------+
                       -<--DelayConnect-<-| mbitber1  |
                                          +-----------+


*/


#include "gmccdma.h"
#include "mblockuser.h"
#include "mbitsrc.h"
#include "maiallocator.h"
#include "mcpmpcoeffs.h"
#include "mcpmpchan.h"
#include "mblockrec.h"
#include "mbitber.h"




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
  FloatParam Ebno, RDist;


  MCPMPCoeffs mcoeffs1;

  MAIAllocator maialloc1;

  MBitSource mbitsource1;
  MBlockUser mblockuser1;

  MCPMPChan  mchannel1;

  MBlockRec  mblockrec1;

  MBitBer     mbitber1;

  /////////
  /////////
  //////////////////////////////////////////////////////
  //////////////////////////////////////////////////////

public:

  System():Block("System")
	  ,_runs("Iterations",1,"Main iterations")
	  ,Ebno("EbNo",3,"SNR (dB)")
	  ,RDist("RDist",1000,"Reference Distance (m)")
	  ,K("SourceSymbs",2,"source symbols")
	  ,J("CodedSymbs",4,"coded symbols")
	  ,N("Carriers",16,"number of carriers")
	  ,Nb("BitSymb",1,"bits per symbol (PSK mapping)")
	  ,M("NumUsers",4,"number of users")
  {

    AddParameter(M);
    AddParameter(N);
    AddParameter(J);
    AddParameter(K);
    AddParameter(Nb);
    AddParameter(Ebno);    
    AddParameter(RDist);
    AddParameter(_runs);


    mblockuser1.ExportParameter("SourceSymbs");
    mblockuser1.ExportParameter("CodedSymbs");
    mblockuser1.ExportParameter("Carriers");
    mblockuser1.ExportParameter("BitSymb");
    mblockuser1.ExportParameter("NumUsers");
        
    mchannel1.ExportParameter("Carriers");
    mchannel1.ExportParameter("NumUsers");

    mblockrec1.ExportParameter("SourceSymbs");
    mblockrec1.ExportParameter("CodedSymbs");
    mblockrec1.ExportParameter("Carriers");
    mblockrec1.ExportParameter("BitSymb");
    mblockrec1.ExportParameter("NumUsers");

    
    mcoeffs1.ExportParameter("Carriers");
    mcoeffs1.ExportParameter("NumUsers");

    maialloc1.ExportParameter("SourceSymbs");
    maialloc1.ExportParameter("BitSymb");
    maialloc1.ExportParameter("CodedSymbs");
    maialloc1.ExportParameter("Carriers");
    maialloc1.ExportParameter("NumUsers");

    mbitber1.ExportParameter("NumUsers");
    mbitsource1.ExportParameter("NumUsers");
    

  }
  
  ~System();
  
  void Build();
  void Setup();
  void Run();
  
};

#endif /* __MUDISP_SYSTEM_H  */
