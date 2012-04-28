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

#include <mudisp/engine/block.h>

#define MAXSTRLEN 255
#define MAXNUMBERLEN 127

//
// static Attributes
//
map < string , unsigned int> Block::class_names;

//
// this function is used to extract the Class name from the 
// typeid() call.
//

// string extract_name( const type_info & ti) {
  
//   char *buf;
//   int l,lnum;
//   sscanf(ti.name(),"%d",&l);
//   buf=new char[l+1];
//   sscanf(ti.name(),"%*d%s",buf);
//   delete [] buf;
//   return string(buf);
// }


Block::Block():
  AbsoluteRun(1),InstID(0) 
{
  Block("Undefined");
}

Block::Block(const char *new_name):
  AbsoluteRun(1),ClassName(new_name),blkstate(NONE)
{

  if (class_names.find(ClassName) == class_names.end()) {
    //
    // ClassName not found
    //
    class_names[ClassName]=InstID=1;
  } 
  else {
    //
    // ClassName already instantiated once
    //
    InstID = ++class_names[ClassName];
  }
  char buf[MAXNUMBERLEN];
  sprintf(buf,"%d",InstID);

  BlockName = ClassName + "_" + buf;
  
  // Inizializza il cond_init  -->
//  pthread_cond_init (&stable_br, NULL); 

}

//
// the Block destructor
//
// Block::~Block() {}

Block::~Block(){

}

//
// AddToRunSequence
//
// Aggiunge il blocco alla Lista.
void Block::AddToRunSequence(Block& newblock, int N) {
  for (register int i=0; i<N; i++)
    run_seq.push_back(&newblock);
}


