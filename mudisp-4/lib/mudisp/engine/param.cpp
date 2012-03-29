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



#include <mudisp/engine/param.h>

GenericParam::GenericParam(const char *name,const char *desc):
  isMuted(false), isExported(false),
  ParamName(name), ParamDesc(desc) {}

IntParam::IntParam( const char* name, 
		    const int & val, 
		    const char* desc): 
  GenericParam(name,desc),
  Value(val){}


void IntParam::DumpString( ostream & os, vector <string> base_name) {

  string tmp;
  for (unsigned int i=0;i<base_name.size();i++)
    tmp += base_name[i] + ".";

  tmp += ParamName + ": ";

  os.width(NAME_FIELD);
  os.setf(ios::left,ios::adjustfield);
  os << tmp.c_str();

  os.width(VALUE_FIELD);
  os.setf(ios::left,ios::adjustfield);
  os << Value;

  if (ParamDesc.size() > 0)
    os << "; " << ParamDesc << endl;
  else
    os << endl;
}

void IntParam::Read( string filename , vector <string> base_name ){
  //
  // some preparative stuff
  //
  ifstream is(filename.c_str());
  string str;
  int currdepth = -1;
  bool isCited=false;
  size_t ptr;

  if (!is) {
    cerr << ParamName << ": error accessing param. file" << endl;
    exit(_ERROR_OPEN_PARAM_FILE_);
  }
  
  while ( getline(is,str) ) {
    string templ;
//
// ok, let's read the file
//
    if (((ptr=str.find(ParamName))!= str.npos) && (str[0]!=';')) {
//
// found ParamName in the line !
//
      for (int dpth=0;dpth< int(base_name.size()); dpth++) {
	templ += base_name[dpth] + ".";
	if ( str.find(templ+ParamName ) != str.npos && (currdepth<dpth)) {
//
// this is a value to read ...
//
	  if (sscanf(str.c_str(),"%*s %d %*s",&Value)!=1) {
	    cerr << ParamName << ": error reading param. value" << endl;
	    exit(_ERROR_READ_PARAM_VALUE_);
	  }
	  isCited = true;
	  currdepth=dpth;
	}
      }
    }
  }
  if (! isCited) {
    cerr << ParamName << ": Parameter not cited in param file." << endl;
    exit(_ERROR_PARAM_NOT_CITED_);
  }
}


void IntParam::Set(const int & val){ 
  Value = val; 
  GenericParam::Mute();
}

//
// FLOAT PARAMETER
//
FloatParam::FloatParam( const char* name, 
		    const double & val, 
		    const char* desc)
  :GenericParam(name,desc),Value(val){}

FloatParam::~FloatParam() {}


void FloatParam::DumpString( ostream & os, vector <string> base_name) {

  string tmp;
  for (unsigned int i=0;i<base_name.size();i++)
    tmp += base_name[i] + ".";

  tmp += ParamName + ": ";

  os.width(NAME_FIELD);
  os.setf(ios::left,ios::adjustfield);
  os << tmp.c_str();

  os.width(VALUE_FIELD);
  os.setf(ios::left,ios::adjustfield);
  os << Value;

  if (ParamDesc.size() > 0)
    os << "; " << ParamDesc << endl;
  else
    os << endl;

}


void FloatParam::Read( string filename , vector <string> base_name ){
  //
  // some preparative stuff
  //
  ifstream is(filename.c_str());
  string str;
  int currdepth = -1;
  bool isCited=false;
  size_t ptr;

  if (!is) {
    cerr << ParamName << ": error accessing param. file" << endl;
    exit(_ERROR_OPEN_PARAM_FILE_);
  }
  
  while ( getline(is,str) ) {
    string templ;
//
// ok, let's read the file
//
    if ( ((ptr=str.find(ParamName)) != str.npos) && (str[0]!=';')) {
//
// found ParamName in the line !
//
      for (int dpth=0; dpth<int(base_name.size()); dpth++) {
	templ += base_name[dpth] + ".";
	if (str.find(templ+ParamName)!=str.npos && (currdepth<dpth)) {
//
// this is a value to read ...
//
	  if (sscanf(str.c_str(),"%*s %lf %*s",&Value)!=1) {
	    cerr << ParamName << ": error reading param. value" << endl;
	    exit(_ERROR_READ_PARAM_VALUE_);
	  }
	  currdepth=dpth;
	  isCited = true;
	}
      }
    }
  }
  if (! isCited) {
    cerr << ParamName << ": Parameter not cited in param file." << endl;
    exit(_ERROR_PARAM_NOT_CITED_);
  }
}

void FloatParam::Set(const double & val){ 
  Value = val; 
  GenericParam::Mute();
}


//
// STRING PARAMETER
//

StringParam::StringParam( const char* name, 
			  const char *val, 
			  const char* desc):
 GenericParam(name,desc){
  strncpy(Value,val,MAXSTRINGL);
}

StringParam::~StringParam() {}


void StringParam::DumpString( ostream & os, vector <string> base_name) {

  string tmp;
  for (unsigned int i=0;i<base_name.size();i++)
    tmp += base_name[i] + ".";

  tmp += ParamName + ": ";

  os.width(NAME_FIELD);
  os.setf(ios::left,ios::adjustfield);
  os << tmp.c_str();

  os.width(VALUE_FIELD);
  os.setf(ios::left,ios::adjustfield);
  os << Value << " ";

  if (ParamDesc.size() > 0)
    os << "   ; " << ParamDesc << endl;
  else
    os << endl;

}


void StringParam::Read( string filename , vector <string> base_name ){
  //
  // some preparative stuff
  //
  ifstream is(filename.c_str());
  string str;
  int currdepth = -1;
  bool isCited=false;
  size_t ptr;

  if (!is) {
    cerr << ParamName << ": error accessing param. file" << endl;
    exit(_ERROR_OPEN_PARAM_FILE_);
  }
  
  while ( getline(is,str) ) {
    string templ;
//
// ok, let's read the file
//
    if ( ((ptr=str.find(ParamName))!=str.npos) && (str[0]!=';')) {
//
// found ParamName in the line !
//
      for (int dpth=0;dpth<int(base_name.size());dpth++) {
	templ += base_name[dpth] + ".";
	if (str.find(templ+ParamName)!=str.npos && (currdepth<dpth)) {
//
// this is a value to read ...
//
	  if (sscanf(str.c_str(),"%*s %s %*s",(char *)(&Value))!=1) {
	    cerr << ParamName << ": error reading param. value" << endl;
	    exit(_ERROR_READ_PARAM_VALUE_);
	  }
	  currdepth=dpth;
	  isCited = true;
	}
      }
    }
  }
  if (! isCited) {
    cerr << ParamName << ": Parameter not cited in param file." << endl;
    exit(_ERROR_PARAM_NOT_CITED_);
  }
}

void StringParam::Set(const char *val){ 
  strncpy(Value,val,MAXSTRINGL);
  GenericParam::Mute();
}

