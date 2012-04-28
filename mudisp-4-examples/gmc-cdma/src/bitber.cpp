//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "bitber.h"
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


void BitBer::Setup() {

  //////// initialization of dynamic data structures

  for (int i = 0; i< shift(); i++)
    shreg.push_back(0);

  ignorecount = shift()+ignore();
    
  lasterrs = errcount = bitcount = 0;

  //////// rate declaration for ports
  

}


void BitBer::Run() {

  if (! ignorecount) {
    unsigned int t,tt;

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

    bitcount++;
    errcount += (tt != t);

#ifdef DUMPERRS
    
    if (bitcount % DUMPINTERVAL == 0) {
      cout << errcount-lasterrs << " errors." << endl;
      lasterrs = errcount;
    }

#endif

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

void BitBer::Finish() {

  ofstream ofs;

  string fn( fname() );

  if (fn != "cout")
    ofs.open( fname(),ios::app);

  if (! ofs ) {
    cerr << BlockName << ": error opening "
	 << fn << endl;
    exit(_ERROR_OPEN_FILE_);
  }




  if (bitcount!=0) {
    if (fn != "cout") {
      ofs.width(NUMWIDTH);
      ofs << value();
      ofs.width(NUMWIDTH);
      ofs << 1.0*errcount/bitcount;
      ofs.width(NUMWIDTH);
      ofs << bitcount;
      ofs.width(NUMWIDTH);
      ofs << errcount << endl;
    }
    else {
      cout.width(NUMWIDTH);
      cout << value();
      cout.width(NUMWIDTH);
      cout << 1.0*errcount/bitcount;
      cout.width(NUMWIDTH);
      cout << bitcount;
      cout.width(NUMWIDTH);
      cout << errcount << endl;
    }
  }

  ofs.close();

  double ebnol=pow(10.0,(value()/10.0));
  cout << "\n BPSK reference BER (AWGN) = " << gsl_cdf_ugaussian_Q(sqrt(2*ebnol)) << endl;
  

}
