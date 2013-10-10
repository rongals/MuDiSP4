 /*
 * Copyright 2010 Innocenti Massimo
 * 
 * This file is part of  MuDiSP4 
 * 
 * 
 * MuDiSP4 (Multirate Digital Signal Processor 4)
 * Copyright 2010, Luca Simone Ronga
 * 
 * 
 *  MuDiSP4 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * MuDiSP4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with MuDiSP3; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef INCLUDED_UHD_MUDISP_USRP_SINK_H
#define INCLUDED_UHD_MUDISP_USRP_SINK_H
 
/*!  Librerie MuDiSP				  */
#include <mudisp/engine/mudisp.h> 
/*! Librerie STD  nuovo module USRP   */
#include <cstdio>
#include <cstdint>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <complex>
#include <cmath>
#include <cstdlib> 
/*! Librerie TNT  */
#include <tnt/tnt.h> 
#include <tnt/cmat.h> 
/*! Librerie UHD  nuovo module USRP  */
#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
/*! Librerie Boost  nuovo module USRP  */
#include <boost/program_options.hpp>  //pharser dei parametri std_out  <-- può essere rimossa
#include <boost/thread/thread.hpp>  
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/thread/thread_time.hpp> //system time
#include <boost/math/special_functions/round.hpp> 

// #include <gruel/realtime.h>
/*! Libreire di Matematica avanzata GLS */
#include "gsl/gsl_randist.h"
#include "gsl/gsl_complex.h"
#include "gsl/gsl_complex_math.h"






using namespace std; 
using namespace TNT;
using namespace uhd;

/*! \class  UHD_MuDiSP_USRP_Sink : public Block   uhd_mudisp_usrp_sink.h "/home/massimo/"
 * \brief Unificate Block Object to Connect MuDiSP to  Universal Software Radio Peripheral 
 * \author Innocenti Massimo
 * \date November 2010
 * \param address single uhd USRP devise IP adresso for USRP2 interface
 * \param rate    rate of outgoing  samples 
 * \param gain	  gain for the RF chain
 * \param freq    rf center frequency in Hz
 * \param undefine
 * \param undefine
 * \param undefine
 * \param undefine
 * \note The USRP2 source outputs 100 Megasamples per second / decimation. To use the default ethernet device, leave interface blank. For systems with only 1 USRP2, you may leave the mac address blank. For multi-USRP2 systems, specify the mac address in the form 00:50:C2:85:3x:xx.
 * \ingroup usrp
 * */
 
 
class UHD_MuDiSP_USRP_Sink : public Block { 
  
private: 
 
  StringParam args;		// Argomento del IP
			//  mac_addr, // 
			//  filename;

  IntParam interp, 
			scale, 
			sample,	// Total Number of Sample 
			spp,	// Sample per Packet
			spf, 	// Sample per Frame
			spb,	// Sample per Buffer 
			repeat;
			
			
  FloatParam rate, // rateof outgoing samples
			 freq, // rf center frequency in Hz 
			 ampl, // amplitude of the waveform
			 gain; // gain for the RF chain
  //static volatile bool signaled;
//  int32_t samples_per_frame, scale_factor, interp_factor;
//  double frequency, tx_gain;

  /////


	uhd::tx_metadata_t md;
public: 
  // FILE * pFile;
  // long lSize;
  // char * buffer;
  // size_t result;
////////   InPorts and OutPorts 
 /*!n \var in1 
  * \brief Define Input Port Object */
	InPort <Vector <complex < double> > > in1; 
	//device_addr_t dev_addr;

  UHD_MuDiSP_USRP_Sink():Block("USRP_UHD_Sink") 
////////  parameters initializazion    
    ,args ("Args","192.168.11.2","Single USRP device address args") 
    ,spb ("Spf",1000,"Sample per Buffer")
    ,rate("Rate",1.5e06,"rate of outgoing samples")
    ,freq("frequency",2400000000.0,"RF center frequency in Hz")
    ,ampl("Amplitude", 1,"amplitude of the waveform")
    ,gain("tx_gain",0.0,"gain for th RF chain")
    ,sample("Sample",1000,"total number of sample to trasmit")
    ,spp("Spp",1000,"Number of sample per Packet ")
    
  /*,samplespf("samples_per_frame",371,"set the number of samples per frame")
    ,filename("input_file", "stdin","set the input file contain the samples")
    ,repeat("repeat",0,"set if repeat the file, 0 for false 1 for true")
    * */
    { 
 
//////// local parameter registration 
      AddParameter(args); 
      AddParameter(rate); 
      AddParameter(ampl);
      AddParameter(freq); 
      AddParameter(gain); 
      AddParameter(sample);
      AddParameter(spp);
      AddParameter(spb);
} 

  void Setup(); 
  void Run(); 
  void Finish();
  
  /*!
   * Set the sub device specification
   * \param spec the subdev spec markup string.
   */
  virtual void set_subdev_spec(const std::string &spec)=0;
  
  /*! Set sample rate for USRP device.
   * \param rate a new rate in Sps
   */
  virtual void set_samp_rate(double rate) = 0;
  
  /*! Get sample rate for USRP device.
   * This is the actual sample rate and may differ from the rate set.
   * \param return actual rate in Sps
   */
  virtual double get_samp_rate(void) =0;
  
  /*! Tune the USRP device to the desired center frequency.
   * \param freq the desider frequency in Hz.
   * \return a tune result width the actual frequencies
   */
	virtual uhd::tune_result_t set_center_freq(double freq)=0;
	
 /*! Get the tunnable frequency range.
  * \return the frequency range in Hz.
  */	
	virtual uhd::freq_range_t get_freq_range(void) =0;
 
  /*!	Set the gain for the dboard.
   * \param gain the gain in dB
   */
   virtual void set_gain(float gain)=0;
   
   /*! 
    * Get the actual dboard gain settings.
    * \return the actual gain in dB
    */
    virtual float get_gain(void)=0;
    
    /*! Get the settable gain range.
     * \return the gain range in dB
     */
   virtual uhd::gain_range_t get_gain_range(void)=0;
   
   /*! 
    * Set the antenna tu use.
    * \param ant the antenna string 
    */
    virtual std::string get_antenna(void)=0;
    
    /*! 
     * Get a list of possible antennas.
     * \return a vector of antenna string
     */ 
    
    virtual std::vector<std::string>get_antennas(void)=0;
	
	/*! 
	 * Set the clock configuration.
	 * \param clock_config the new configuraion
	 */
	virtual void set_clock_config(const uhd::clock_config_t &clock_config) =0;
	
	/*!
	 *  Get the current time registeres.
	 * \return the current usrp time
	 */
	
	virtual uhd::time_spec_t get_time_now(void)=0;
	
	/*!
	 * Set the time registers asap .
	 * \param time_spec the new time
	 */
	virtual void set_time_next_pps (const uhd::time_spec_t &time_spec)=0;
	
	
	/*! 
	 * Get access to the underlying uhd device object.
	 * \return the simples usrp device object.
	 */
	virtual usrp::multi_usrp::sptr get_device(void)=0;
   
}; 
 
#endif 
 
