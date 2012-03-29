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

#ifndef __MUDISP2_BLOCK_HXX
#define __MUDISP2_BLOCK_HXX

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


#include <cstdlib>
#include <string>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <map>
#include <deque>
#include <list>
#include <algorithm>


#include <mudisp/engine/parlist.h>
#include <mudisp/engine/port.h>
#include <mudisp/engine/lcm.h>

// #include <pthread.h>



/*!
 * Block - Block Definistions
 *This class define the member function to create a base block. 
 * 
 */

//namespace block{
#define NAMELEN 255

/*! @class Block block.h "engine/block.h"
 *  @brief This is a Main Block Class
 *  @author Luca Simone Ronga
 *  @version   3
 *  @date      1999-2011
 *  @pre       First initialize the system.
 *  @warning   Core use can crash your application
 * 	@ingroup Core
 */ 
class Block;

/*! 
 *  Define \b enum  of runtime state.
 */
typedef enum {
	BLK_IN,   /**< Block State from \a InPort , 0 */
	BLK_OUT,  /**< Block State from \a OutPort, 1 */
	BUSY,	  /**< Block on Busy State,  	    2 */
	READY,    /**< Block on Ready State, 	    3 */
	RUN,	  /**< Block on Run State, 	    4 */
	DONE,	  /**< Block Done 		    5 */
	WAIT,	  /**< Block on Waiting State 	    6 */
	NONE	  /**< No State Set 		    7 */
} status;

	

/*! @class Link block.h "engine/block.h"
 *  @brief This is a Link Class
 *  @author Luca Simone Ronga
 *  @version   3
 *  @date      1999-2011
 *  @pre       First initialize the system.
 *  @warning   Core use can crash your application
 *  @ingroup Core
 */ 

class Link {
public:
/** @var Block &a 
 * @brief Pointer reference  to source block */
 /** @var Block &b 
 * @brief Pointer reference  to sink block */
  Block &a, &b;
  
  /** @var Portk &ap 
 * @brief Pointer reference  to source block port */
   /** @var Portk &bp 
 * @brief Pointer reference  to sink block port */
  Port &ap, &bp;
/** @fn Link()
 * @brief CLass Constructor */
  Link();
  
  /** @fn Link(Block & aa, Port & aapp, Block  & bb, Port & bbpp): a(aa),b(bb),ap(aapp),bp(bbpp)
   * @brief Class constructor 
   * @param aa Source Block
   * @param aapp Source Block Port
   * @param bb Sink Block
   * @param bbpp Sink Block Port
   * */
   
  Link(Block & aa, Port & aapp, 
       Block  & bb, Port & bbpp):
    a(aa),b(bb),ap(aapp),bp(bbpp){}

};

//
// TLink Class
//
/*! @class TLink block.h "engine/block.h"
 *  @brief This is a Transport Link Class
 *  @author Luca Simone Ronga
 *  @version   3
 *  @date      1999-2011
 *  @pre       First initialize the system.
 *  @warning   Core use can crash your application
 *  @ingroup Core
 */ 

class TLink {
public:
/** @var Block &a 
 * @brief Pointer reference  to transport block */
 
  Block &a;
  /** @var Port &ap
   * @brief Transport block sink port
   * */
  /** @var Port &tp
   * @brief Transport block source port
   * */
   
  Port &ap, &tp;
/** @fn TLink()
 * @brief CLass Constructor */
  TLink();
  
  /** @fn TLink(Block & aa, Port & aapp, Port & ttpp): a(aa),ap(aapp),tp(ttpp)
   * @brief Class constructor 
   * @param aa Transport Block
   * @param aapp Sink Block Port
   * @param ttpp Source Block Port
   * */
  TLink(Block & aa, Port & aapp, 
       Port & ttpp):
    a(aa),ap(aapp),tp(ttpp){}

};


