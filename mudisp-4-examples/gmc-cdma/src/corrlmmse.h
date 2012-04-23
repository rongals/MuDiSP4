//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#ifndef __MUDISP2_CORRLMMSE_H 
#define __MUDISP2_CORRLMMSE_H

#include "mudisp.h"
#include <complex>
#include "tnt.h"
#include "cmat.h"
#include "transv.h"
#include "pulse.h"

using namespace std;
using namespace TNT;

////////
////////   Here put your personal includes
////////


class CorrLMMSE : public Block {
private:

////////   Parameters instances

  IntParam D,G,S,Nb,Cd;
  FloatParam A, mu, ni, Th;
  StringParam seqfname;

////////   Local Attributes

  unsigned int symbol_time, M, Ns, indim, count;
  complex <double> symbol_arg;
  Vector < double > code;
  Matrix < complex <double> > wvec, xvec ,svec, Fmat; 
  Vector < unsigned int > gray_decoding;
  TxPulse pulse1;
  bool isLMMSE;


  ////////   distance
  double CorrLMMSE::distance(unsigned int, complex<double>);

public:

////////   InPorts and OutPorts

  OutPort < unsigned int > out1;
  InPort < Matrix < complex <double> > > vin1;  


////////   constructor


  CorrLMMSE():Block("CorrLMMSE")
    
////////  parameters initializazion

    ,D("SemiWin",1,"half the observation window")
    ,G("SpreadFact",63,"spreading factor")
    ,S("ChipRes",1,"samples per chip")
    ,Nb("BitSymb",1,"bits per symbol (PSK mapping)")
    ,Cd("CodeNum",1,"channel number (code)")
    ,mu("Mu",0.1,"step in the gradient descent algorithm")
    ,Th("LMMSETh",0.01,"threshold for blind -> LMMSE")
    ,ni("Ni",0.0,"surplus energy limitator")
    ,seqfname("SeqFName","sequences/gold_63.dat","sequences matrix filename")

    {

//////// local parameter registration

      AddParameter(D);
      AddParameter(G);
      AddParameter(S);
      AddParameter(mu);
      AddParameter(ni);
      AddParameter(Nb);
      AddParameter(Th);
      AddParameter(Cd);
      AddParameter(seqfname);

    }

  void Setup();
  void Run();
  void Finish();
  
};


#endif /* __MUDISP_MYBLOCK_H  */

