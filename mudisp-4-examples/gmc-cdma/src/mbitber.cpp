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
#include "gsl/gsl_math.h"


//
// Uncomment if you need the bit pair to be dumped verbosely
//
//#define DUMPBITS

//
// Uncomment if you need the errors dumped every ERROR_REPORT_INTERVAL (defined in gmccdma.h)
//
//#define DUMPERRS


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

    stopFlag = ! (maxerrs()==0);

    for (int i=0;i<M();i++) { // user loop

      unsigned int userErrors = 0;

      for (int j=0;j<bpu();j++) { // bit loop  
	
	tt = gsl_matrix_uint_get(&ref,i,j);
	t = gsl_matrix_uint_get(&rec,i,j);
	userErrors += (tt != t);

      } // bit loop 

      gsl_vector_uint_set(bitcount,i,gsl_vector_uint_get(bitcount,i)+bpu());
      gsl_vector_uint_set(errcount,i,gsl_vector_uint_get(errcount,i)+userErrors);

      if (gsl_vector_uint_get(errcount,i)<maxerrs())
    	  stopFlag=false;

    } // user loop 
      
    
    if (framecount ==  ERROR_REPORT_UPDATE_FR) { // report errors

    	// dumperrs <- errocount
      gsl_vector_uint_memcpy(dumperrs,errcount);
      // dumperrs <- dumperrs - lasterrs
      gsl_vector_uint_sub(dumperrs,lasterrs);
      // lasterrs <- errcount
      gsl_vector_uint_memcpy(lasterrs,errcount);

      framecount = 0;

#ifdef DUMPERRS
      for (int i=0;i<M();i++) {
	cout << gsl_vector_uint_get(dumperrs,i) << " errors for user " << i << endl;
      }      
#endif      
    } // end update report errors

    framecount++;

    //////// production of data

    vout1.DeliverDataObj(*dumperrs);
    
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

  unsigned int minimum, maximum, sum;
  minimum = (unsigned int)GSL_POSINF;
  maximum = sum = 0;

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
    // find maximum
    if (gsl_vector_uint_get(errcount,u)>maximum)
        	maximum = gsl_vector_uint_get(errcount,u);
    // find minimum
    if (gsl_vector_uint_get(errcount,u)<minimum)
        	minimum = gsl_vector_uint_get(errcount,u);
    sum += gsl_vector_uint_get(errcount,u);

  } // user loop

  // min, max and mean
  if (gsl_vector_uint_get(bitcount,0)!=0) {
    if (fn != "cout") {
	ofs.width(NUMWIDTH);
	ofs << "min";
	ofs.width(NUMWIDTH);
	ofs << value();
	ofs.width(NUMWIDTH);
	ofs << 1.0*minimum/gsl_vector_uint_get(bitcount,0);
	ofs.width(NUMWIDTH);
	ofs << gsl_vector_uint_get(bitcount,0);
	ofs.width(NUMWIDTH);
	ofs << minimum << endl;

	ofs.width(NUMWIDTH);
	ofs << "max";
	ofs.width(NUMWIDTH);
	ofs << value();
	ofs.width(NUMWIDTH);
	ofs << 1.0*maximum/gsl_vector_uint_get(bitcount,0);
	ofs.width(NUMWIDTH);
	ofs << gsl_vector_uint_get(bitcount,0);
	ofs.width(NUMWIDTH);
	ofs << maximum << endl;

	ofs.width(NUMWIDTH);
	ofs << "mean";
	ofs.width(NUMWIDTH);
	ofs << value();
	ofs.width(NUMWIDTH);
	ofs << 1.0*sum/gsl_vector_uint_get(bitcount,0)/M();
	ofs.width(NUMWIDTH);
	ofs << gsl_vector_uint_get(bitcount,0);
	ofs.width(NUMWIDTH);
	ofs << sum/M() << endl;


    }
    else {
	cout.width(NUMWIDTH);
	cout << "min";
	cout.width(NUMWIDTH);
	cout << value();
	cout.width(NUMWIDTH);
	cout << 1.0*minimum/gsl_vector_uint_get(bitcount,0);
	cout.width(NUMWIDTH);
	cout << gsl_vector_uint_get(bitcount,0);
	cout.width(NUMWIDTH);
	cout << minimum << endl;

	cout.width(NUMWIDTH);
	cout << "max";
	cout.width(NUMWIDTH);
	cout << value();
	cout.width(NUMWIDTH);
	cout << 1.0*maximum/gsl_vector_uint_get(bitcount,0);
	cout.width(NUMWIDTH);
	cout << gsl_vector_uint_get(bitcount,0);
	cout.width(NUMWIDTH);
	cout << maximum << endl;

	cout.width(NUMWIDTH);
	cout << "mean";
	cout.width(NUMWIDTH);
	cout << value();
	cout.width(NUMWIDTH);
	cout << 1.0*sum/gsl_vector_uint_get(bitcount,0)/M();
	cout.width(NUMWIDTH);
	cout << gsl_vector_uint_get(bitcount,0);
	cout.width(NUMWIDTH);
	cout << sum/M() << endl;

    }
  }


  ofs.close();
  
  double ebnol=pow(10.0,(value()/10.0));
  cout << "\n BPSK reference BER (AWGN) = " << gsl_cdf_ugaussian_Q(sqrt(2*ebnol)) << endl;
  
  gsl_vector_uint_free(lasterrs);
  gsl_vector_uint_free(errcount);
  gsl_vector_uint_free(bitcount);
  gsl_vector_uint_free(dumperrs);


}

bool MBitBer::getStopFlag() {
	return stopFlag;
}