//
// block class declaration
//
/*! @class Block block.h "engine/block.h"
 *  @brief Block class declaration
 *  @author Luca Simone Ronga
 *  @version   3
 *  @date      1999-2011
 *  @pre       First initialize the system.
 *  @warning   Core use can crash your application
 *  @ingroup Core
 */
class Block {
protected:
  /*! \var unsigned int AbsoluteRun  
   * \brief Define Number of times will the block runs for one run sequence */
  unsigned int AbsoluteRun;
public:
  // status blk_state;
  /** @typedef boost::mutex::scoped_lock lock;
   * @brief block status aviable
   * */
typedef boost::mutex::scoped_lock lock;
  
  
  // 
  // Condional Variable for Controll
  //
  /** @var boost::condition stable_br
   * @brief Condional Variable for Controll 
   * */
  boost::condition stable_br;
  
  //
  // Thread  globale mutex per gestione delle operazioni di I/O su schermo 
  //
  
  //
  // Identity Attributes
  //
  /** @var string ClassName
   * @brief Block Class Identity Attributes
   * */
  string ClassName;
  /** @var  unsigned int InstID
   * @brief Block ID Identity Attributes
   * */
  unsigned int InstID;
  /** @var string BlockName
   * @brief Block Name Identity Attributes
   * */
  string BlockName;
  
  /** @fn status blkstate
   * @brief Define block state contenitor */
 status blkstate;
  //
  // this map holds the newly generated class types derived from
  // Block
  //
  /** @fn static map < string , unsigned int> class_names
   * @brief This map holds the newly generated class types derived from  Block
   * */
  static map < string , unsigned int> class_names;

  //
  // Runtime Related Attributes
  //
  // the following attributes are manipulated by
  // AddToRunSequence() method
/** @var deque < Block * > run_seq
 * @brief Runtime Related Attributes. run sequence express the order of execution 
   for the blocks linked to this one
   each block may be present more than once
   * */
  // run sequence express the order of execution 
  // for the blocks linked to this one
  // each block may be present more than once
  deque < Block * > run_seq;


/** @var map < Block *, unsigned int > block_map
 * @brief Runtime Related Attributes.   Block_map express the list of blocks linked
   to this one, with the clock rate relative to
   Run() clock
   * */
  // block_map express the list of blocks linked
  // to this one, with the clock rate relative to
  // Run() clock
  map < Block *, unsigned int > block_map;

/** @var deque < Link > link_list
 * @brief Runtime Related Attributes.  link_list stores the mapping of the registered links
   along with the rates of the related ports
   * */
  // link_list stores the mapping of the registered links
  // along with the rates of the related ports
  //
  deque < Link > link_list;

  
  /** @var list < Link > blk_link_list_out
 * @brief Runtime Related Attributes. link_list stores the mapping of the registered links
 * along with the rates of the related ports
   * */
   /** @var list < Link > blk_link_list_in
 * @brief Runtime Related Attributes. link_list stores the mapping of the registered links
 * along with the rates of the related ports
   * */  
  // link_list stores the mapping of the registered links
  // along with the rates of the related ports
  //
  list < Link > blk_link_list_out;
  list < Link > blk_link_list_in;
  
    /** @var deque < TLink > tlink_list
 * @brief Runtime Related Attributes. tlink_list stores the mapping of the registered links  between the outports and inports and the trans-ports
   * */
  // tlink_list stores the mapping of the registered links
  // between the outports and inports and the trans-ports
  //
  deque < TLink > tlink_list;
 
   //
  // Parameters related attributes
  //
 /** @var  ParamList param_list
  * @brief Parameters related attributes
  * */
  ParamList param_list;
  
  
  
  //
  // Constructors and Methods
  //
  
   /** @fn Block()
  * @brief Constructors and Methods
  * */
  Block();
/** @fn Block(const char *)
  * @brief Constructors and Methods, width block name
  * */
  Block( const char * );

  virtual ~Block();

  //
  // Runtime
  //
      /** @fn virtual void Build()
   * @brief Build service routine */
  virtual void Build();
  
