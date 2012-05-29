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

#define GEO_AREA_CENTER_LON 11.252945
#define GEO_AREA_CENTER_LAT 43.799908
#define GEO_AREA_SIZE 0.000010

////////
////////   Here put your personal includes
////////


class MCPMPCoeffs : public Block {
 private:

  ////////   Parameters instances

  IntParam N,L,M;
  FloatParam PTau;
  FloatParam Ricean;
  StringParam GeoFn;

  ////////   Local Attributes

  double gain, gainrice;
  gsl_matrix_complex *ch;
  gsl_rng * ran; 
  unsigned int _M;

  // Geo Model

  gsl_vector_complex *geoPositions; // node coordinates
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
    {

      //////// local parameter registration
      AddParameter(N);
      AddParameter(M);
      AddParameter(L);
      AddParameter(PTau);
      AddParameter(Ricean);
      AddParameter(GeoFn);

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

  
};

#endif /* __MUDISP_MYBLOCK_H  */

