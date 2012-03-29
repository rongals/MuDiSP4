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


/** @def DUMP_FILE_NAME 
 * @brief define configuration file name
 * */
#define DUMP_FILE_NAME "param.txt"

#include <mudisp/engine/port.h>
#include <mudisp/engine/block.h>
#include <mudisp/engine/system.h>
#include <mudisp/engine/param.h>

#ifndef __MAIN_H__
#define __MAIN_H__

/** @fn void main(int argc, char* argv[])
 * @brief  Main routine for running MuDiSP 
 * @param args arguments numer pass to main
 * @param argv[] vector for arguments
 * */
void main(int argc, char* argv[]) {

  System s;

  s.Build();

  if (argc!=2) {
    cout << "MuDiSP v4.0\n" 
         << "by Luca Ronga\n\n"
	 << "A sample parameter file for your system has been\n"
	 << "generated in \"param.txt\"\n\n"
	 << "To run the simulation use the syntax:\n"
	 << argv[0] << "<param_file>\n\n";

    ofstream outstr(DUMP_FILE_NAME);
    s._DumpParameters(outstr);
    outstr << endl; 
    outstr.close();
  }
  else {
    string filename(argv[1]);
    s._ReadParameters(filename);
    s.Setup();
    s.SequenceFinder();
    s.Run();
    s.Finish();
  }
}

#endif
