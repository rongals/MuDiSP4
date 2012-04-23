//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "corrlmmse.h"

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

void CorrLMMSE::Setup() {

//////// initialization of dynamic data structures

  M=(2*D()+1);
  symbol_time = count = 0;
  Ns=(1 << Nb());
  
  symbol_arg = complex<double>(0,1)*2.0*PI/Ns;
  
  gray_decoding=Vector <unsigned int>(Ns);

  indim = G()*S()*M;

  svec = Matrix < complex <double> > (indim,1);
  xvec = Matrix < complex <double> > (indim,1);
  wvec = Matrix < complex <double> > (indim,1);
  Fmat = Matrix < complex <double> > (indim,M);

  isLMMSE = false;
 
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

    svec[currsample][0] = code[tchip] * pulse1(tsample) / sqrt(G());
    
  }

  //////// Fmat generation
  
  Index1D I(1,G()*S()), J0(1,1);
  int offset = G()*S();
  
  for (int j=0; j<M; j++) {
    Index1D J(j+1,j+1);
    Fmat(I+offset*j,J) = svec(I+D()*offset,J0);
  }
  
  
}


void CorrLMMSE::Run() {

  complex <double> ycorr, recov;
  
  Matrix < complex <double> > yvec(1,indim);

  /////////// fetch of sampled input
  yvec = vin1.GetDataObj();

  if ( isLMMSE )
    ycorr = (Hermitian(yvec)*wvec)[0][0] ;
  else
    ycorr = (Hermitian(yvec)*(svec+xvec))[0][0] ;

  recov = (Hermitian(yvec)*(yvec))[0][0];


  /////////// canceler update
  
  if (! isLMMSE) {
    xvec = MatGain(xvec,1-mu()*ni()) 
      - MatGain(yvec-Fmat*(transpose(Fmat)*yvec), mu()/recov*ycorr);
  } 

#ifdef DUMPXNRG
  
  if (count % DUMPINTERVAL == 0 ) {
    if (isLMMSE) {
      cout << "<w,w> = " << (Hermitian(wvec)*wvec)[0][0] << endl;
    }
    else
      cout << "<x,x> = " << (Hermitian(xvec)*xvec)[0][0] << endl;
  }
  
#endif

  if ( count++ % LMMSE_SWITCH == 0 ) {
    complex <double> xenergy = (Hermitian(xvec)*xvec)[0][0];
    if ( (! isLMMSE) && ( abs(xenergy) > Th() )) {

#ifdef DUMPXNRG
      cout << "Switched to LMMSE! " << endl;
#endif
      wvec = (svec + xvec);
      isLMMSE = true;
    }
  }

  //
  // Decision process
  //

  unsigned int Ns=(1 << Nb());
  unsigned int symbol_id=Ns, outbit;
  double mindist=VERY_BIG_FLOAT;

  for (unsigned int sid=0; sid<Ns; sid++){
    if (distance(sid,conj(ycorr))<mindist) {
      symbol_id=sid;
      mindist=distance(sid,conj(ycorr));
    }
  }

  //
  // decision directed update of LMMSE vector
  //
  if ( isLMMSE ) {
    complex <double> guess = exp( - symbol_arg * symbol_id );
    wvec = wvec - MatGain(yvec, mu()/recov*(ycorr-guess));
  }    
  
  for (int i=0; i<Nb(); i++) {
    outbit=( gray_decoding[symbol_id] >> (Nb()-i-1) ) & 0x1;
    out1.DeliverDataObj( outbit );
  }
}

double CorrLMMSE::distance(unsigned int symbol_id, complex<double> ycorr) {
  complex <double> refsym = exp( symbol_arg * symbol_id );
  return abs( refsym - ycorr );
}


void CorrLMMSE::Finish() {

//////// post processing

}