      /** @fn virtual void _ReadParameters( string filename, vector <string> base_name)
   * @brief Dump block parameter 
   * @param filename Filename to read in parameters
   * @param base_name Vector of parameter name */	
   
  virtual void _ReadParameters( string filename,
		        vector <string> base_name);
    /** @fn virtual void _DumpParameters(ostream &os, vector <string> base_name)
   * @brief Dump block parameter 
   * @param os Standar Out for parameter
   * @param base_name Vector of parameter name */		        
  virtual void _DumpParameters(ostream &os, 
		        vector <string> base_name);
		        
      /** @fn virtual void _ReadParameters(string filename)
		* @brief Read block parameter
		* @param filename File from read parameter for Block */
  virtual void _ReadParameters(string filename);
    /** @fn virtual void _DumpParameters(ostream &os)()
   * @brief Dump block parameter 
   * @param os Standar Out for parameter*/
  virtual void _DumpParameters(ostream &os);
    /** @fn virtual void Setup()
   * @brief Setup service routine */
  virtual void Setup();
    /** @fn virtual void Run()
   * @brief Run service routine */
  virtual void Run();
  /** @fn virtual void Finish()
   * @brief Finish service routine */
  virtual void Finish();

  //private:
/*! \fn  status GetBlkState()
 * \brief Operator for return Block runtime State 
 * \return Block Status
 */
  status GetBlkState();
  
  /*! \fn void PrintBlkState() 
   * \brief Std Out Block State 
   */
  void PrintBlkState();
  
  /*
	void SetStateBlockIn(){
	  rn_blk_state = BLK_IN;
  }*/
	  
  //
  // Run Sequence Builder
  //
  /*! \fn void AddToRunSequence(Block& blk , int N=1);
   * \brief Method Add block to run sequence flow graph 
   * \param blk Pointer to a blk will insert on the graph flow
   * \param N Number of time to insert in the Run sequence
   */
  void AddToRunSequence(Block& blk , int N=1);
  
  /*! @fn void ThreadClassInitialize(int papone)
   * @brief to redefine 
   * @param papone ID*/
  void ThreadClassInitialize(int papone);
    /*! @fn void ThreadClassMax(int papone)
   * @brief to redefine 
   * @param papone ID*/
  void ThreadClassMax(int papone);
  
      /** @fn  template <class DATATYPE> void Connect(Block &b1, 
					  OutPort <DATATYPE> &b1p,
					  Block &b2,
					  InPort <DATATYPE> &b2p)
					  * @brief Realize connection from two block.
					  * @param b1 Source Block
					  * @param b1p Source block reference connection port
					  * @param b2 Sink Block
					  * @param b2p Sink block destination port 
					  * @tparam DATATYPE template type for data fow in graph
					  * */
  template <class DATATYPE> void Connect(Block &b1, 
					 OutPort <DATATYPE> &b1p,
					 Block &b2,
					 InPort <DATATYPE> &b2p){
    
    // blocks registration
    block_map[&b1]=1;
    block_map[&b2]=1;
    
    // link registration
    Link newlink( b1,b1p, b2,b2p);    
    link_list.push_back( newlink );
    
    
    b1.blk_link_list_out.push_front(newlink);	 							//<-- Inserisce il link nella lista dei link in uscita al blocco 1   
	b2.blk_link_list_in.push_front(newlink);								//<-- Inserisce il link nella coda dei link in ingresso al blocco 2
   // b1.blk_link_list.unique();											//<--- Controlla che tale link sia unico
    
    // do the connection
    b1p.ConnectTo( b2p );

  }
  
        /** @fn  template <class DATATYPE> void Connect(Block &b1, 
					 OutPort <DATATYPE> &b1p,
       					 TransPort <DATATYPE> &tp1)
					  * @brief Realize connection from block to Transport Block.
					  * @param b1 Source Block
					  * @param b1p Source block reference connection port
					  * @param tp1 Sink port for Transport Block
					  * @tparam DATATYPE template type for data fow in graph
					  * */
  template <class DATATYPE> void Connect(Block &b1, 
					 OutPort <DATATYPE> &b1p,
       					 TransPort <DATATYPE> &tp1){
    
    // blocks registration
    block_map[&b1]=1;
    
    
    // link registration
    TLink newlink( b1,b1p,tp1);    
    tlink_list.push_back( newlink );
    
    // do the connection
    b1p.ConnectTo( tp1 );
  }
  
