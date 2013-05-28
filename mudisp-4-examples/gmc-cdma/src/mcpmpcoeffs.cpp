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
#include "gsl/gsl_sf_trig.h"
#include "gsl/gsl_sf_exp.h"
#include "gsl/gsl_sf_log.h"
#include "gsl/gsl_math.h"

//
//
// Matrix display
//
//
void gsl_matrix_show(gsl_matrix *mat) {
  unsigned int N=mat->size1, M=mat->size2;
  for (int i=0; i<N; i++) {
    for (int j=0; j<M; j++) {
      double el = gsl_matrix_get(mat,i,j);
      cout << setw(15)
	   << el;
    }
    cout << endl;
  }
  cout << endl;
}


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
  runCount = 0;

  // path loss matrix
  pathLoss = gsl_matrix_alloc(_M,_M);
  gsl_matrix_set_all(pathLoss,1.0);

  double pwr=0.0;
  for (int j=0; j<L(); j++) {
    pwr += exp(-j*2.0/PTau());
  }
  
  gain=sqrt(1.0/(pwr*(Ricean()+1.0)));
  gainrice=sqrt(Ricean()/(Ricean()+1.0));


  // cout << "Sum(taps^2)=" << pwr << endl
  //      << "gain=" << gain << endl
  //      << "gainrice=" << gainrice << endl;

  //
  // GEO
  //

  // geo initialization
  GeoInit();

  // check if rendering is desired
  string geofn(GeoFn());
  if (GeoFn()=="none") // check if GEO rendering is desired
    geoRenderEnabled = 0;
  else { // GEO rendering enabled
    geoRenderEnabled = 1;
    GeoRender();
  }

  //
  // SOS Spatial Channel Model
  //
  SOSN = 2.0 * SOSM() * SOSM();
  sosc = sqrt(2.0/SOSN);

  sosfrn = gsl_vector_alloc(SOSM()+2);
  sosfxn = gsl_vector_alloc(SOSN);
  sosfyn = gsl_vector_alloc(SOSN);


  //
  // we generate the radial spatial frequency
  //
  gsl_vector_set(sosfrn,0,0.0);

  for (int i=0;i<SOSM()+1;i++) {
	  double old = gsl_vector_get(sosfrn,i);
	  double tmp1 = 1.0 / sqrt(SOSa()*SOSa() + 4.0 * M_PI * M_PI * old * old ) - SOSP() / (SOSM() * SOSa());
	  double nextval = 1.0 / (2.0 * M_PI) * sqrt( 1.0 / (tmp1 * tmp1) - SOSa() * SOSa() );
	  gsl_vector_set(sosfrn,i+1,nextval);

	  //cout << "fr_" << i+1 << " = " << nextval << endl;
  }

  //
  // we compute the cartesian spatial frequency
  //
  for (int i=0; i<SOSM();i++) { // radius values
	  for(int j=0; j<2*SOSM(); j++) { // angle values
		  double phi = M_PI * (2 * j - 2 * SOSM() +1) / (4 * SOSM());
		  double frm = gsl_vector_get(sosfrn,i+1);
		  double fxn = frm * gsl_sf_cos(phi);
		  double fyn = frm * gsl_sf_sin(phi);

		  gsl_vector_set(sosfxn,j+i*2*(SOSM()),fxn);
		  gsl_vector_set(sosfyn,j+i*2*(SOSM()),fyn);


		  //cout << "fx_" << i << "," << j << " = " << fxn << ", " << fyn << endl;

	  }
  }

  cout << "Spatial frequencies generated." << endl;

  //
  // now we generate the random phases theta_n for each link tx->rx (M x SOSN )
  //
  sostheta = gsl_matrix_alloc(M()*M(),SOSN);
  for (int i=0; i<_M; i++) { // user i (tx)
	  for (int ii=0;ii<_M;ii++) { // user ii (rx)
		  for (int j=0; j<SOSN; j++ ) { // phase of spatial frequency j
			gsl_matrix_set(sostheta,i*_M+ii,j,gsl_ran_flat(ran,0,2.0*M_PI));
		  }
	  }
  }

  cout << "Spatial phases generated." << endl;


  //
  // Spatial Channel (first) Update
  //
  SpatialChannelUpdate();

  //  ____   ___  _   _  ___     ___  _   _ ___ _
  // / ___| / _ \| \ | |/ _ \   / _ \| | | |_ _| |
  // \___ \| | | |  \| | | | | | | | | | | || || |
  //  ___) | |_| | |\  | |_| | | |_| | |_| || ||_|
  // |____/ \___/|_| \_|\___/   \__\_\\___/|___(_)
  //



  //////// rate declaration for ports


}


