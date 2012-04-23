//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#ifndef __MUDISP2_CORRLMMSETR_H 
#define __MUDISP2_CORRLMMSETR_H

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


class CorrLMMSETr : public Block {
private:

////////   Parameters instances

  IntParam D,G,S,Nb,Cd, Trs, shift;
  FloatParam A, mu;
  StringParam seqfname;

////////   Local Attributes

  unsigned int symbol_time, M, Ns, indim, count;
  complex <double> symbol_arg, true_symbol;
  Vector < double > code;
  Matrix < complex <double> > wvec; 
  Vector < unsigned int > gray_decoding, gray_encoding;
  TxPulse pulse1;
  bool isTrained;

  deque < unsigned int  > shreg;

  ////////   distance
  double CorrLMMSETr::distance(unsigned int, complex<double>);

public:

////////   InPorts and OutPorts

  OutPort < unsigned int > out1;
  InPort < Matrix < complex <double> > > vin1;  
  InPort < unsigned int > in2;

////////   constructor


  CorrLMMSETr():Block("CorrLMMSETr")
    
////////  parameters initializazion

    ,D("SemiWin",1,"half the observation window")
    ,Trs("TraSymb",3000,"leading training symbols")
    ,G("SpreadFact",63,"spreading factor")
    ,S("ChipRes",1,"samples per chip")
    ,Nb("BitSymb",1,"bits per symbol (PSK mapping)")
    ,Cd("CodeNum",1,"channel number (code)")
    ,shift("TraShift",1,"training sequence alignment")
    ,mu("Mu",0.1,"step in the gradient descent algorithm")
    ,seqfname("SeqFName","sequences/gold_63.dat","sequences matrix filename")

    {

//////// local parameter registration

      AddParameter(D);
      AddParameter(G);
      AddParameter(Trs);
      AddParameter(S);
      AddParameter(mu);
      AddParameter(Nb);
      AddParameter(Cd);
      AddParameter(seqfname);
      AddParameter(shift);
    }

  void Setup();
  void Run();
  void Finish();
  
};


#endif /* __MUDISP_MYBLOCK_H  */

