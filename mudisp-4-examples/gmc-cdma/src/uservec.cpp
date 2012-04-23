//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "uservec.h"
#include "rndgen.h"

#define MAXNUMLEN 128
#define PI 3.1415926536

void UserVec::Setup() {

//////// initialization of dynamic data structures

  M=(2*D()+1);
  symbol_time = 0;
  Ns=(1 << Nb());

  symbol_arg = complex<double>(0,1)*2.0*double(PI/Ns);

  symbol_hist=Vector<complex < double > >(4*D()+3);
  
  gray_encoding=Vector <unsigned int>(Ns);

  outdim = G()*S()*M;

  txgain = sqrt(pow(10,A()/10.0)) / sqrt(double(G()));

  phshift = complex<double>(0,0);

  cout << BlockName << " delay = " << Dl() << endl;

  if (Foffs()!=0)
    phincr = complex <double> (0.0, 2*PI/Foffs());
  else
    phincr = complex <double> (0.0,0.0);

  count=0;

//////// rate declaration for ports

 in1.SetRate( Nb() );

//////// sequences loading

  ifstream ifs;

  ifs.open( seqfname() );
  if (! ifs ) {
    cerr << BlockName << ": error opening "
	 << seqfname() << endl;
    exit(_ERROR_OPEN_FILE_);
  }

  Matrix < double > gm;

  ifs >> gm;
  ifs.close();

  code = Vector< double >(G());
  
  for (int i=1; i<G()+1; i++)
    code(i)=gm(Cd(),i);
  
//////// parameters checks

  if (abs(Dl())>((D()+1)*S()*G())) {
    cerr << BlockName << ": error : delay too large " << endl;
    exit(1);
  }

///////// Pulse initialization

  pulse1 = TxPulse(S());

///////// Gray Encoder SetUp

  for (unsigned int i=0; i<Ns; i++) {
    gray_encoding[i]=0;
    for (unsigned int k=0; k<Nb(); k++) {
      unsigned int t=(1<<k);
      unsigned int tt=2*t;
      gray_encoding[i] += t * (((t+i)/tt) % 2);
    }
  }
}


void UserVec::Run() {

  // number of symbols in PSK constellation
  unsigned int Ns=(1 << Nb());
  unsigned int symbol_id=0;

//////// I take Nb bits from input and map it in new_symbol

  for (int i=0;i<Nb();i++) {
    symbol_id = (symbol_id << 1);
    symbol_id += in1.GetDataObj();

  }


  new_symbol = exp( symbol_arg * double(gray_encoding[symbol_id]) + phshift );

  phshift += phincr;

  //   cout << "New Symbol ID = " << symbol_id << endl;
  //  cout << "New Symbol = " << new_symbol << endl;
  
//////// update of symbol history

  int hist_size = 4*D()+3;

  for (register int i=0; i<hist_size-1; i++) {
    symbol_hist[i]=symbol_hist[i+1];
  }
  symbol_hist[hist_size-1]=new_symbol;


//////// production of spreading vector

  Matrix < complex <double> > tmp(outdim,1); 

  for (register int i=0; i< G()*S()*M; i++){

    unsigned int isymb, tsymb, tchip, ichip, tsample, currsample;
        
    currsample = i-Dl()+(1+D())*G()*S();

    // position in samples from the start of the chip
    tsample = currsample % S();

    // position in chips
    ichip = currsample / S();

    // position in chips from the start of the symbol 
    tchip = ichip % G(); 

    // position in symbol
    tsymb = ichip / G();

    tmp[i][0] = txgain * 
      symbol_hist[tsymb] * 
      code[tchip] * 
      pulse1(tsample);

  }

  vout1.DeliverDataObj(tmp);

}


void UserVec::Finish() {

//////// post processing

}