void MCPMPCoeffs::Run() {


	gsl_complex z = gsl_complex_rect(0,0);
	gsl_complex o = gsl_complex_rect(1,0);



	//
	// Update Positions
	//
	if (runCount++ % GEO_UPDATE_INTERVAL == 0) {

		GeoUpdate(5.0); // each GEO_UPDATE_INTERVAL runs are equivalent to x seconds
		SpatialChannelUpdate();

		if (geoRenderEnabled)
			GeoRender();
		cout << "Updating node positions." << endl;

		//gsl_matrix_show(pathLoss);


		//
		//
		// CHANNEL MODEL BASED ON
		// CAI AND GIANNAKIS: 2D CHANNEL SIMULATION MODEL FOR SHADOWING PROCESSES
		// IEEE Trans Veh Tech Vol 52, No. 6, Nov. 2003
		//
		//




		//
		// Exponentially decaying power profile
		//
		//
		gsl_matrix_complex_set_all(ch,z);
		gsl_complex chcoeff;

		for (int i=0; i<_M; i++) { // user i (tx)
			for (int ii=0;ii<_M;ii++) { // user ii (rx)
				double plgain = gain * gsl_matrix_get(pathLoss,i,ii);
				for (int j=0; j<L(); j++) { // tap j
					double coeffstd=plgain*exp(-j/PTau())/sqrt(2.0);
					if (j==0) {  // if this is the first tab
						chcoeff = gsl_complex_rect( gsl_ran_gaussian(ran,coeffstd) + coeffstd * gainrice,
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


	}

		cout << "channel:" << endl;
		gsl_matrix_complex_show(ch);


	//////// production of data
	mout1.DeliverDataObj( *ch );

}

void MCPMPCoeffs::Finish() {
  gsl_matrix_complex_free(ch);
  gsl_matrix_free(pathLoss);
  gsl_rng_free( ran );
  if (geoRenderEnabled) {
    ofs.close();
  }
  gsl_vector_complex_free(geoPositions);
  gsl_vector_complex_free(geoVelocities);
  gsl_vector_free(sosfrn);
  gsl_vector_free(sosfxn);
  gsl_vector_free(sosfyn);
  gsl_matrix_free(sostheta);
}



void MCPMPCoeffs::GeoInit() {

  // initial geo positions

  geoPositions = gsl_vector_complex_alloc(2*M());
  geoVelocities  = gsl_vector_complex_alloc(2*M());


  // tx[i]
  for (int i=0;i<M();i++) {
    double lat=gsl_ran_flat(ran,
			     GEO_AREA_CENTER_LAT-GEO_AREA_SIZE/2.0,
			     GEO_AREA_CENTER_LAT+GEO_AREA_SIZE/2.0);
    double lon=gsl_ran_flat(ran,
			     GEO_AREA_CENTER_LON-GEO_AREA_SIZE/2.0,
			     GEO_AREA_CENTER_LON+GEO_AREA_SIZE/2.0);
    double velmod=gsl_ran_flat(ran,
			     GEO_VELOCITY_MIN,
			     GEO_VELOCITY_MAX);
    double veldir=gsl_ran_flat(ran,
			       0.0,
			       2*M_PI);

    gsl_complex vel = gsl_complex_polar(velmod,veldir);

    // vx (km/h), vy (km/h)
    // deltalon (deg) = vx / 3.6 / 111111 Cos[lat]
    // deltalat (deg) = vy / 3.6 / 111111 
    
    double deltalon = GSL_REAL(vel) / 3.6 / 111111.0 / cos(GEO_AREA_CENTER_LAT / 180.0 * M_PI);
    double deltalat = GSL_IMAG(vel) / 3.6 / 111111.0;

    gsl_vector_complex_set(geoPositions,i,gsl_complex_rect(lat,lon));
    gsl_vector_complex_set(geoVelocities,i,gsl_complex_rect(deltalon,deltalat));

  }

  // rx[i] same position zero velocity 
  for (int i=M();i<2*M();i++) {
    double lat=gsl_ran_flat(ran,
			     GEO_AREA_CENTER_LAT-GEO_AREA_SIZE/2.0,
			     GEO_AREA_CENTER_LAT+GEO_AREA_SIZE/2.0);
    double lon=gsl_ran_flat(ran,
			     GEO_AREA_CENTER_LON-GEO_AREA_SIZE/2.0,
			     GEO_AREA_CENTER_LON+GEO_AREA_SIZE/2.0);
    double velmod=gsl_ran_flat(ran,
			     GEO_VELOCITY_MIN,
			     GEO_VELOCITY_MAX);
    double veldir=gsl_ran_flat(ran,
			       0.0,
			       2*M_PI);

    gsl_complex vel = gsl_complex_polar(velmod,veldir);

    // vx (km/h), vy (km/h)
    // deltalon (deg) = vx / 3.6 / 111111 Cos[lat]
    // deltalat (deg) = vy / 3.6 / 111111 
    
    double deltalon = GSL_REAL(vel) / 3.6 / 111111.0 / cos(GEO_AREA_CENTER_LAT / 180.0 * M_PI);
    double deltalat = GSL_IMAG(vel) / 3.6 / 111111.0;

    gsl_vector_complex_set(geoPositions,i,gsl_complex_rect(GEO_AREA_CENTER_LAT,GEO_AREA_CENTER_LON));
    gsl_vector_complex_set(geoVelocities,i,gsl_complex_polar(0.0,0.0));

  }



  // header and footer of kml section
  kmlhead << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl
	  << "<kml xmlns=\"http://earth.google.com/kml/2.0\">" << endl
	  << "<Document>" << endl
	  << "<name>MuDiSP User locations</name>" << endl
	  << "<Style id=\"greenIcon\">" << endl
	  << "\t<IconStyle>" << endl
	  << "\t\t<scale>2.0</scale>" << endl
	  << "\t\t<Icon>" << endl 
	  << "\t\t\t<href>http://maps.google.com/mapfiles/ms/icons/yellow-dot.png</href>" << endl
	  << "\t\t</Icon>" << endl
	  << "\t</IconStyle>" << endl
	  << "</Style>" << endl
	  << "<Style id=\"blueIcon\">" << endl
	  << "\t<IconStyle>" << endl
	  << "\t\t<scale>2.0</scale>" << endl
	  << "\t\t<Icon>" << endl 
	  << "\t\t\t<href>http://maps.google.com/mapfiles/ms/icons/red-dot.png</href>" << endl
	  << "\t\t</Icon>" << endl
	  << "\t</IconStyle>" << endl
	  << "</Style>" << endl;


  kmlheadend << "</Document>" << endl
	     << "</kml>" << endl;

}



void MCPMPCoeffs::GeoUpdate(double seconds) {

  for (int i=0;i<2*M();i++) {



    gsl_complex v = gsl_vector_complex_get(geoVelocities,i); // expressed in deltalon/s,deltalat/s
    gsl_complex p = gsl_vector_complex_get(geoPositions,i); // expressed in lon,lat
    gsl_complex np = gsl_complex_add(p,gsl_complex_mul_real(v, seconds));

    gsl_vector_complex_set(geoPositions,i,np);

 //   cout << "node " << i << " position = " << GSL_IMAG(np) << ", " << GSL_REAL(np) << endl;

  }

  

}



void MCPMPCoeffs::GeoRender() {

  ostringstream kmltmp;

  //  kmlobject.seekp(ios_base::beg);

  for (int i=0;i<M();i++) { // tx loop

    gsl_complex pos = gsl_vector_complex_get(geoPositions,i);

    kmltmp.precision(10);
    kmltmp << "\t<Placemark>" << endl
	      << "\t\t<name>Tx_" << i << "</name>" << endl
	      << "\t\t<description>^</description>" << endl
	      << "\t\t<styleUrl>#greenIcon</styleUrl>" << endl
	      << "\t\t<Point>" << endl
	      << "\t\t\t<coordinates>" << GSL_IMAG(pos) << ","
	      << GSL_REAL(pos) << ","
	      << 0 << "</coordinates>" << endl
	      << "\t\t</Point>" << endl
	      << "\t</Placemark>" << endl;
  }

  for (int i=0;i<M();i++) { // rx loop
    
    gsl_complex pos = gsl_vector_complex_get(geoPositions,i+_M);
    
    kmltmp.precision(10);
    kmltmp << "\t<Placemark>" << endl
	      << "\t\t<name>Rx_" << i << "</name>" << endl
	      << "\t\t<description>^</description>" << endl
	      << "\t\t<styleUrl>#blueIcon</styleUrl>" << endl      
	      << "\t\t<Point>" << endl
	      << "\t\t\t<coordinates>" << GSL_IMAG(pos) << "," 
	      << GSL_REAL(pos) << "," 
	      << 0 << "</coordinates>" << endl
	      << "\t\t</Point>" << endl
	      << "\t</Placemark>" << endl;
  }


  ofs.open(GeoFn());  
  if (! ofs ) {
    cerr << BlockName << ": error opening "
	 << GeoFn() << endl;
    exit(_ERROR_OPEN_FILE_);
  }

  ofs << kmlhead.str() << kmltmp.str() << kmlheadend.str();
  ofs.close();

}



double MCPMPCoeffs::GeoDistance(unsigned int tx, unsigned int rx) {
  gsl_complex txPos =  gsl_vector_complex_get(geoPositions,tx); // lat,lon
  gsl_complex rxPos =  gsl_vector_complex_get(geoPositions,rx+_M); // lat,lon

  double txPosLat = GSL_REAL(txPos)*M_PI_OVER_180;
  double txPosLon = GSL_IMAG(txPos)*M_PI_OVER_180;
  double rxPosLat = GSL_REAL(rxPos)*M_PI_OVER_180;
  double rxPosLon = GSL_IMAG(rxPos)*M_PI_OVER_180;

  double deltaLat = rxPosLat-txPosLat; // ok 
  double deltaLon = rxPosLon-txPosLon; // be careful around 180E/W
  double meanLat = (rxPosLat+txPosLat)/2.0; // be careful around poles

  if (deltaLon > M_PI) // es Lon1 =-179 Lon2=179 D=2
    deltaLon -= 2*M_PI;
  if (deltaLon < -M_PI)
    deltaLon += 2*M_PI;

  double tmparg = gsl_sf_cos(meanLat)*deltaLon;

  return EARTH_RADIUS * gsl_hypot( deltaLat,tmparg );

}


//
// we update shadowing and pathloss coefficients for all the links tx -> rx
//
void  MCPMPCoeffs::SpatialChannelUpdate() {

	for (int i=0; i<_M; i++) { // user i (tx)
		for (int ii=0;ii<=i;ii++) { // user ii (rx)

			// we are considering the spatial channel tx-rx, so we get the sostheta(j) vector
			// and we compute the shadowing coefficient for the position of rx (rx_lon,rx_lat)

			gsl_complex txPos =  gsl_vector_complex_get(geoPositions,i); // lat,lon
			gsl_complex rxPos =  gsl_vector_complex_get(geoPositions,ii+_M); // lat,lon

			double txPosLat = GSL_REAL(txPos);
			double txPosLon = GSL_IMAG(txPos);
			double rxPosLat = GSL_REAL(rxPos);
			double rxPosLon = GSL_IMAG(rxPos);

			double deltaLat = rxPosLat-txPosLat; // ok
			double deltaLon = rxPosLon-txPosLon; // be careful around 180E/W
			double meanLat = (rxPosLat+txPosLat)/2.0; // be careful around poles

			if (deltaLon > 180) // es Lon1 =-179 Lon2=179 D=2
				deltaLon -= 360;
			if (deltaLon < -180)
				deltaLon += 360;

			double x= deltaLon * 111111 * gsl_sf_cos(meanLat*M_PI_OVER_180); // cartesian x position around tx (in m)
			double y= deltaLat * 111111; // cartesian y position arond tx (in m)

			// shadowing i->ii
			double shadowdb=0;
			for (int j=0; j<SOSN; j++){
				double th = gsl_matrix_get(sostheta,i*_M+ii,j);
				double fxn = gsl_vector_get(sosfxn,j);
				double fyn = gsl_vector_get(sosfyn,j);
				shadowdb += sosc * gsl_sf_cos(2.0 * M_PI * ( fxn * x + fyn * y + th));
			} // for j

			// pathloss i->ii
			// 10 Log ( d^-3) = 10 Log ( d^2 ^-3/2) = -15 Log ( d^2 ) = -15 Log ( d^2 )
			double dist = sqrt(x*x+y*y);
		    double plossdb =  ( -30.0 * gsl_sf_log( dist ) + 30.0 * gsl_sf_log(ZeroDb()) )/M_LN10;

		    cout << "rx[" << i << "] x=" << x << " y=" << y << " ploss="
		    		<< plossdb << " shadow=" << shadowdb << " dist=" << dist << endl;
		    //
		    double cx = pow(10,( 0.5 * plossdb + SOSsigma() * shadowdb )/10.0 );

		     gsl_matrix_set(pathLoss,i,ii,cx);
		     gsl_matrix_set(pathLoss,ii,i,cx); // it's symmetric !

		} // for ii
	} // for i
}

