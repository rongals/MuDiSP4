//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#ifndef __MUDISP2_CORRMULTI_H 
#define __MUDISP2_CORRMULTI_H

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


class CorrMultiBlind : public Block {
private:

////////   Parameters instances

  IntParam D,G,S,Nb,Cd,Nc;
  FloatParam A, mu, ni;
  StringParam seqfname;

////////   Local Attributes

  unsigned int symbol_time, M, Ns, indim, count;
  complex <double> symbol_arg;
  Matrix < double > code;
  Matrix < complex <double> > xvec ,svec, Fmat; 
  Vector < unsigned int > gray_decoding;
  TxPulse pulse1;


  ////////   distance
  double CorrMultiBlind::distance(unsigned int, complex<double>);

public:

////////   InPorts and OutPorts

  OutPort < unsigned int > out1;
  InPort < Matrix < complex <double> > > vin1;  


////////   constructor


  CorrMultiBlind():Block("CorrMultiBlind")
    
////////  parameters initializazion

    ,D("SemiWin",1,"half the observation window")
    ,G("SpreadFact",63,"spreading factor")
    ,S("ChipRes",1,"samples per chip")
    ,Nb("BitSymb",1,"bits per symbol (PSK mapping)")
    ,Cd("CodeNum",1,"initial channel number (code)")
    ,Nc("SyncCodes",1,"synchro channels")
    ,mu("Mu",0.1,"step in the gradient descent algorithm")
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
      AddParameter(Cd);
      AddParameter(Nc);
      AddParameter(seqfname);

    }

  void Setup();
  void Run();
  void Finish();
  
};


#endif /* __MUDISP_MYBLOCK_H  */

