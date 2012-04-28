//
// MuDiSP3 (Multirate Digital Signal Processor 3)
// Copyright 1999, Luca Simone Ronga
//
//
//  This file is part of MuDiS3.
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


#ifndef __MUDISP2_PORT_HXX
#define __MUDISP2_PORT_HXX


#include <stack>
#include <list>
#include <queue>

#include <cstdlib>
#include <string>
#include <sstream>
#include <stdio.h>
#include <iostream>

// #include <pthread.h>
/*-------------------------------------------------------------------
 * 					librerie threads
 * 
 * 
 * N.B. per la conpilazione, utilizzare le l'estensione 
 * 
 * -l:/usr/lib/libboost_thread-mt.so
 * -------------------------------------------------------------------*/

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/bind.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/thread/detail/config.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/thread/thread_time.hpp>


/*! Devo definire una routine per il mio threads */



/*! Inizio della fase di elaborazione della porta */

using namespace std;


/*! @class Port port.h <mudisp/engine/port.h>
 *  @brief Port class declaration
 *  @author Luca Simone Ronga
 *  @version   3
 *  @date      1999-2011
 *  @pre       First initialize the system.
 *  @warning   Core use can crash your application
 *  @ingroup Core
 */
class Port {
protected:
 /** @var unsigned int rate 
     @brief Tasso In(Out) della porta */
  unsigned int rate;
 /** @var  unsigned int runs 
	@brief Numero d esecuzioni della porta */
  unsigned int runs;
public:

 /*! @typedef boost::mutex::scoped_lock  lock
  *  \brief definisco la mia variabile di blocco  \a lock.
  * 
  */ 
	typedef boost::mutex::scoped_lock  lock;
/*	
 /*! \var  boost::condition stable_br;
  *  \brief definisco la mia variabile condizionale  \a stable_br.
  * 
  */ 
  // boost::condition stable_br;
/** @fn  Port(unsigned int rt, unsigned int rs):rate(rt),runs(rs) 
* @brief Class constructor
* @param rt Port Rate
* @param rs Port Runs
*/
  Port(unsigned int rt, unsigned int rs):rate(rt),runs(rs){ }

/** @fn unsigned int GetRate()
 * @brief Method for get rate
 * @return Rate 
 * */
  unsigned int GetRate(){ return rate; };
/** @fn void SetRate( unsigned int rt )
 * @brief Method for set port Rate
 * @param rt Port Rate
 * */
  void SetRate( unsigned int rt ){ rate=rt; }
 /** @fn unsigned int GetRuns()
  * @brief Method for get port numbers of runs 
  * @return Numbero of runs for port
  * */
  unsigned int GetRuns(){ return runs; };
  /** @fn void SetRuns( unsigned int rs )
   * @brief Method for Set Port Runs parameter
   * @param rs Numbers of runs for port */
  void SetRuns( unsigned int rs ){ runs=rs; }
  /** @fn virtual int Elements()
   * @brief Method for get numbers of elements in the queue
   * @return Elements on the queue
   * */
  virtual int Elements(){ return 0; }
 // void Set_Thread_cond_var( boost::condition in_cond_var) {
 //   stable_br = in_cond_var;
 // }
  ~Port(){
  }
  
};

// typedef struct 

template <class DATATYPE> class InPort;

//
// OutPort objects store the data object 
// produced by blocks.
/** @class OutPort port.h <mudisp/engine/port.h>
 * @brief  OutPort objects store the data object produced by blocks.
 * @tparam DATATYPE Port data type precision
 * */
