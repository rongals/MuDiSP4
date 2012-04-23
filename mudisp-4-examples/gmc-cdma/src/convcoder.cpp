// CONVOLUTIONAL VITERBI-ENCODER --> (K-1)*n MAX : 32
// ELEMENTI DELLA MATRICE DELLE CONNESSIONI IN FORMATO DECIMALE INTENDENDO
// IL MSB QUELLO PIU' VICINO ALLE USCITE. 
// LA MATRICE DEVE ESSERE N X M CON N NUM INGRESSI E M NUM DI USCITE.
//


#include "convcoder.h"


void ConvCod :: Setup () {
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
  i1.SetRate(n);
  o1.SetRate(kout);
 
  /////// CONSTRAINT LENGHT COMPUTATION  ( MAX CONNECTIONS NUMBER ) /////////
 
  
  K=0;
  index=0;
  for (int i=0;i<n;i++)
    for (int j=0;j<kout;j++) 
      if (transfun[i][j]>K) K=transfun[i][j];
  for (double p=0;pow(2,p)<=double(K);p++) index+=1;
  K=index;
  
  // K   = constraint length
  // K-1 = number of delay states
}
void ConvCod :: Run () {
  
  for (int i=0;i<n;i++) {
    Status[i]=Status[i]<<1;
    Status[i]=Status[i]|i1.GetDataObj();
  }
  for (int j=0;j<kout;j++){         
    bool Other=false;
    for(int i=0;i<n;i++){            
      tempout=Status[i] & transfun[i][j];  
      for(int nbit=0;nbit<K;nbit++){     
	if (nbit==0 & !Other) out=tempout&1;     
	else out=(tempout&1)^out;
	tempout=tempout >> 1;
      }
      Other=true; 
    }
    o1.DeliverDataObj(out);
  }
}
