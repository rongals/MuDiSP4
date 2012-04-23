//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#ifndef __MUDISP2_CORRELATOR_H 
#define __MUDISP2_CORRELATOR_H

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


class Correlator : public Block {
private:

////////   Parameters instances

  IntParam D,G,S,Nb,Cd;
  FloatParam A;
  StringParam seqfname;

////////   Local Attributes

  unsigned int symbol_time, M, Ns, indim;
  complex <double> symbol_arg;
  Vector < double > code;
  Matrix < complex <double> > wvec; 
  Vector < unsigned int > gray_decoding;
  TxPulse pulse1;


  ////////   distance
  double Correlator::distance(unsigned int, complex<double>);

public:

////////   InPorts and OutPorts

  OutPort < unsigned int > out1;
  InPort < Matrix < complex <double> > > vin1;  


////////   constructor


  Correlator():Block("Correlator")
    
////////  parameters initializazion

    ,D("SemiWin",1,"half the observation window")
    ,G("SpreadFact",63,"spreading factor")
    ,S("ChipRes",1,"samples per chip")
    ,Nb("BitSymb",1,"bits per symbol (PSK mapping)")
    ,Cd("CodeNum",1,"channel number (code)")
    ,seqfname("SeqFName","sequences/gold_63.dat","sequences matrix filename")

    {

//////// local parameter registration

      AddParameter(D);
      AddParameter(G);
      AddParameter(S);
      AddParameter(Nb);
      AddParameter(Cd);
      AddParameter(seqfname);

    }

  void Setup();
  void Run();
  void Finish();
  
};


#endif /* __MUDISP_MYBLOCK_H  */

