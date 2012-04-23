//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef USERS
#define USERS 2
#endif

#ifndef __MUDISP2_SYSTEM_H
#define __MUDISP2_SYSTEM_H

#include "mudisp.h"

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////   Here put your personal includes
////////

#include "uservec.h"
#include "bitsrc.h"
#include "awgn.h"
#include "corrblind.h"
#include "bitber.h"

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

  IntParam D,G,S,Nb,Cd,seed;
  FloatParam A, ebno;
  StringParam seqfname;


  BitSource *bitsources;
  UserVec   *users;
  AWGN awgn1;
  CorrBlind corr1;
  BitBer bitber1;

  /////////
  /////////
  //////////////////////////////////////////////////////
  //////////////////////////////////////////////////////

public:

  System():Block("System")
    ,awgn1(USERS)
    ,_runs("Iterations",10000,"Main iterations")
    ,seed("RndSeed",1234567L,"random generators seed")
    ,D("SemiWin",1,"half the observation window")
    ,G("SpreadFact",63,"spreading factor")
    ,S("ChipRes",1,"samples per chip")
    ,Nb("BitSymb",1,"bits per symbol (PSK mapping)")
    ,seqfname("SeqFName","sequences/gold_63.dat","sequences matrix filename")
    ,ebno("EbNo",0,"SNR (dB) for desired user")
    {
      AddParameter(_runs);
      AddParameter(seed);
      AddParameter(D);
      AddParameter(G);
      AddParameter(S);
      AddParameter(Nb);
      AddParameter(seqfname);
      AddParameter(ebno);
      corr1.ExportParameter("SemiWin");
      corr1.ExportParameter("SpreadFact");
      corr1.ExportParameter("ChipRes");
      corr1.ExportParameter("BitSymb");
      corr1.ExportParameter("SeqFName");

      users = new UserVec[USERS];
      bitsources = new BitSource[USERS];

      for (int i=0; i<USERS; i++) {
	users[i].ExportParameter("SemiWin");
	users[i].ExportParameter("SpreadFact");
	users[i].ExportParameter("ChipRes");
	users[i].ExportParameter("BitSymb");
	users[i].ExportParameter("SeqFName");
      }


    }

  ~System();

  void Build();
  void Setup();
  void Run();
  
};

#endif /* __MUDISP_SYSTEM_H  */
