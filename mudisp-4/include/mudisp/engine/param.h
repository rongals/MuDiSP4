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



#ifndef __MUDISP2_PARAM_HXX
#define __MUDISP2_PARAM_HXX
/** @def NAME_FIELD
 * @brief Define max length of field name, defaul 40
 * */ 
#define NAME_FIELD 40
#define VALUE_FIELD 15
#define MAXLINEL 255
#define MAXSTRINGL 128


#include <mudisp/engine/errors.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <cstring>

using namespace std;

/** @class  GenericParam parlist.h <mudisp/engine/parlist.h>
 *  @brief Define Param Object for class param in Mudisp functions Block 
 *  @author Luca Simone Ronga
 *  @ingroup Core
 * */
class GenericParam {

private:
	/** @var bool isMuted
	 * @brief True if the parameter has been set by hand 
	 * */
  //
  // True if the parameter has been set by hand
  //
  bool isMuted;
	
	/** @var bool isExported
	 * @brief True if the parameter needs to read the file but does not appear in the param file
	 * */
  //
  // True if the parameter needs to read the file but does
  // not appear in the param file
  //
  bool isExported;

public:
  
  /** @var string ParamName
   * @brief name of the parameter
   * */
  //
  // name of the parameter
  //
  string ParamName;

  /** @var string ParamDesc
   * @brief description of the parameter
   * */
  //
  // description of the parameter
  //ar 
  /** @var  string ParamDesc
   * @brief Description of the parameter
   * */
  string ParamDesc;

/** @fn GenericParam(): isMuted(false), isExported(false)
 * brief Class Constructor 
 * */
  GenericParam(): isMuted(false), isExported(false) {}
  /** @fn   GenericParam(const char *name,const char *desc)
 * @brief Class Constructor 
 * @param name String of char for define parameter name
 * @param desc Short parameter description 
 * */
  GenericParam(const char *name,const char *desc);
  
  virtual ~GenericParam(){}

  //
  // Default line dump in parameter file 
  //
  /** @fn  virtual void DumpString( ostream & , vector <string> base_name )
   * @brief Dump class parameter in the MuDiSp configuration file 
   * @param base_name name of parameter
   * */
  virtual void DumpString( ostream & , vector <string> base_name ){}
    /** @fn virtual void Read( string filename , vector <string> base_name )
   * @brief Read class parameter from the MuDiSp configuration file 
   * @param base_name name of parameter
   * @param filename Name to store configuration file 
   * */
  virtual void Read( string filename , vector <string> base_name ){}
  /** @fn  virtual void Set(const int &val)
   * @brief Set value for interger parameter */
  virtual void Set(const int &val){} 
  
  /** @fn  virtual void Set(const double &val)
   * @brief Set value for double precifion float parameter */
   
  virtual void Set(const double &val){} 
  /** @fn  virtual void Set(const char *val)
   * @brief Set value for array of char parameter */
   
  virtual void Set(const char *val){}
	/** @fn bool isMute()
	 * @brief Check if parameter is in Muted state 
	 * @return true if in Muted*/
  bool isMute(){ return isMuted; }
  	/** @fn void Mute()
	 * @brief Set parameter Muted state = true
	 * */
  void Mute() { isMuted=true; }
  	/** @fn bool  Exported()
	 * @brief Check if parameter is in Exported state 
	 * @return true if in Exported*/
  bool Exported(){ return isExported; }
    	/** @fn void Export()
	 * @brief Set parameter isExported state = true
	 * */
  void Export() { isExported=true; }

};

/** @class IntParam parlist.h <mudisp/engine/parlist.h>
 * @brief Define Integer Parameter for MuDiSP
 * */
class IntParam : public GenericParam {
public:
/** @var int Value 
 * @brief Class Value contenitor */
  int Value;

/** @fn IntParam()
 * @brief Class Empty Constructor 
 * */
  IntParam(){}
  /** @fn IntParam(const char*, const int &, const char* desc="")
   * @brief Class Constructor, Define parameter name, value end description 
   * @param desc Short description of parameter 
   * */
  IntParam(const char*, const int &, const char* desc="");
  ~IntParam(){}
  /** @fn int operator()()
   * @brief Operator for get parameter value
   * @return Value of parameter 
   * */
  int operator()(){ return Value; }
  
