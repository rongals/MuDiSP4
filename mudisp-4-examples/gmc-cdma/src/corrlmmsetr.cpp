//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "corrlmmsetr.h"

#define MAXNUMLEN 128
#define VERY_BIG_FLOAT 9.9e9999
#define PI 3.1415926536

//
// Uncomment for X energy dumps
//
#define DUMPXNRG


#define DUMPINTERVAL 1000
#define LMMSE_SWITCH 2000

inline Matrix<complex<double> > Hermitian(const Matrix<complex< double> > & a ) {
  unsigned int N,M;

  N=a.num_rows();
  M=a.num_cols();

  Matrix<complex<double> > tmp(a);

  for (int i=0; i<N; i++)
    for (int j=0; j<M; j++) 
      tmp[i][j] = conj(a[i][j]);

  return transpose( tmp );

}

inline Matrix<complex<double> > MatGain(const Matrix<complex< double> > & a , 
				        const complex <double > & gain) {

  unsigned int N,M;

  N=a.num_rows();
  M=a.num_cols();

  Matrix<complex<double> > tmp(a);

  for (int i=0; i<N; i++)
    for (int j=0; j<M; j++) 
      tmp[i][j] = gain * a[i][j];

  return tmp;

}

void CorrLMMSETr::Setup() {

//////// initialization of dynamic data structures

  M=(2*D()+1);
  symbol_time = count = 0;
  Ns=(1 << Nb());
  
  symbol_arg = complex<double>(0,1)*2.0*PI/Ns;
  
  gray_decoding=Vector <unsigned int>(Ns);
  gray_encoding=Vector <unsigned int>(Ns);

  indim = G()*S()*M;

  wvec = Matrix < complex <double> > (indim,1);

  isTrained = true;

  for (int i = 0; i< shift(); i++)
    shreg.push_back(0);
  
 
//////// rate declaration for ports

  out1.SetRate( Nb() );
  in2.SetRate( Nb() );

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
  


  /////////// Gray Demapping

  for (unsigned int i=0; i<Ns; i++) {
    unsigned int tmp=0;
    for (unsigned int k=0; k<Nb(); k++) {
      unsigned int t=(1<<k), tt=2*t;
      tmp += t * (((t+i)/tt) % 2);
    }
    gray_decoding[tmp]=i;
  }

  ////////// Pulse initialization

  pulse1=TxPulse(S());


  ////////// correlator initialization

  for (register int i=0; i< G()*S(); i++){

    unsigned int isymb, tsymb, tchip, ichip, tsample, currsample;
        
    currsample = i+D()*G()*S();

    // position in samples from the start of the chip
    tsample = currsample % S();

    // position in chips
    ichip = currsample / S();

    // position in chips from the start of the symbol 
    tchip = ichip % G(); 

    //wvec[currsample][0] = code[tchip] * pulse1(tsample) / sqrt(G());
    
  }

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


void CorrLMMSETr::Run() {

  complex <double> ycorr, ynrg;
  
  Matrix < complex <double> > yvec(1,indim);

  /////////// fetch of sampled input
  yvec = vin1.GetDataObj();


  /////////// generation of training data
  // number of symbols in PSK constellation
  unsigned int Ns=(1 << Nb());
  unsigned int symbol_id=0, outbit;
  double mindist=VERY_BIG_FLOAT;

  
  ////// I take Nb bits from input and map it in new_symbol
  
  for (int i=0;i<Nb();i++) {
    symbol_id = (symbol_id << 1);
    symbol_id += in2.GetDataObj();
  }

  shreg.push_back(symbol_id);
  symbol_id=shreg.front();
  shreg.pop_front();

  true_symbol = exp( symbol_arg * gray_encoding[symbol_id] );

  ycorr = (Hermitian(wvec)*yvec)[0][0] ;
  
  ynrg = (Hermitian(yvec)*(yvec))[0][0];

  //
  // Training switch
  //
  if (count >= Trs() &&  isTrained ) {
    isTrained = false;
#ifdef DUMPXNRG
    cout << "\n\nLMMSE Training Completed\n\n" << endl;
#endif
  }


#ifdef DUMPXNRG
  if (count % DUMPINTERVAL == 0 && isTrained) {
    cout << "<w,w> = " << (Hermitian(wvec)*wvec)[0][0]
	 << "\t |e|  = " << abs( true_symbol-ycorr ) << endl;

  }
#endif

  //
  // LMMSE update of the receiver
  //
  if ( isTrained && (count++ > shift()) ) {
    wvec = wvec - MatGain(yvec, mu()/ynrg*(ycorr-true_symbol));
//     cout << "ycorr       = " << ycorr 
// 	 << "\ntrue_symbol = " << true_symbol 
// 	 << "\ndiff        = " << ycorr-true_symbol << endl << endl;
  }



  //
  // Decision Process
  //
  if ( ! isTrained ) {
    symbol_id=Ns;
    for (unsigned int sid=0; sid<Ns; sid++){
      if (distance(sid,ycorr)<mindist) {
	symbol_id=sid;
	mindist=distance(sid,ycorr);
      }
    }
  }

  for (int i=0; i<Nb(); i++) {
    outbit=( gray_decoding[symbol_id] >> (Nb()-i-1) ) & 0x1;
    out1.DeliverDataObj( outbit );
  }
}

double CorrLMMSETr::distance(unsigned int symbol_id, complex<double> ycorr) {
  complex <double> refsym = exp( symbol_arg * symbol_id );
  return abs( refsym - ycorr );
}


void CorrLMMSETr::Finish() {

//////// post processing

}
