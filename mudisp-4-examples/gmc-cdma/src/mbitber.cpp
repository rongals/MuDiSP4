//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "mbitber.h"
#include "gmccdma.h"
#include "gsl/gsl_randist.h"
#include "gsl/gsl_cdf.h"


//
// Uncomment if you need the bit pair to be dumped verbosely
//
//#define DUMPBITS

//
// Uncomment if you need the errors dumped every ERROR_REPORT_INTERVAL (defined in gmccdma.h)
//
#define DUMPERRS


#define NUMWIDTH 15


void MBitBer::Setup() {

  //////// initialization of dynamic data structures

  lasterrs = gsl_vector_uint_calloc(M());
  errcount = gsl_vector_uint_calloc(M());
  bitcount = gsl_vector_uint_calloc(M());
  dumperrs = gsl_vector_uint_calloc(M());

  //////// rate declaration for ports



}


void MBitBer::Run() {

    unsigned int t,tt;

    gsl_matrix_uint ref = min1.GetDataObj();
    gsl_matrix_uint rec = min2.GetDataObj();


    for (int i=0;i<M();i++) { // user loop

      unsigned int userErrors = 0;

      for (int j=0;j<bpu();j++) { // bit loop  
	
	tt = gsl_matrix_uint_get(&ref,i,j);
	t = gsl_matrix_uint_get(&rec,i,j);
	userErrors += (tt != t);

      } // bit loop 

      gsl_vector_uint_set(bitcount,i,gsl_vector_uint_get(bitcount,i)+bpu());
      gsl_vector_uint_set(errcount,i,gsl_vector_uint_get(errcount,i)+userErrors);
    } // user loop 
      
    
    if (framecount ==  ERROR_REPORT_INTERVAL) { // report errors

      gsl_vector_uint_memcpy(dumperrs,errcount);
      gsl_vector_uint_sub(dumperrs,lasterrs);
      gsl_vector_uint_memcpy(lasterrs,errcount);

      framecount = 0;

#ifdef DUMPERRS
      for (int i=0;i<M();i++) {
	cout << gsl_vector_uint_get(dumperrs,i) << " errors for user " << i << endl;
      }      
#endif      
      vout1.DeliverDataObj(*dumperrs); //this produces an output every ERROR_REPORT_INTERVAL frames
      cout << "Producing error report..done." << endl;      
    } // end report errors

    framecount++;

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
  
  gsl_vector_uint_free(lasterrs);
  gsl_vector_uint_free(errcount);
  gsl_vector_uint_free(bitcount);
  gsl_vector_uint_free(dumperrs);


}