  /** @overload void DumpString( ostream & os ,   vector <string> base_name )
   * @brief Method overload top class function for write parameter in the configuration file 
   * @param os Standard Output for parameter file
   * @param base_name Name of parameter */
  void DumpString( ostream & os ,   vector <string> base_name );
  
    /** @overload void Read( string filename , vector <string> base_name )
   * @brief Method overload top class function for read parameter from configuration file 
   * @param filename path for parameter file
   * @param base_name Name of parameter 
   * */
  void Read( string filename , vector <string> base_name );

/** @fn  void Set(const int & val)
 * @brief Method for set value of parameter
 * @param val value tu set */
  void Set(const int & val);

};

/** @class FloatParam parlist.h <mudisp/engine/parlist.h>
 * @brief Define Double Precision floating Parameter for MuDiSP
 * */
 
class FloatParam : public GenericParam {
public:
/** @var doube Value
 * @brief define class value contenitor */
  double Value;

   /** @fn FloatParam()
   * @brief Class Constructor
   * */
  FloatParam(){}
    /** @fn FloatParam(const char*, const double &, const char* desc="")
   * @brief Class Constructor, Define parameter name, value end description 
   * @param desc="" Short description of parameter 
   * */
   FloatParam(const char*, const double &, const char* desc="");
  ~FloatParam();
  
   /** @fn double operator()()
   * @brief Operator for get parameter value
   * @return Value of parameter 
   * */
   
  double operator()(){ return Value; }
  
    /** @overload void DumpString( ostream & os ,   vector <string> base_name )
   * @brief Method overload top class function for write parameter in the configuration file 
   * @param os Standard Output for parameter file
   * @param base_name Name of parameter */
   
  void DumpString( ostream & os ,   vector <string> base_name );
  
      /** @overload void Read( string filename , vector <string> base_name )
   * @brief Method overload top class function for read parameter from configuration file 
   * @param filename path for parameter file
   * @param base_name Name of parameter 
   * */
  void Read( string filename , vector <string> base_name );
  /** @fn  void Set(const  double & val)
 * @brief Method for set value of parameter
 * @param val value tu set */
  void Set(const double & val);


};

/** @class StringParam parlist.h <mudisp/engine/parlist.h>
 * @brief Define String Parameter for MuDiSP
 * */
 
class StringParam : public GenericParam {
public:

/** @var char Value[MAXSTRINGL]
 * @brief define class value contenitor */
  char Value[MAXSTRINGL];

  /** @fn StringParam()
   * @brief Class Constructor 
   * */
   
  StringParam(){}
  
    /** @fn StringParam(const char*, const char *, const char* desc="")
   * @brief Class Constructor, Define parameter name, value end description 
   * @param desc="" Short description of parameter 
   * */
   
  StringParam(const char*, const char * , const char* desc="");
  ~StringParam();
  
   /** @fn const char *operator()()
   * @brief Operator for get parameter value
   * @return Value of parameter 
   * */
   
  const char *operator()(){ return Value; }

  /** @overload void DumpString( ostream & os ,   vector <string> base_name )
   * @brief Method overload top class function for write parameter in the configuration file 
   * @param os Standard Output for parameter file
   * @param base_name Name of parameter */
   
  void DumpString( ostream & os , vector <string> base_name );
  
      /** @overload void Read( string filename , vector <string> base_name )
   * @brief Method overload top class function for read parameter from configuration file 
   * @param filename path for parameter file
   * @param base_name Name of parameter 
   * */
  void Read( string filename , vector <string> base_name );
  
  /** @fn  void Set(const char * val)
 * @brief Method for set value of parameter
 * @param val value tu set */
  void Set( const char * val );


};


#endif /* __MUDISP_PARAM_HXX  */

