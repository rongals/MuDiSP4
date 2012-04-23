//
// MuDiSP3 (Multirate Digital Signal Processor 3)
// Copyright 1999, Luca Simone Ronga
//
//
//  This file is part of MuDiSP3.
//
//  MuDiSP3 is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  MuDiSP3 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with MuDiSP3; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
/////////////////////////////////////////////////////////



#ifndef __MUDISP2_GSLWRITE_HXX
#define __MUDISP2_GSLWRITE_HXX

#include "mudisp.h"
#include <fstream>
#include "gsl/gsl_vector.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_blas.h"


//
// WRITE_DOUBLE
//

class GSLWrite : public Block {

private:

  StringParam fname;
  ofstream ofs;


public:

  InPort < gsl_vector_complex > in1;

  GSLWrite():Block("Write"),
    fname("OutFile","cout"){
    AddParameter(fname);
  }

  void Setup();
  void Run();
  void Finish();

};


#endif /* __MUDISP_WRITE_HXX  */