template <class DATATYPE> class OutPort : virtual public Port {
  
private:
  
  /** @var list < InPort <DATATYPE> * > deliv_list
   * @brief Data Delivery List
   * */
  // 
  // Data Delivery List
  //
  list < InPort <DATATYPE> * > deliv_list;
  //
  /** @var boost::mutex Port_Monitor
   * @brief Port monitor mutex variable */
  boost::mutex Port_Monitor;

public:

   /*!  Mettiamo qui dentro la dichiarezione del threads */
  
  
  /*{ \var  boost::mutex stable_mutex;
   *  \brief definisco il thread con accesso in mutua esclusione.
   */
 //  boost::mutex stable_mutex;

/** @fn OutPort(unsigned int rt=1, unsigned int rs=1 ): Port(rt,rs)  
 * @brief Constructor for the class
 * @param rt Port data rate
 * @param rs Port Runs */
   OutPort(unsigned int rt=1, unsigned int rs=1 ):
  Port(rt,rs){
	 /// Construttore della classe, inserire qui la parte riguardante il noise loop.

  }
  
  /** @fn void ConnectTo( InPort <DATATYPE> & dest )
   * @brief Method for create connection Builder
   * @param dest Destination InPort 
   * @tparam DATATYPE Port Data type precision */ 
  // 
  // Connection Builder
  //
  void ConnectTo( InPort <DATATYPE> & dest ) {
    deliv_list.push_front( &dest );
  }


  // 
  // Thread Service Routine
  /*! Controlla che la fase di Building del System si sia conclusa 
 * raggiungendo la stabilità dei rate di trasmissione 
 */
/*! \fn void *watch_stable() 
 * \brief Routine di gestione del thread. Mette il thread in attesa che la variabile condizionale
 * generi l'evento per lo sblocco dell'elaborazione.
 */ 

 void watch_stable(){
  int ID =0;
   // faccio il lock del mio mutex.
  /// boost::mutex::scoped_lock lock(stable_mutex);
  std::cout << " entrata del thread " << ID << std::endl;
   ///  stable_br.wait(lock);
   std::cout << " uscita del thread " << ID << std::endl;
   /// lock.unlock(); 
}



  /*!   Ecco il perché del mio problema   */
  //
  // Delayed Connection Builder // <-- Ecco il perche del problema
  //
  
  /** @fn void DelayConnectTo( InPort <DATATYPE> & dest, DATATYPE init=DATATYPE(0) )
   * @brief Method for create connection Builder for loop or in case of block OutPort 
   * data are unaviable for a runs sequence, this method create a neutral data element define by 
   * user.
   * @param dest Destination block InPort  
   * @param init Data internal generate if OutPort queue are empty 
   * @warning This method is high instable and in some applicatin can create memory segmentation fault.
   * */ 
  void DelayConnectTo( InPort <DATATYPE> & dest, 
		       DATATYPE init=DATATYPE(0) ) {
    deliv_list.push_front( &dest );
    lock lk(OutPort::Port_Monitor);
  /*!  A questo punto devo inserire il mio thread di controllo che blocca 
   * l'esecuzione allo statu mutex_lock fino a quando non si è stabilizzato 
   * i rate di esecuzione.
   */
        for (int i=0; i<GetRate(); i++){
		    dest.PutDataObj(init);
		}
		lk.unlock();
  }
  
 /** @fn void watch_stable_internal(int ID)
  * @brief Print the thread ID in the standard output
  * @param ID id to print in the standard output
  * */
  	void watch_stable_internal(int ID){
		 std::cout << " ID " << ID; 
    }
 
 
   /** @fn void AdvancedDelayConnectTo( InPort <DATATYPE> & dest, DATATYPE init=DATATYPE(0) )
   * @brief Method for create connection Builder for loop or in case of block OutPort 
   * data are unaviable for a runs sequence, this method create a neutral data element define by 
   * user. 
   * @param dest Destination block InPort  
   * @param init Data internal generate if OutPort queue are empty 
   * @note This mehod is more robust than previous method, and work greate when  DelayConnectTo faoult
   * */ 
  // Delayed Connection Builder
  //
 void AdvancedDelayConnectTo( InPort <DATATYPE> & dest, 
		       DATATYPE init=DATATYPE(0)) {
		
	unsigned int rep;
    deliv_list.push_front( &dest );
     for (int i=0; i<GetRate()*GetRuns(); i++){
		    dest.PutDataObj(init); // Istanzia almeno il primo.
	} 
	#ifdef MUDISP_DEBUG_VERBOSE
	std::cout << "Fine del Delay Connect " << endl;
	#endif
 }
  
  
 

/** @fn void DeliverDataObj( const DATATYPE & dt )
 * @brief Data Access Functions the block which owns the port put the data
 * @param dt Data to send 
 * */ 
  //
  // Data Access Functions
  // the block which owns the port put the data
  void DeliverDataObj( const DATATYPE & dt ) {
	  // Put resurce on block state
	lock lk(OutPort::Port_Monitor);
	// Send data to alla block in deliverable list.
    typename list< InPort <DATATYPE> * >::iterator i = deliv_list.begin();
    for (unsigned int n=deliv_list.size();n>0;n--) {
      (*i++)->PutDataObj(dt);
    }
    // Unlock the resurce
    lk.unlock();
  }
/** @fn void watch_stable_ed()
 * @brief Print Exectuion state on standard Output
 * */
     void watch_stable_ed(){
		 int ID=2;
  std::cout << "entrata del thread " << ID << std::endl;
   std::cout << "uscita del thread " << ID << std::endl;

} 

}; 

