//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "mcpmpcoeffs.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_matrix.h"
#include "gmccdma.h"
#include "gsl/gsl_randist.h"

extern void gsl_matrix_complex_show(gsl_matrix_complex *);

void MCPMPCoeffs::Setup() {

  //////// initialization of dynamic data structures

  gsl_rng_env_setup();
  cout << BlockName << ".gsl_rng_default_seed=" << gsl_rng_default_seed << endl;

  // ch : channel coeffs matrix h(n) (M**2xN)
  //                               ij 
  // ch matrix structure
  //
  //   +-                 -+
  //   | h(0) . . . . h(n) | |
  //   |  11           11  | |
  //   |                   | | Rx1
  //   | h(0) . . . . h(n) | |
  //   |  12           12  | |
  //   |                   |
  //   | h(0) . . . . h(n) | |
  //   |  21           21  | |
  //   |                   | | Rx2
  //   | h(0) . . . . h(n) | |
  //   |  22           22  | |
  //   +-                 -+
  // 
  //   where h(n) represents the channel impulse response
  //          ij
  //
  //   at time n, from tx i to rx j
  //   the matrix has MxM rows and N comumns.
  //   The (i,j) channel is locater at row i*M+j
  //   with i,j in the range [0,M-1] and rows counting from 0
  //
  //


  _M = M();
  ch = gsl_matrix_complex_alloc(_M*_M,N());
  ran = gsl_rng_alloc( gsl_rng_default );

  double pwr=0.0;
  for (int j=0; j<L(); j++) {
    pwr += exp(-j*2.0/PTau());
  }
  
  gain=sqrt(1.0/(pwr*(Ricean()+1.0)));
  gainrice=sqrt(Ricean()/(Ricean()+1.0));


  cout << "Sum(taps^2)=" << pwr << endl
       << "gain=" << gain << endl
       << "gainrice=" << gainrice << endl;

  //
  // GEO
  //
  string geofn(GeoFn());
  if (GeoFn()=="none") // check if GEO desired
    geoEnabled = 0;
  else { // GEO rendering enabled
       geoEnabled = 1;
    GeoInit();
    GeoRender();
  }
  
  
  //////// rate declaration for ports


}


void MCPMPCoeffs::Run() {

  
  gsl_complex z = gsl_complex_rect(0,0);
  gsl_complex o = gsl_complex_rect(1,0);

  //
  //
  // produce the idiot channel matrix
  //
  //
  //   for (int i=0; i<USERS; i++) {
  //     for (int j=0; j<N(); j++) {
  //       if (j==0)
  // 	gsl_matrix_complex_set(ch,i,j,o);
  //       else if (j==1)
  // 	gsl_matrix_complex_set(ch,i,j,gsl_complex_polar(0.0,M_PI/3.0));
  //       else
  // 	gsl_matrix_complex_set(ch,i,j,z);

  //     }
  //   }



  //
  // Exponentially decaying power profile
  //
  //
  gsl_complex chcoeff;
  gsl_matrix_complex_set_all(ch,z);
  for (int i=0; i<_M; i++) { // user i (tx)
    for (int ii=0;ii<_M;ii++) { // user ii (rx)
      for (int j=0; j<L(); j++) { // tap j 
	double coeffstd=gain*exp(-j/PTau())/sqrt(2.0);
	if (j==0) {  // if this is the first tab 
	  chcoeff = gsl_complex_rect( gsl_ran_gaussian(ran,coeffstd)+gainrice,
	   			      gsl_ran_gaussian(ran,coeffstd));
	  //chcoeff = o;
	  
	} else { // this is not the first tap
	  chcoeff = gsl_complex_rect( gsl_ran_gaussian(ran,coeffstd),
				      gsl_ran_gaussian(ran,coeffstd));
	  //chcoeff = z;

	} // if

	gsl_matrix_complex_set(ch,i*_M+ii,j,chcoeff);	

      } // j loop
    } // ii loop
  } // i loop
  
  // cout << "channel:" << endl;
  // gsl_matrix_complex_show(ch);

 
  //////// production of data
  mout1.DeliverDataObj( *ch );

}

void MCPMPCoeffs::Finish() {
  gsl_matrix_complex_free(ch);
  gsl_rng_free( ran );
  if (geoEnabled) {
    ofs.close();
    gsl_vector_complex_free(geoPositions);
  }
}

void MCPMPCoeffs::GeoInit() {

  // initial geo positions

  geoPositions = gsl_vector_complex_alloc(M());
  for (int i=0;i<M();i++) {
    double lat=gsl_ran_flat(ran,
			     GEO_AREA_CENTER_LAT-GEO_AREA_SIZE/2.0,
			     GEO_AREA_CENTER_LAT+GEO_AREA_SIZE/2.0);
    double lon=gsl_ran_flat(ran,
			     GEO_AREA_CENTER_LON-GEO_AREA_SIZE/2.0,
			     GEO_AREA_CENTER_LON+GEO_AREA_SIZE/2.0);
    gsl_vector_complex_set(geoPositions,i,gsl_complex_rect(lat,lon));

  }

  // header and footer of kml section
  kmlhead << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl
	  << "<kml xmlns=\"http://earth.google.com/kml/2.0\">" << endl;

  kmlheadend << "</kml>" << endl;
  
}



void MCPMPCoeffs::GeoUpdate(double seconds) {
}



void MCPMPCoeffs::GeoRender() {

  kmlobject.seekp(ios_base::beg);

  for (int i=0;i<1;i++) { 

    gsl_complex pos = gsl_vector_complex_get(geoPositions,i);

    kmlobject.precision(10);
    kmlobject << "\t<Placemark>" << endl
	      << "\t\t<name>User " << i << "</name>" << endl
	      << "\t\t<description>^</description>" << endl
	      << "\t<LookAt>" << endl
	      << "\t\t<longitude>" << GEO_AREA_CENTER_LON << "</longitude>" << endl
	      << "\t\t<latitude>"<< GEO_AREA_CENTER_LAT << "</latitude>" << endl
	      << "\t\t<range>" << 1000 << "</range>" << endl
	      << "\t\t<tilt>" << 0 << "</tilt>" << endl
	      << "\t\t<heading>" << 0 << "</heading>" << endl
	      << "\t</LookAt>" << endl
	      << "\t\t<Point>" << endl
	      << "\t\t\t<coordinates>" << GSL_IMAG(pos) << "," << GSL_REAL(pos) << "," << 0 << "</coordinates>" << endl
	      << "\t\t</Point>" << endl
	      << "\t</Placemark>" << endl;
  }

  ofs.open(GeoFn());  
  if (! ofs ) {
    cerr << BlockName << ": error opening "
	 << GeoFn() << endl;
    exit(_ERROR_OPEN_FILE_);
  }

  ofs << kmlhead.str() << kmlobject.str() << kmlheadend.str();
  ofs.close();

}