          /** @fn template <class DATATYPE> void Connect(TransPort <DATATYPE> &tp1,
					 Block &b1,
					 InPort <DATATYPE> &b1p)
					  * @brief Realize connection from Transport Block to Block.
					  * @param tp1 Source Transport Block port
					  * @param b1  Sink block 
					  * @param b1p Sink port for Block
					  * @tparam DATATYPE template type for data fow in graph
					  * */
					  
  template <class DATATYPE> void Connect(TransPort <DATATYPE> &tp1,
					 Block &b1,
					 InPort <DATATYPE> &b1p) {
    
    // blocks registration
    block_map[&b1]=1;
    
    
    // link registration
    TLink newlink(b1,b1p,tp1);
    tlink_list.push_back( newlink );
    
    // do the connection
    tp1.ConnectTo( b1p );
  }
  
/**--------------------------------------------------------------------*
 * 
 *--------------------------------------------------------------------*/  
  
      /** @fn  template <class DATATYPE> void DConnect(Block &b1, 
					  OutPort <DATATYPE> &b1p,
					  Block &b2,
					  InPort <DATATYPE> &b2p,
					  DATATYPE init=DATATYPE(0))
					  * @brief Realize connection from two block whithout generate data flow block in case of \a b1 outport is empty. 
					  * More useful in case of flow graph with loop
					  * @param b1 Source Block
					  * @param b1p Source block reference connection port
					  * @param b2 Sink Block
					  * @param b2p Sink block destination port 
					  * @param init internal generate neutral data avoid data flow block state (default = 0) 
					  * @tparam DATATYPE template type for data fow in graph
					  * */ 
					  
  template <class DATATYPE> void DConnect(Block &b1, 
					  OutPort <DATATYPE> &b1p,
					  Block &b2,
					  InPort <DATATYPE> &b2p,
					  DATATYPE init=DATATYPE(0)){
    
    // blocks registration
    block_map[&b1]=1;
    block_map[&b2]=1;

#ifdef MUDISP_VERBOSE
    // link registration is not performed for Delayed Links 
    cout << "Ci Sono " << endl;
    cout << "undefine " <<  b1.GetAbsoluteRun() << endl; 
#endif

    // do the connection
    b1p.SetRuns(b1.GetAbsoluteRun());

#ifdef MUDISP_VERBOSE
    cout << "undefine " <<  b1.GetAbsoluteRun() <<  " and "<< b1p.GetRuns() << endl; 
#endif

    b1p.DelayConnectTo( b2p,init );
  }
  
/*!-------------------------------------------------------------------
 * 
 * 
 * 
 * -------------------------------------------------------------------*/
    /** @fn template <class DATATYPE> void DAConnect(Block &b1, 
					  OutPort <DATATYPE> &b1p,
					  Block &b2,
					  InPort <DATATYPE> &b2p,
					  DATATYPE init=DATATYPE(0))
					  * @brief Realize connection from two block whithout generate data flow block in case of \a b1 outport is empty. 
					  * More useful in case of flow graph with loop
					  * @param b1 Source Block
					  * @param b1p Source block reference connection port
					  * @param b2 Sink Block
					  * @param b2p Sink block destination port 
					  * @param init internal generate neutral data avoid data flow block state (default = 0) 
					  * @tparam DATATYPE template type for data fow in graph
					  * */ 
    template <class DATATYPE> void DAConnect(Block &b1, 
					  OutPort <DATATYPE> &b1p,
					  Block &b2,
					  InPort <DATATYPE> &b2p,
					  DATATYPE init=DATATYPE(0)){
    
    // blocks registration
    block_map[&b1]=1;
    block_map[&b2]=1;
    
    // link registration is not performed for Delayed Links
    
    boost::mutex io_mutex; 
    boost::condition go;
    typedef boost::mutex::scoped_lock scoped_lock;   
//    boost::thread thrd(boost::bind(boost::bind(&OutPort<DATATYPE>::AdvancedDelayConnectTo, b1p),  b2p, init));
//    thrd.join();
  
  b1p.AdvancedDelayConnectTo( b2p,init);
    // do the connection
   //    boost::thread thrd1(boost::bind(&Block::ThreadClassInitialize, b1 , 3));
    //   boost::thread thrd2(boost::bind(&Block::ThreadClassMax, b1 , 3));
    //   thrd1.join();
    //   thrd2.join();
     // b1.ThreadClassInitialize(3);
 //  b1p.AdvancedDelayConnectTo( b2p,init);
    
#ifdef MUDISP_VERBOSE    
    cout << " Delay  Advanced Connect  " <<  b1.GetAbsoluteRun() << endl;
    cout << b2p.GetRate()*GetAbsoluteRun() << endl;
#endif

  }
  
  
 
