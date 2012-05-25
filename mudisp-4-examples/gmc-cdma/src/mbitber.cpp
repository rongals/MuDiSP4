//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "mbitber.h"
#include "gsl/gsl_randist.h"
#include "gsl/gsl_cdf.h"


//
// Uncomment if you need the bit pair to be dumped verbosely
//
//#define DUMPBITS

//
// Uncomment if you need the errors dumped every DUMPINTERVAL
//
#define DUMPERRS
#define DUMPINTERVAL 1000


#define NUMWIDTH 15


void MBitBer::Setup() {

  //////// initialization of dynamic data structures

  for (int i = 0; i< shift(); i++)
    shreg.push_back(0);

  ignorecount = shift()+ignore();
    
  lasterrs = gsl_vector_uint_calloc(M());
  errcount = gsl_vector_uint_calloc(M());
  bitcount = gsl_vector_uint_calloc(M());

  framepos = 0;
  framelenght = M()*bpu();

  //////// rate declaration for ports
  

}


void MBitBer::Run() {

  if (! ignorecount) {
    unsigned int t,tt;

    if (framepos == framelenght)
      framepos = 0;

    tt=in1.GetDataObj();
    t=in2.GetDataObj();

    shreg.push_back(tt);
    tt=shreg.front();
    shreg.pop_front();

#ifdef DUMPBITS

    cout << "[ " ;
    if (t) 
      cout << "x";
    else
      cout << ".";
    cout << " ";
    if (tt) 
      cout << "x";
    else
      cout << ".";

    cout << " ]" << endl;

#endif

    int curruser = int(framepos/bpu());
    gsl_vector_uint_set(bitcount,curruser,gsl_vector_uint_get(bitcount,curruser)+1);
    gsl_vector_uint_set(errcount,curruser,gsl_vector_uint_get(errcount,curruser)+(tt != t));

#ifdef DUMPERRS
    
    if (gsl_vector_uint_get(bitcount,curruser) % DUMPINTERVAL == 0) {
      cout << gsl_vector_uint_get(errcount,curruser)-gsl_vector_uint_get(lasterrs,curruser) << " errors for user " << curruser << endl;
      gsl_vector_uint_set(lasterrs,curruser,gsl_vector_uint_get(errcount,curruser));
    }

#endif

    framepos++;

  } 
  else {
    ignorecount--;

    unsigned int t,tt;
    tt=in1.GetDataObj();
    t=in2.GetDataObj();
    shreg.push_back(tt);
    tt=shreg.front();
    shreg.pop_front();

  }

  //////// production of data

}

void MBitBer::Finish() {

  ofstream ofs;

  string fn( fname() );

  if (fn != "cout")
    ofs.open( fname(),ios::app);

  if (! ofs ) {
    cerr << BlockName << ": error opening "
	 << fn << endl;
    exit(_ERROR_OPEN_FILE_);
  }

  for (int u=0;u<M();u++) { // user loop

    if (gsl_vector_uint_get(bitcount,u)!=0) {
      if (fn != "cout") {
	ofs.width(NUMWIDTH);
	ofs << u;
	ofs.width(NUMWIDTH);
	ofs << value();
	ofs.width(NUMWIDTH);
	ofs << 1.0*gsl_vector_uint_get(errcount,u)/gsl_vector_uint_get(bitcount,u);
	ofs.width(NUMWIDTH);
	ofs << gsl_vector_uint_get(bitcount,u);
	ofs.width(NUMWIDTH);
	ofs << gsl_vector_uint_get(errcount,u) << endl;
      }
      else {
	cout.width(NUMWIDTH);
	cout << u;
	cout.width(NUMWIDTH);
	cout << value();
	cout.width(NUMWIDTH);
	cout << 1.0*gsl_vector_uint_get(errcount,u)/gsl_vector_uint_get(bitcount,u);
	cout.width(NUMWIDTH);
	cout << gsl_vector_uint_get(bitcount,u);
	cout.width(NUMWIDTH);
	cout << gsl_vector_uint_get(errcount,u) << endl;
      }
    }
  } // user loop
  ofs.close();
  
  double ebnol=pow(10.0,(value()/10.0));
  cout << "\n BPSK reference BER (AWGN) = " << gsl_cdf_ugaussian_Q(sqrt(2*ebnol)) << endl;
  

}
