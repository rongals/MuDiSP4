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

#include <library/base/uhd_mudisp_usrp_sink.h>

void UHD_MuDiSP_USRP_Sink::Setup() {
	
	/*! \fn  uhd::set_thread_priority_safe();
     *  \brief Setta la priorità del thread istanziato per l'esecuzione  
     */
		uhd::set_thread_priority_safe(); 

	//--> Prova ad impostare lo scheduling del thread in modalità realtime,
    // se si genera una eccezzione nell'impostazione dello scheduling viene
    // generato un messaggio di WARNING e viene mantenuta la priorità di 
    // scheduling che il programma aveva prima di invocare la chiamata, il
    // programma procede con l'esecuzione normalmente.
	
	
	 uhd::device_addr_t dev_addr;
		dev_addr["addr0"] = args();
	
	uhd::usrp::single_usrp::sptr sdev = uhd::usrp::single_usrp::make(dev_addr[0]);
	//uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(args());
    uhd::device::sptr dev = sdev->get_device();
    
    
    sdev->set_tx_rate(rate());
    sdev->set_tx_freq(freq());
    sdev->set_time_now(uhd::time_spec_t(0.0));
   
   
#ifdef MUDISP_VERBOSE 
    std::cout << boost::format("Using Device: %s") % sdev->get_pp_string() << std::endl;
    //set the tx sample rate
    std::cout << boost::format("Setting TX Rate: %f Msps...") % (rate/1e6) << std::endl;
  
    std::cout << boost::format("Actual TX Rate: %f Msps...") % (sdev->get_tx_rate()/1e6) << std::endl << std::endl;
    //set the tx center frequency
    std::cout << boost::format("Setting TX Freq: %f Mhz...") % (freq/1e6) << std::endl;
    std::cout << boost::format("Actual TX Freq: %f Mhz...") % (sdev->get_tx_freq()/1e6) << std::endl << std::endl;
    std::cout << boost::format("Setting device timestamp to 0...") << std::endl;
#endif
//////// Initialization of dynamic data structures  
}

 
void UHD_MuDiSP_USRP_Sink::Run() { 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Vector <complex<double>> real_input;
 //  real_input= Vector<complex<double>>(sample());
  // real_input = in1.GetDataObj();      /////////////////////////ingresso fittizio per far partire il blocco
  	uhd::device_addr_t dev_addr;
	dev_addr["addr0"] = args();
	
	uhd::usrp::single_usrp::sptr sdev = uhd::usrp::single_usrp::make(dev_addr[0]);
	
  std::vector<std::complex<float> > buff(spp());
  
  // , std::complex<float>(ampl, ampl)

    //send the data in multiple packets
    size_t num_packets = (sample()+spp()-1)/spp();
    
 for (size_t i = 0; i < num_packets; i++){
        //setup the metadata flags and time spec
        
        md.start_of_burst = true;                  //always SOB (good for continuous streaming)
        md.end_of_burst   = false;  //only last packet has EOB
        // md.has_time_spec  = (i == 0);              //only first packet has time
      //  md.time_spec = uhd::time_spec_t(seconds_in_future);

        size_t samps_to_send = std::min<int>(sample()-spp()*i, spp());
}
        //send the entire packet (driver fragments internally)
       // size_t num_tx_samps = sdev->send(
       //     &buff.front(), samps_to_send, md,
       //     uhd::io_type_t::COMPLEX_FLOAT32,
       //     uhd::device::SEND_MODE_FULL_BUFF,
       //     //send will backup into the host this many seconds before sending:
       //     seconds_in_future + 0.1 //timeout (delay before transmit + padding)
       // );

		md.start_of_burst = false;
		md.end_of_burst   = true;
       sdev->get_device()->send(
            &buff.front(), buff.size(), md,
            uhd::io_type_t::COMPLEX_FLOAT32,
            uhd::device::SEND_MODE_FULL_BUFF
        );


}
 
void UHD_MuDiSP_USRP_Sink::Finish() { 
		md.start_of_burst = false;        
        md.end_of_burst   = true;	
        }          
