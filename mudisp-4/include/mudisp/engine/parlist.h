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



#ifndef __MUDISP2_PARLIST_HXX
#define __MUDISP2_PARLIST_HXX

#include <mudisp/engine/param.h>
#include <string>
#include <map>
#include <list>
#include <iostream>
#include <vector>

/** @class  ParamList parlist.h <mudisp/engine/parlist.h>
 *  @brief Define List of enumaration for param in Mudisp function Block 
 *  @author Luca Simone Ronga
 *  @ingroup Core
 * */
class ParamList {

private:
/** @var list < GeneriecParam * > plist
 * @brief Define a list of parameter
 * */
  list < GenericParam * > plist;
/** @var map < string , GenericParam *> pmap
 * @brief Define a contenitor map list of parameter and name
 * */  
  map < string , GenericParam *> pmap; 

public:

	/** @fn ParamList()
	 * @brief Class Constructor */
  ParamList(){}
  
  /** @fn void AddParameter(GenericParam & toadd )
   * @brief Get a new parameter and add this to parameter list 
   * @param toadd parameter to add in the list
   * */

  void AddParameter( GenericParam & toadd );
  
  /** @fn void ReadAllParameters( string filename , vector <string> base_name)
   * @brief Read all parameter from configuration file and store parameter name in a vector for recal when need 
   * @param filename configuation filename
   * @param base_name vector of parameter name read from file
   * */
  void ReadAllParameters( string filename , vector <string> base_name);
  
  /** @fn void DumpAllParameters( ostream & os, vector <string> base_name)
   * @brief Store (Dump) parameter list in configuration file 
   * @param os define standard output position for data out
   * @param base_name vaector of parameter name to store in file */ 
  void DumpAllParameters( ostream & os, vector <string> base_name);
  
  /** @fn bool empty()
   * @brief Check if parameter list \a plist is empty
   * @return true if \a plist is empty */
  bool empty();

/** @fn template <class PType> void Set(string pName, PType val)
 * @brief Set parameter value
 * @tparam pName Parameter Name
 * @tparam pValue Parameter value
 * */
  template <class PType> void Set(string pName, PType val){

    if (pmap.find(pName)!=pmap.end()) {
      //
      // parameter name found !
      //
      
      GenericParam *t;
      t=pmap[pName];
      
      t->Set(val);
    } 
    else {
      cerr << "Error using SetParamter, " << pName 
	   << " not found." << endl;
    }
  }


/** @fn void ExportParam(string pName)
 * @brief if found, the parameter is muted and will not appear in the parameter file
 * @param pName Parameter name */
  //
  // if found, the parameter is muted and will not appear in the 
  // parameter file
  //
  void ExportParam(string pName);



};

#endif /* __MUDISP_PARLIST_HXX  */
