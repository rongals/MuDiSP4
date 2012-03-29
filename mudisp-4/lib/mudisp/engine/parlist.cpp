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



#include <mudisp/engine/parlist.h>

void ParamList::AddParameter( GenericParam & toadd ) {
  plist.push_front( &toadd );
  pmap[toadd.ParamName]=&toadd; 
}

void ParamList::ReadAllParameters( string filename , 
				   vector <string> base_name) {
  list < GenericParam * > ::iterator iter;
  for ( iter=plist.begin(); iter!=plist.end(); iter++ ) {
    if (! (*iter)->isMute() ) 
      (*iter)->Read(filename,base_name);
  }
}

void ParamList::DumpAllParameters( ostream & os, 
				   vector <string> base_name){
  bool isTheFirst=true;

  list < GenericParam * > ::iterator iter;
  for ( iter=plist.begin(); iter!=plist.end(); iter++ )    
    if (!((*iter)->Exported() || (*iter)->isMute())) {
      if (isTheFirst) {
	os << ";\n;  Parameters for " 
	   << base_name[base_name.size()-1]
	   << "\n;\n";
	isTheFirst=false;
      }
      (*iter)->DumpString(os ,base_name);    
    }

  if (! isTheFirst )
    os << "\n\n";
}

bool ParamList::empty() { return plist.empty(); }

void ParamList::ExportParam(string pName){

  if (pmap.find(pName)!=pmap.end()) {
    //
    // parameter name found !
    //
    
    GenericParam *t;
    t=pmap[pName];
    
    t->Export();
  } 
  else {
    cerr << "Error using MuteParam, " << pName 
	 << " not found." << endl;
  }
}
