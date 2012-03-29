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



#ifndef __MUDISP2_WRITE_HXX
#define __MUDISP2_WRITE_HXX

#include <mudisp/engine/mudisp.h>
#include <fstream>
#include <complex>

//
// WRITE_DOUBLE
//


#define NUMWIDTH 20

/*! @class Write write.h <library/base/write.h>
 *  @brief  Implementazione di un block di scrittura su file o schermo
 *  @param  fname Nome del file
 *  @author Luca Simone Ronga
 *  @version 1
 *  @date     1999-2011
 *  @ingroup  sink_blk
 */ 
class Write : public Block {

private:
/** @var StringParam fname
 * @brief File name
 * */
  StringParam fname;
  /** @var IntParam gnuplot
   * @brief Gnuplot format
   * */
  IntParam gnuplot;
  /** @var ofstream ofs
   * @brief Standard Out contenitor
   * */
  ofstream ofs;
  /** @var int f 
   * @brief Variable */
 int f;

public:

	/** @var InPort <double> in1
	 * @brief Define InPort Object for data in streaming 
	 * */
  InPort <double> in1;
  
  
/** @fn Write():Block("Write")
    ,fname("OutFile","none", "No out, sobstitute cout for standard out or filename ")
    ,gnuplot("Compatible",1, "If set 1 the out is gnuplot style else if 0 the output is in standard out form")
    * @brief Class Constructor 
    * */
  Write():Block("Write")
    ,fname("OutFile","none", "No out, sobstitute cout for standard out or filename ")
    ,gnuplot("Compatible",1, "If set 1 the out is gnuplot style else if 0 the output is in standard out form"){
    AddParameter(fname);
    AddParameter(gnuplot);
  }

/** @fn void Setup()
 * @brief Setup Method
 * */ 
  void Setup();
/** @fn void Run()
 * @brief Run Method
 * */ 
  void Run();
/** @fn void Finish()
 * @brief Finish Method
 * */ 
  void Finish();

};

//
// WRITE_COMPLEX
//
/*! @class WriteCx write.h <library/base/write.h>
 *  @brief  Implementazione di un block di scrittura su file o schermo di campioni complessi
 *  @param  fname Nome del file
 *  @param  Name non specificato  
 *  @author Luca Simone Ronga
 *  @version   1
 *  @date     1999-2011
 *  @ingroup  sink_blk
 */ 
class WriteCx : public Block {
/** @var StringParam fname
 * @brief Filename Path
 * */

/** @var StringParam Name
 * @brief Filename Name
 * */
 
  StringParam fname, Name;
    /** @var ofstream ofs
   * @brief Standard Out contenitor
   * */
  ofstream ofs;
  /** @var  IntParam _runs
   * @brief Numbers of runs for block */
     /** @var  IntParam gnuplot
   * @brief gnuplot state variable  */
  IntParam gnuplot, _runs;
    /** @var int f 
   * @brief Variable */
  unsigned int f;

public:

	/** @var InPort <double> in1
	 * @brief Define Block InPort Object for data in streaming 
	 * */
  InPort < complex<double> > in1;

/** @fn WriteCx():Block("WriteCx")
    ,fname("OutFile","none" )
    ,Name("Nome","","ID Name")
    ,gnuplot("Compatible",1, "If set 1 the out is gnuplot style else if 0 the output is in standard out form")
    ,_runs("Iterations", 1, "Number of iterations") 
    * @brief Class Constructor 
    * */
  WriteCx():Block("WriteCx")
    ,fname("OutFile","none" )
    ,Name("Nome","","ID Name")
    ,gnuplot("Compatible",1, "If set 1 the out is gnuplot style else if 0 the output is in standard out form")
    ,_runs("Iterations", 1, "Number of iterations") {
    AddParameter(fname);
    AddParameter(gnuplot);
    AddParameter(Name);
    AddParameter(_runs);
  }

/** @fn void Setup()
 * @brief Setup Method
 * */ 
  void Setup();
  /** @fn void Run()
 * @brief Run Method
 * */ 
  void Run();
  /** @fn void Finish()
 * @brief Finish Method
 * */
  void Finish();

};




#endif /* __MUDISP_WRITE_HXX  */
