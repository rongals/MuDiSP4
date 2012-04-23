//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#ifndef __MUDISP2_USERVEC_H 
#define __MUDISP2_USERVEC_H

#include "mudisp.h"
#include <complex>
#include "tnt.h"
#include "cmat.h"
#include "pulse.h"

using namespace std;
using namespace TNT;

////////
////////   Here put your personal includes
////////


class UserVec : public Block {
private:

////////   Parameters instances

  IntParam D,G,S,Nb,Cd,Dl,Foffs;
  FloatParam A;
  StringParam seqfname;

////////   Local Attributes

  unsigned int symbol_time, M, Ns, outdim, count;
  double txgain;
  complex <double> symbol_arg, phshift, phincr;
  complex <double> new_symbol;
  Vector < complex < double > > symbol_hist;
  Vector < double > code;
  Vector < unsigned int > gray_encoding;
  TxPulse pulse1;


public:

////////   InPorts and OutPorts

  InPort < unsigned int > in1;
  OutPort < Matrix < complex <double> > > vout1;  


////////   constructor


  UserVec():Block("UserVec")
    
////////  parameters initializazion

    ,D("SemiWin",1,"half the observation window")
    ,G("SpreadFact",63,"spreading factor")
    ,S("ChipRes",1,"samples per chip")
    ,Nb("BitSymb",1,"bits per symbol (PSK mapping)")
    ,Cd("CodeNum",1,"channel number (code)")
    ,Dl("Delay",0,"delay (samples)")
    ,A("Gain",0.0,"received power (dB)")
    ,Foffs("FreqOffs",0,"frequency offset (symb/cycles)")
    ,seqfname("SeqFName","sequences/gold_63.dat","sequences matrix filename")

    {

//////// local parameter registration

      AddParameter(Foffs);
      AddParameter(D);
      AddParameter(G);
      AddParameter(S);
      AddParameter(Nb);
      AddParameter(Cd);
      AddParameter(Dl);
      AddParameter(A);
      AddParameter(seqfname);

    }

  void Setup();
  void Run();
  void Finish();
  
};

#endif /* __MUDISP_MYBLOCK_H  */

