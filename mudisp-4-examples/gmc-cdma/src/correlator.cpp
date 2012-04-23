//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "correlator.h"

#define MAXNUMLEN 128
#define VERY_BIG_FLOAT 9.9e9999
#define PI 3.1415926536

inline Matrix<complex<double> > Hermitian( Matrix<complex< double> > & a ) {

  unsigned int N,M;

  N=a.num_rows();
  M=a.num_cols();

  Matrix<complex<double> > tmp(a);

  for (int i=0; i<N; i++)
    for (int j=0; j<M; j++) 
      tmp[i][j] = conj(a[i][j]);

  return transpose( tmp );

}


void Correlator::Setup() {

//////// initialization of dynamic data structures

  M=(2*D()+1);
  symbol_time = 0;
  Ns=(1 << Nb());
  
  symbol_arg = complex<double>(0,1)*2.0*PI/Ns;
  
  gray_decoding=Vector <unsigned int>(Ns);

  indim = G()*S()*M;

  wvec = Matrix < complex <double> > (indim,1);
 
//////// rate declaration for ports

  out1.SetRate( Nb() );

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

    wvec[currsample][0] = code[tchip] * pulse1(tsample) / sqrt(G());
    
  }

}


void Correlator::Run() {

  complex <double> ycorr;
  
  Matrix < complex <double> > yvec(1,indim);

  /////////// fetch of sampled input
  yvec = vin1.GetDataObj();
    
  ycorr = (Hermitian(yvec)*wvec)[0][0] ;

  // cout << ycorr << endl;

  //
  // Decision process
  //

  unsigned int Ns=(1 << Nb());
  unsigned int symbol_id=Ns, outbit;
  double mindist=VERY_BIG_FLOAT;

  for (unsigned int sid=0; sid<Ns; sid++){
    if (distance(sid,ycorr)<mindist) {
      symbol_id=sid;
      mindist=distance(sid,ycorr);
    }
  }

  for (int i=0; i<Nb(); i++) {
    outbit=( gray_decoding[symbol_id] >> (Nb()-i-1) ) & 0x1;
    out1.DeliverDataObj( outbit );
  }
}

double Correlator::distance(unsigned int symbol_id, complex<double> ycorr) {
  complex <double> refsym = exp( symbol_arg * symbol_id );
  return abs( refsym - ycorr );
}


void Correlator::Finish() {

//////// post processing

}
