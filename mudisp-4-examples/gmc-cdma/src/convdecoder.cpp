// 
//  
// CONVOLUTIONAL VITERBI-DECODER --> (K-1)*n MAX : 32
// 
//


#include "convdecoder.h"


void ConvDecod :: Setup () {
  string fn( filename() );
  ifile.open( fn.c_str() );
  if (! ifile) {
    cerr << BlockName << "error opening " << fn.c_str() <<endl;
    exit(_ERROR_OPEN_FILE_);
  }
  ifile >> transfun;
  ifile.close();
  n=transfun.num_rows();     // Number of inputs
  kout=transfun.num_cols();  // Number of Outputs   
 
  Status=Vector <unsigned int> (n);
  TempStatus = Vector <unsigned int> (n);
  TempDestStatus = Vector <unsigned int> (n);
  in = Vector <unsigned int> (n);
  OUT = Vector <unsigned int> (n);
  
  i1.SetRate(kout);
  o1.SetRate(n);
  
  /////// CONSTRAINT LENGHT COMPUTATION  ( MAX CONNECTIONS NUMBER ) /////////
  
  K=0;
  index=0;
  for (int i=1;i<n+1;i++)
    for (int j=1;j<kout+1;j++)
      {
	if (transfun(i,j)>K) K=transfun(i,j);
      }
  for (double p=0;pow(2,p)<=double(K);p++) index+=1;
  K=index;            
  L=5*K;
  
  //
  // K    = constraint length
  // L    = Trellis lenght
  // n    = information bits
  // kout = coded bits
  // 
  // total number of states = 2**(K-1)*n
  
  nstates=1;
  nstates=(1 << ((K-1)*n));
  mask=1;
  mask=(( 1 << (K-1))-1) ;
  source = Vector <unsigned int> (L);
  desti = Vector <unsigned int> (L);
  
  TM = Matrix <unsigned int> (nstates,(1<<n));
  TMB = Matrix <unsigned int> (nstates,nstates);
  TMF = Matrix <unsigned int> (nstates,nstates);  
  TSM = Matrix <unsigned int> (nstates,nstates);
  BRANCHMAT = Matrix <unsigned int> (nstates,nstates);
  STATE_MET_MAT = Matrix <unsigned int> (nstates,L);
  PATH_STATE_MAT = Matrix <unsigned int> (nstates,L);
  
  BRANCHMAT=0xFFFFFFFF;   /// metrics are initialized to the maximum value : ((2**32)-1)
    
                 ///////////////    STATE TRANSITION MATRIX INITIALIZATION   ////////////////
  for (unsigned int stat=0;stat<nstates;stat++){
    State=stat;
    for (int i=n-1;i>=0;i--){
      Status[i]=State & mask;       //// conversion of the u-int number representing the state in the state n-vector //////
      State = State >> (K-1);
      TempStatus[i]=Status[i];
    }
    for (unsigned int input=0;input< (1 << n);input ++){
      DestStatus=0;
      TempStatus=Status;              
      TempDestStatus=Status;
      actinput=input;
      for (int inpos=n-1;inpos>=0;inpos--){
	in[inpos]=actinput & 1;     
	actinput=actinput >> 1;
	TempStatus[inpos]=TempStatus[inpos]<<1;
	TempStatus[inpos]=TempStatus[inpos]|in[inpos];  //// TempStatus is the input of the encoder ! ///
	TempDestStatus[inpos]=TempDestStatus[inpos] << 1;
	TempDestStatus[inpos]=TempDestStatus[inpos] | in[inpos];
	TempDestStatus[inpos]=TempDestStatus[inpos] & mask; 
      }
      for (int destpos=0;destpos<n;destpos++){
	DestStatus=DestStatus << (K-1);
	DestStatus=DestStatus | TempDestStatus[destpos];    ////saving DestState for each combination of start state & input
      }
      
      ///// Encoding Process : 
      
      output=0;
      for (int jj=0;jj<kout;jj++){           
	bool Other=false;
	for(int ii=0;ii<n;ii++){           
	  tempout=TempStatus[ii] & transfun[ii][jj];   
	  for(int nbit=0;nbit<K;nbit++){      
	    if (nbit==0 & !Other) out=tempout&1;     
	    else out=(tempout&1)^out;
	    tempout=tempout >> 1;
	  }
	  Other=true; 
	}
	output=output<<1;
	output=output | out;
      }                            
      
      ////// End of Encoding Process ///////
      
      TM[stat][input]=DestStatus;
      TMB[stat][DestStatus]=output;
      TMF[stat][DestStatus]=input;
      TSM[DestStatus][stat]=1;     ////In TSM I save the possible source states for each destination state ///
    }
  }
}

////////////
////////////   END OF SETUP !!
////////////

