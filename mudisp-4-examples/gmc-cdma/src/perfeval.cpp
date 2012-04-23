//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "perfeval.h"
#include "gsl/gsl_randist.h"
#include "gsl/gsl_cdf.h"


//
// Uncomment if you need the bit pair to be dumped verbosely
//
//#define DUMPBITS

//
// Uncomment if you need the errors dumped every DUMPINTERVAL
//
//#define DUMPERRS
//#define DUMPINTERVAL 1000


#define NUMWIDTH 15


void PerfEval::Setup() {

  //////// initialization of dynamic data structures

  for (int i = 0; i< shift(); i++)
    shreg.push_back(gsl_vector_uint_class());

  ignorecount = shift()+ignore();
    
  lasterrs = errcount = bitcount = 0;

  //////// rate declaration for ports
  

}


void PerfEval::Run() {

  if (! ignorecount) {

    gsl_vector_uint_class ttt=in1.GetDataObj();
    gsl_vector_uint_class t=in2.GetDataObj();


    shreg.push_back(ttt);
    gsl_vector_uint_class tt(shreg.front());
    shreg.pop_front();
    
//     cout << "ignorecount : " << ignorecount << endl;
//     cout << "received frame 1: " << tt.vec->size << endl;
//     cout << "received frame 2: " << t.vec->size << endl << endl;



    for (unsigned int i=0; i<tt.vec->size; i++) {

      unsigned int btt = gsl_vector_uint_get(tt.vec,i);
      unsigned int bt = gsl_vector_uint_get(t.vec,i);


#ifdef DUMPBITS

    cout << "[ " ;
    if (bt) 
      cout << "x";
    else
      cout << ".";
    cout << " ";
    if (btt) 
      cout << "x";
    else
      cout << ".";

    cout << " ]" << endl;

#endif

    bitcount++;
    errcount += (btt != bt);

    }


#ifdef DUMPERRS
    
    if (bitcount % DUMPINTERVAL == 0) {
      cout << errcount-lasterrs << " errors." << endl;
      lasterrs = errcount;
    }

#endif

    


  } 
  else {
    ignorecount--;

    gsl_vector_uint_class ttt=in1.GetDataObj();
    gsl_vector_uint_class t=in2.GetDataObj();

    shreg.push_back(ttt);
    gsl_vector_uint_class tt(shreg.front());
    shreg.pop_front();

  }

  //////// production of data

  _stop_criteria = (errcount >= errtarget());


}

void PerfEval::Finish() {

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
  cout << "\n BPSK reference BER = " << gsl_cdf_ugaussian_Q(sqrt(2*ebnol)) << endl;
  

}


int PerfEval::StopCriteria() {
  //  cout << "stop criteria: " << _stop_criteria << endl;
  return _stop_criteria;
}

