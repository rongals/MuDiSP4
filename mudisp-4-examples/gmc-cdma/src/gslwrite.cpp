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



#include "gslwrite.h"

//
// GSLWRITE
//


void GSLWrite::Setup(){
  string fn( fname() );
  if (fn != "cout") {
    ofs.open( fname());
  if (! ofs ) {
    cerr << BlockName << ": error opening "
	 << fn << endl;
    exit(_ERROR_OPEN_FILE_);
  }
  }
}


void GSLWrite::Run() {

  gsl_vector_complex a = in1.GetDataObj();

  string fn( fname() );

  
  for (int i=0; i<a.size; i++) {
    gsl_complex ai = gsl_vector_complex_get(&a,i);
    if (fn == "cout")
      cout <<  "( " << GSL_REAL(ai) << ", " << GSL_IMAG(ai) << " )" << endl;
    else
       ofs <<  "( " << GSL_REAL(ai) << ", " << GSL_IMAG(ai) << " )" << endl;
  }

    if (fn == "cout")
      cout <<  endl;
    else
       ofs <<  endl;

}  

void GSLWrite::Finish(){
    string fn( fname() );
    if (fn != "cout")
      ofs.close();
  }
  