/*! Devo Lavorare qui dentro */
//
// SequenceFinder
// finds the run sequence of the registered blocks
//
void Block::SequenceFinder() {
	#ifdef MUDISP_DEBUG_VERBOSE   
		std::cout << " Entro ora nel SequenceFinder() " << std::cout << this << std::endl;
	#endif

//
// I propagate the Sequence Finder through the
// block_map
//
  map < Block * , unsigned int >::iterator iter;
  for ( iter=block_map.begin(); iter!=block_map.end(); iter++ ) {
    (*iter).first->SequenceFinder();
  }
  

//
// Next, two steps are performed: first the rates for each blocks are found,
// then the run sequence is determined.
//

  if (block_map.size()==0)
    return;
  
  bool isStable=false;
  bool isDependent, isSourceInserted;

  unsigned int maxpasses = link_list.size()+1;
  unsigned int passes=0;

  while ( ! isStable ) {

    isStable=true;
    deque < Link >::iterator lk;

    if (passes++ > maxpasses) {
      cerr << BlockName << ": Unable to find a stable rate table!"
	   << endl << "Exiting..." << endl;
      exit(_ERROR_UNSTABLE_RATES_);
    }
    for (lk=link_list.begin(); lk!=link_list.end(); lk++ ) {
      
      unsigned int trate, ar, br;
      
      ar = (*lk).ap.GetRate() * block_map[&(*lk).a];
      br = (*lk).bp.GetRate() * block_map[&(*lk).b];

      trate = LCM(ar,br); 

      if (ar != br) {
	isStable = false;
	block_map[&(*lk).a] = trate / (*lk).ap.GetRate();
	block_map[&(*lk).b] = trate / (*lk).bp.GetRate();

#ifdef MUDISP_VERBOSE

	cout << BlockName << ": " 
	     << "New rates :" << (*lk).a.BlockName 
	     << "[" << block_map[&(*lk).a] 
	     << "] ----> " << (*lk).b.BlockName 
	     << "[" << block_map[&(*lk).b] << "]" << endl;

#endif

      }
      /*!    <---    -- > */
      
      // Devo mettere un puntatore inverso  
      if (isStable){
	   SetAbsoluteRun(block_map[&(*lk).a]);
	   
	   // <-- cout << "Block Run" << "[" << block_map[&(*lk).a] << "]" << endl;
	  }
    }
  }

//
// After the rates let's set the TransPort rates accordingly
//
  deque < TLink >::iterator tlk;
  for (tlk=tlink_list.begin(); tlk!=tlink_list.end(); tlk++ ) {
      unsigned int brate = block_map[&(*tlk).a];
      unsigned int prate = (*tlk).ap.GetRate();
      (*tlk).tp.SetRate( brate*prate );
//      (*tlk).ap.Set_Thread_cond_var(stable_br);  // <-- Settava la variabile condizionale
  }

//
// The second step tries to find the sequence from the rates
// previously discovered
// The brutal method: any block appear with its multiplicity
// on the run sequence

// lk1 is the link iterator
  deque < Link >::iterator lk1;
  map < Block *, unsigned int >::iterator bmi;

  isStable = false;
  while (! isStable) {
    isStable = true;
    for (bmi=block_map.begin(); bmi!=block_map.end(); bmi++) {
      //
      // take a source block
      //
      Block &blk = *(bmi->first);
      //
      // I consider only the blocks not already inserted
      //
      if (block_map[&blk] != 0) {
	//
	// find it as destination
	//
	isDependent=false;
	isSourceInserted = true;
	for (lk1=link_list.begin(); lk1!=link_list.end(); lk1++ ) {
	 
	  if ( blk.BlockName == (*lk1).b.BlockName ) {
	    isDependent=true;
	    // 
	    // the considered block has a source so it is 
            // dependent on the others blocks. I need to
	    // check if all the sources are already inserted in 
	    // the list
	    //
	    Block & srcblk = (*lk1).a;
	    if ( block_map[&srcblk]!=0 ) {
	      //
	      // the source isn't inserted so I cannot
	      // accept the block
	      //
	      isSourceInserted=false;
	      break;
	    }
	  }
	}
	//
	// 
	// I put (*blk) in the runsequence if
	// 
	//          isDependent is FALSE
	// or when
	//          isDependent is TRUE and isSourceInserted is TRUE
	//
	if ( (! isDependent) || (isDependent && isSourceInserted) ) {
	  AddToRunSequence(blk,block_map[&blk]);
#ifdef MUDISP_VERBOSE
	  cout << BlockName << ": Block " 
	       << blk.BlockName << " added to Run Sequence " 
	       << block_map[&blk] << " time/s." << endl; 
#endif
	  blk.SetAbsoluteRun(block_map[&blk]);
	  //<--
      // blk.SetLink(link_list);
      //<-- 
	  block_map[&blk]=0;
	  isStable=false;
	} 
      }
    }
  }
  /*!  Qui dovrei sbloccare il mio thread settando la variabile condizionale */
#ifdef MUDISP_DEBUG_VERBOSE
  std::cout<< "Esco dal SequenceFinder " <<  std::endl; 
#endif   

   stable_br.notify_all(); // <<-- Manda il segnale broadcast a tutti i threads interrorri dalla variabile condizionale
}

//
// If not otherwise specified the Run() method execute the element
// in the run list
//
/**___________________________________________________________________*/

/*! Devo lanciare i threads da qui dentro -->> */
void Block::Build() {
  map < Block * , unsigned int >::iterator iter;
  for ( iter=block_map.begin(); iter!=block_map.end(); iter++ ) {
    (*iter).first->Build();
  }
}

/**___________________________________________________________________*/



void Block::Setup() {
//
// invoked by _ReadParameters() and _DumpParameters()
//
// During read :
//    1> read the parameters
//    2> executes Setup()
//    3> descend the tree
// During dump:
//    1> dump parameters
//    2> executes Setup()
//    3> descend the tree

}

void Block::_ReadParameters(string filename,
			    vector <string> base_name){
  //
  // the new name is built
  //
  base_name.push_back(BlockName);
  
    
  //
  // then the local parameters are read
  //
  param_list.ReadAllParameters( filename, base_name);

  //
  // the Setup() is invoked
  //
  Setup();

  // 
  // the call is propagated to the linked blocks
  map < Block * , unsigned int >::iterator iter;
  for ( iter=block_map.begin(); iter!=block_map.end(); iter++ ) {
    (*iter).first->_ReadParameters(filename,base_name);
  }
}