void ConvDecod :: Run () {
  received=0;
  cmp=0;
  dest=0;
  decinfo=0;
  
  for(int ninp=0;ninp<kout;ninp++){           
    received = received << 1;
    received = received | i1.GetDataObj();
  }
  
  ///// Branch Metrics Matrix calculation (at time (5*K)-1 of Trellis ) ///////
  
  for(unsigned int state=0;state<nstates;state++){
    for(unsigned int inp=0;inp<(1 << n);inp++){
      dest = TM[state][inp];
      cmp = TMB[state][dest];
      tmpbranch = cmp ^ received;
      branch=0;
      for(int nbit=0;nbit<kout;nbit++){   // Branch metric computation for each possible trans. from present state
	branch+=tmpbranch & 1;   
	tmpbranch=tmpbranch >> 1;
      }
      BRANCHMAT[state][dest]=branch;
    }
  }
 
                      ///////////// UPDATING THE TRELLIS : ///////////////

  for(int ss=0;ss<nstates;ss++)
    for(int time=0;time<(L-1);time++){
      STATE_MET_MAT[ss][time]=STATE_MET_MAT[ss][time+1];
      PATH_STATE_MAT[ss][time]=PATH_STATE_MAT[ss][time+1];
    }
  for(int deststate=0;deststate<nstates;deststate++){
    bool done=false;
    bool ddone=false;
    for(int sourcestate=0;sourcestate<nstates;sourcestate++)
      if(TSM[deststate][sourcestate]==1){
	if (!done){
	  STATE_MET_MAT[deststate][L-1]=
	    STATE_MET_MAT[sourcestate][L-2]
	    +BRANCHMAT[sourcestate][deststate];
	  done=true;
	}
	else if (STATE_MET_MAT[sourcestate][L-2]+BRANCHMAT[sourcestate][deststate]<
		 STATE_MET_MAT[deststate][L-1])
	  STATE_MET_MAT[deststate][L-1]=
	    STATE_MET_MAT[sourcestate][L-2]+
	    BRANCHMAT[sourcestate][deststate];
	if (!ddone){
	  PATH_STATE_MAT[deststate][L-1]=sourcestate;
	  minsrcmet=STATE_MET_MAT[sourcestate][L-2];
	  minbranch=BRANCHMAT[sourcestate][deststate];
	  ddone=true;}
	else if (STATE_MET_MAT[sourcestate][L-2]<minsrcmet){
	  minsrcmet=STATE_MET_MAT[sourcestate][L-2];
	  minbranch=BRANCHMAT[sourcestate][deststate];
	  PATH_STATE_MAT[deststate][L-1]=sourcestate;
	}
	else if (STATE_MET_MAT[sourcestate][L-2]==minsrcmet)
	  if (BRANCHMAT[sourcestate][deststate]<minbranch){
	    minbranch=BRANCHMAT[sourcestate][deststate];
	    PATH_STATE_MAT[deststate][L-1]=sourcestate; 
	  }
      }
  }
  //////// DECISION ROUTINE ////////////
  pathmet=0;
  minpath=0xFFFFFFFF;
  minstate=0;
  
  for(int destination=0;destination<nstates;destination++)
    if (destination==0){
      min=STATE_MET_MAT[destination][L-1];
    }
    else {
      if (STATE_MET_MAT[destination][L-1]<min){
	min=STATE_MET_MAT[destination][L-1];
      }
    }
  
  ////// ho gli stati di partenza a metrica minore per il TB ////////
  
  for (unsigned int destin=0;destin<nstates;destin++){
    if (STATE_MET_MAT[destin][L-1]==min){
      
      desti[L-1]=destin;
      pathmet=STATE_MET_MAT[destin][L-1];
      for (int t=(L-1);t>0;t--){       // trovo il percorso per ogni stato di partenza a met minore
	d=desti[t];
	source[t]=PATH_STATE_MAT[d][t];
	pathmet+=STATE_MET_MAT[source[t]][t-1];  // 
	desti[t-1]=source[t];
      }
      
      if (pathmet<minpath){
	minpath=pathmet;
	minstate=destin;
      }
    }
  }
  desti[L-1]=minstate;
  
  for (int tt=(L-1);tt>0;tt--){
    d=desti[tt];
    source[tt]=PATH_STATE_MAT[d][tt];
    desti[tt-1]=source[tt];
  }
  s=source[1];
  d=desti[1];
  
  decinfo=TMF[s][d];
  
  for(int o=n-1;o>=0;o--){
    OUT[o]=decinfo & 1;
    decinfo=decinfo >> 1;
  }
  for(int oo=0;oo<n;oo++)
    o1.DeliverDataObj(OUT[oo]);
}

//////// STATE_MET_MAT : State metric for each time of Trellis  ////////
//////// PATH_STATE_MAT : Source State with less state_metric for each present state ///////