 /* 
  template <class DATATYPE>  void DelayConnectLaunch(OutPort template <DATATYPE> &b1p, InPort template <DATATYPE> &b2p, DATATYPE init) { 
	  b1p.AdvancedDelayConnectTo(b2p,init);
	  } 
  */
  
  //
  /*! \fn void SequenceFinder()
   * \brief Analizza la lista dei blocchi e le interconnessioni e cerca di trovare una sequenza di esecuzione stabile dal punto di vista delle interconnessioni e dei rete di esecuzione dei blocchi e delle porte.
   */
  void SequenceFinder();

  //
  // Parameters related method
  //
/** @fn void AddParameter( GenericParam &par )
 * @brief Add the parameter to the list
 * */
  // add the parameter to the list.
  void AddParameter( GenericParam &par );

/** @fn template <class PType > void SetParameter( string pName, PType  val ) 
 * @brief set statically the param value
 * @param pName Parameter Name
 * @param val Value of parameter
 * @tparam PType Template parameter type  
 * */
  // set statically the param value
  template <class PType > void SetParameter( string pName, 
					    PType  val ){
    param_list.Set(pName,val);
  }

  // the parameter is exported to the container block
  /** @fn  void ExportParameter( string pName )
   * @brief The parameter is exported to the container block
   * @param pName parametr name */
  void ExportParameter( string pName );

/** @fn void SetAbsoluteRun(unsigned int abs_run)
 * @brief Set block Absolute Run
 * @param abs_run Block absolute run
 * */
 
  void SetAbsoluteRun(unsigned int abs_run){
	  AbsoluteRun = abs_run;
  }
  
  /** @fn void SetLink( list < Link > link_index) 
   * @brief Set block link
   * @param link_index  index of block intreconnection list*/
   void SetLink( list < Link > link_index ){
	 // link_list  = link_index;
  }
  
  
  
 // status GetBlkState();
/*  void SetBlock_In(){
  STATE=BLK_IN;
  }
 */
/** @fn void SetDone()
 * @brief Set state of blck Done */
 void SetDone();

