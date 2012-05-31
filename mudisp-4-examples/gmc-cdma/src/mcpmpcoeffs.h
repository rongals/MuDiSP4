//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP2_MCPMPCOEFFS_H 
#define __MUDISP2_MCPMPCOEFFS_H

#include "mudisp.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_rng.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define GEO_AREA_CENTER_LAT 29.964324
#define GEO_AREA_CENTER_LON 30.893594
#define GEO_AREA_SIZE 0.01 // in dec degrees
#define GEO_VELOCITY_MIN 0.0 
#define GEO_VELOCITY_MAX 0.00000436 // 100 kmph (rad/s = km/h / 3.6 / 1.11 * 0.00001 / 360 * 2 PI )
#define GEO_UPDATE_INTERVAL 1000
#define EARTH_RADIUS 6371.009 // km
#define M_PI_OVER_180 M_PI/180.0


// Accuracy versus decimal places at the equator
// decimal
// places	degrees		distance
// 0		1.0    		111 km
// 1		0.1		11.1 km
// 2		0.01		1.11 km
// 3		0.001		111 m
// 4		0.0001		11.1 m
// 5		0.00001		1.11 m
// 6		0.000001	0.111 m
// 7		0.0000001	1.11 cm
// 8		0.00000001	1.11 mm
//

////////
////////   Here put your personal includes
////////


class MCPMPCoeffs : public Block {
 private:

  ////////   Parameters instances

  IntParam N,L,M;
  FloatParam PTau;
  FloatParam Ricean;
  FloatParam ZeroDb;
  StringParam GeoFn;

  ////////   Local Attributes

  double gain, gainrice;
  gsl_matrix_complex *ch;
  gsl_matrix *pathLoss;
  gsl_rng * ran; 
  unsigned int _M, runCount;


  // Geo Model

  gsl_vector_complex *geoPositions, *geoVelocities; // node coordinates tx + rx (2xM)
  unsigned int geoEnabled;
  ostringstream kmlhead,kmlobject,kmlheadend;
  ofstream ofs;


 public:

////////   InPorts and OutPorts

  OutPort < gsl_matrix_complex > mout1;  // complex mat MxMxN 



  MCPMPCoeffs():Block("MCPMPCoeffs")
    ////////  parameters initializazion
    ,M("NumUsers",2,"number of users")
    ,N("Carriers",16,"number of carriers")
    ,L("ChanTaps",2,"number of channel taps")
    ,PTau("PowerTau",1.0,"decay factor of delay spread")
    ,Ricean("RiceFact",0.0,"Ricean factor of channel")
    ,GeoFn("GeoFName","none","kml for georendering (none to disable)")
    ,ZeroDb("ZeroDb",0.01,"zero dB distance for path loss (km)")
    {

      //////// local parameter registration
      AddParameter(N);
      AddParameter(M);
      AddParameter(L);
      AddParameter(PTau);
      AddParameter(Ricean);
      AddParameter(GeoFn);
      AddParameter(ZeroDb);
    }

  ~MCPMPCoeffs() {
  }

  void Setup();
  void Run();
  void Finish();

 protected:

  void GeoInit();
  void GeoUpdate(double seconds=1.0);
  void GeoRender();
  double GeoDistance(unsigned int,unsigned int);
  void PathLossUpdate();
  
};

#endif /* __MUDISP_MYBLOCK_H  */

