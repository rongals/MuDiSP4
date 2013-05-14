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

#define GEO_UPDATE_INTERVAL 30
#define GEO_AREA_CENTER_LAT 43.807635
#define GEO_AREA_CENTER_LON 11.195560
#define GEO_AREA_SIZE 0.03 // in dec degrees
#define EARTH_RADIUS 6371.009 // km
#define M_PI_OVER_180 M_PI/180.0



// vx (km/h), vy (km/h)
// deltalon = vx / 3.6 / 111111 Cos[lat]
// deltalat = vy / 3.6 / 111111 
#define GEO_VELOCITY_MIN 0.0 // expressed in km/h
#define GEO_VELOCITY_MAX 100.0 // expressed in km/h


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

//
// Quick and dirty
//
// 111.111 km = 1 deg LAT
// 111.111 cos(LAT) km = 1 deg LON
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