  //
  // Gets the absolute run rate of the block 
  // 
/** void PrintList()
 * @brief Gets the absolute run rate of the block and print Block List if no errors occurrence
 * */
void PrintList(){
 cout << "mylist contains: " << endl;
  for (list<Link>::iterator it=blk_link_list_out.begin(); it!=blk_link_list_out.end(); ++it)
    //cout << " " << (*it).ap->Elements() <<  (*it).bp->Elements() << endl;
    cout << " Output List " << (*it).a.BlockName << " --> " << (*it).b.BlockName << " "<< endl;
  for (list<Link>::iterator it=blk_link_list_in.begin(); it!=blk_link_list_in.end(); ++it)
    cout << " Input List " << (*it).b.BlockName << " --> " << (*it).a.BlockName << endl;
    
}
/** void PrintPortRate()
 * @brief Check flow graph and print Port Rate */
void PrintPortRate(){
 cout << "mylist contains: " << endl;
  for (list<Link>::iterator it=blk_link_list_out.begin(); it!=blk_link_list_out.end(); ++it)
    //cout << " " << (*it).ap->Elements() <<  (*it).bp->Elements() << endl;
    cout << " Output List " << (*it).a.BlockName << " --> " << (*it).bp.Elements() << " "<< endl;   
}

/*!bool TestUnique( list<Link>(first), list<Link>(second)){
 if ((first).a ==(second).a  &&  (first).b ==(second).b )
 return true;
 }
*/
/** void Unique()
 * @brief check for errate recursion on flow connection */
void Unique(){
	list<Link>::iterator it=blk_link_list_out.begin();
	list<Link>::iterator it2=blk_link_list_out.begin();
	while ( it!=blk_link_list_out.end() && blk_link_list_out.size() >1 ){
		it2++;
		if (it2!=blk_link_list_out.end() ){
			if((*it).a.BlockName== (*it2).a.BlockName && (*it).b.BlockName== (*it2).b.BlockName ){ 
				it = blk_link_list_out.erase(it);
				it--;
			}
		}
		++it;
	}
	
	it=blk_link_list_in.begin();
    it2=blk_link_list_in.begin();
	while ( it!=blk_link_list_in.end() && blk_link_list_in.size() >1 ){
		it2++;
		if (it2!=blk_link_list_in.end() ){
			if((*it).a.BlockName== (*it2).a.BlockName && (*it).b.BlockName== (*it2).b.BlockName ){ 
				it = blk_link_list_in.erase(it);
				it--;
			}
		}
		++it;
		//}
	}
}
/** @fn unsigned int GetAbsoluteRun()
 * @brief Method for get Block absolute ranning rate
 * @return running rate for current poter block */
 
  unsigned int GetAbsoluteRun(){
	  return AbsoluteRun;
	  }
private:
  /** @fn boost::mutex monitor
   * @brief Define a mutex monitor for class 
   * */
  boost::mutex monitor;
  
  /** @var  boost::condition cond_var
   * @brief define condition variable for control mutex state 
   * */
  boost::condition cond_var;
  //
  // 
  /*! \struct executer 
   * \brief Crea una struttura unaria per eseguire l'overload del metodo run, si passa dell'implementazione virtuale del metodo \p run() all'implementazione fisica associata al blocco. 
   * \param  *to_exe puntatore del blocco a cui è associata la funzione unaria da sostituire,
   * \param void Tipo di ritorno della funzione chiamata, in questo caso il tipo è \a void pèoiché la funzione  run() non resituisce alcun tipo.
   * 
   * /n Tale metodo deve essere modificato nell'esecuzione del blocco come singolo thread.*/
  struct executer : public unary_function<Block *, void>
  {
	/** @fn  void operator()(Block *to_exe)
	 * @brief Executer service rutine for current block. Launch every time block go in ececution mode  
	 * */
    void operator()(Block *to_exe) { 
	//	cout<< to_exe->GetAbsoluteRun() << endl;
#ifdef MUDISP_VERBOSE		
	if ( (to_exe->GetBlkState())==7){
		cout << "From Block " << to_exe->BlockName << " " ;
		to_exe->PrintList();
		to_exe->Unique();
		cout << endl << "*----------------------------------------------------------------------*" 
		<< endl << "*----------------------------------------------------------------------*"  << endl;
		to_exe->PrintList();
		to_exe->SetDone();
		}
#endif
    	// to_exe->PrintPortRate();										//<-- 

		to_exe->Run();  
		
		//to_exe->
		}
  };

  //
  // MapReset
  //
  /** @fn void ResetClassNames()
   * @brief Reset Map 
   * */
  void ResetClassNames();

};

//} // namespace block
#endif /* __MUDISP_BLOCK_HXX */