void Block::_DumpParameters(ostream &os,
			    vector <string> base_name){

  map < Block * , unsigned int >::iterator iter;

  //
  // the new name is built
  //
  base_name.push_back(BlockName);

  //
  // then the parameters are dumped
  //
  if (!param_list.empty()) {
    param_list.DumpAllParameters( os, base_name);
  }

  //
  // executes Setup()
  //
  Setup();


  // 
  // now we propagate the call to the linked blocks
  //
  for ( iter=block_map.begin(); iter!=block_map.end(); iter++ ) {
    (*iter).first->_DumpParameters(os,base_name);
  }
}

void Block::_ReadParameters( string filename){
  vector <string> bn;
  _ReadParameters( filename, bn );
}

void Block::_DumpParameters(ostream &os){
  vector <string> bn;
  _DumpParameters(os,bn);
}



void Block::Run() {
	#ifndef MUDISP_REALTIME
    for_each(run_seq.begin(),run_seq.end(),executer());
    #else
		cout  << " Running Realtime " << endl;
    for_each(run_seq.begin(),run_seq.end(),realtime_executer());
    #endif
}

void Block::Finish() {
  map < Block * , unsigned int >::iterator iter;
  for ( iter=block_map.begin(); iter!=block_map.end(); iter++ ) {
    (*iter).first->Finish();
  }
}

void Block::AddParameter( GenericParam &par ) {
  param_list.AddParameter(par);
}

void Block::ExportParameter( string pName ){
  param_list.ExportParam(pName);
}

void Block::ResetClassNames(){
  class_names.clear();
}


status Block::GetBlkState(){
	  return blkstate;
  }
  
  void Block::SetDone(){
	  blkstate = DONE;
  }

void Block::PrintBlkState(){
	  switch (blkstate){
	case BLK_IN : cout << BlockName << " State on: BLK_IN " << endl;
		break ;
	case BLK_OUT: cout << BlockName << " State on: BLK_OUT " << endl;
		break ;
	case BUSY: cout << BlockName << " State on: BUSY" << endl;
		break ;
	case READY: cout << BlockName << " State on: READY " << endl;
		break ;
	case RUN: cout << BlockName << " State on: RUN " << endl;
		break ;
	case DONE: cout << BlockName << " State on: DONE " << endl;
		break ;
	case WAIT: cout << BlockName << " State on: WAIT " << endl;
		break ;
	case NONE: cout << BlockName << " State on: NONE " << endl;
		break ;
  }
}

/*

void Block::ThreadClassInitialize(int papone){
//	boost::mutex mutex;
//	scoped_lock lock(mutex);
	
	std::cout << "Test 2" <<  papone <<std::endl;
	stable_br.wait(lock);
	std::cout << "Test out from" <<  papone <<std::endl;
	//boost::this_thread::sleep(boost::posix_time::milliseconds(750)); // <-- Piattaforma  di sincronizzazione
	lock.unlock();
};
* 
* */

void Block::ThreadClassInitialize(int papone){
	lock lk(Block::monitor);
	std::cout << "Test 3" <<  " " <<  papone << &stable_br << std::endl;
	cond_var.wait(lk);
	std::cout << "Sblocco del blocco" <<  " " <<  papone <<std::endl;
};

void Block::ThreadClassMax(int papone){
	lock lk(Block::monitor);
	std::cout << "Inizializzo il thread di servizio" << &stable_br <<  std::endl;
	boost::this_thread::sleep(boost::posix_time::milliseconds(1500)); // <-- Piattaforma  di sincronizzazione
	//std::cout << "Test 3" <<  " " <<  papone <<std::endl;37
	lk.unlock();
	cond_var.notify_all();
	std::cout << "Uscita dal thread di servizio" << std::endl;
};