/** @class InPort port.h <mudisp/engine/port.h> 
 * @brief InPort objects store the data object to be consumed. Data are 
 * pushed by the OutPorts
 * @tparam DATATYPE Variable datatype
*/
//
// InPort objects store the data object 
// to be consumed. Data are pushed by the
// OutPorts
template <class DATATYPE> class InPort : virtual public Port {
  
private:
/** @var queue < DATATYPE > port_data_buffer
 * @brief Queue Data Repository
 * @tparam DATATYPE Data type precision 
 * */ 
  //
  // Queue Data Repository
  //
  queue < DATATYPE > port_data_buffer;
 /** @var boost::mutex Port_Monitor
  * @brief Mutex Variable
  * */
  // Mutex Variable
  boost::mutex Port_Monitor;
public:
  //
  // contructors
  //
  /** @fn  InPort(unsigned int rt=1, unsigned int rs=1): Port(rt,rs)
   * @brief Constructor for class 
   * @param rt Port Data rate
   * @param rs Port number of runs*/
  InPort(unsigned int rt=1, unsigned int rs=1):
    Port(rt,rs){}


    virtual ~InPort(){};
 
 /** @fn virtual void PutDataObj( const DATATYPE & data) 
  * @brief  Data Access Functions. The block which owns the port put the data
  * @param data Data to store in the port queue 
  * */
    //
    // Data Access Functions
    //
    // the block which owns the port put the data
    virtual void PutDataObj( const DATATYPE & data) {
		lock lk(InPort::Port_Monitor);
		port_data_buffer.push(data);
      lk.unlock();
    }
    
    /*! \fn int Elements() 
     * @brief Method for get number of elements in the queue
     * \return  number of elements inside the \a InPort Buffer.
     * */
    int Elements(){
		lock lk(InPort::Port_Monitor);
		lk.unlock();
	return port_data_buffer.size();	
	}
	
	/*! \fn bool Empty() 
	 * @brief Check state of Data queue
	 * \return State of data buffer, return \b true if \a InPort buffer if empty
	 */
    bool Empty(){
	return port_data_buffer.empty();
	}
	
    // the receiving blocks access it
#ifdef OLD_PORT
  // Deprecate version of GetDataObj for Old Code
    DATATYPE GetDataObj(DATATYPE init=DATATYPE(0)){
	//  DATATYPE data= DATATYPE(0);
	 DATATYPE data;
	 
	 lock lk(InPort::Port_Monitor);
		if (!port_data_buffer.empty())
			{	
				data=port_data_buffer.front();
				port_data_buffer.pop();    
			} else {
				data = init;
			}
		lk.unlock();
		return data;
		
    }
#endif

#ifndef OLD_PORT   
 /*! @fn DATATYPE GetDataObj()
 * @brief \b pop one element from data queue.
 * @tparam DATATYPE Tamplate data type format
 * @return One element from queue
 */ 
    DATATYPE GetDataObj(){
      lock lk(InPort::Port_Monitor);
        DATATYPE data=port_data_buffer.front();
		port_data_buffer.pop();
      lk.unlock();  
      return data;
    } 

 /*! @fn DATATYPE GetDataObj(DATATYPE init)
 * @brief \b if queue is non-empty then pop one element from data queue
 * @param init default element returned if the queue is empty
 * @tparam DATATYPE Template data type format
 * @return One element from queue
 */ 
 
 
    DATATYPE GetDataObj(DATATYPE &init){
      
      lock lk(InPort::Port_Monitor);
        if (!port_data_buffer.empty()) {	
	  DATATYPE data=port_data_buffer.front();
	  port_data_buffer.pop();
	  lk.unlock();
	  return data;
	} else {
	  DATATYPE data=init;
	  lk.unlock();  
	  return data;
	}
    } 
#endif 
}; 



/*! @class TransPort  port.h "mudisp/engine/port.h"
  * \brief This class implement a way to sub system block made by several base block.
  * @tparam DATATYPE Template data type format
*/
template <class DATATYPE> class TransPort : public InPort <DATATYPE>,
  public OutPort <DATATYPE> {

  public:
  /** @typedef boost::mutex::scoped_lock lock
   * @brief Define a scoped_lock for class 
   * */
  typedef boost::mutex::scoped_lock lock;
  
  /*! \fn TransPort(unsigned int rt=1, unsigned int rs =1): Port(rt, rs )
  / \brief contructors
  * \param rt Transport Port rate
  * \param rs Transport block runs
  */
  TransPort(unsigned int rt=1, unsigned int rs =1):
    Port(rt, rs ){}

/** @var boost::mutex Trans_Monitor
 * @brief Create a mutex state fariable dor monitoring data */
	boost::mutex Trans_Monitor;
  
  /*! @fn void PutDataObj( const DATATYPE & data) 
   * @brief  Data Access Functions a transport forward the data to the connected sub-blocks
   * @param data Data object
   */   
  void PutDataObj( const DATATYPE & data) {
	lock lk(Trans_Monitor);
	DeliverDataObj( data );
    lk.unlock();
  }
};


#endif /* __MUDISP_PORT_HXX */
