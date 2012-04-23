//
//
//
//
//

#ifndef __MUDISP2_CONVCODER_H
#define __MUDISP2_CONVCODER_H

#include "mudisp.h"
#include <iostream>
#include "tnt.h"
#include "vec.h"
#include "cmat.h"
#include <math>

using namespace std;
using namespace TNT;

class ConvCod : public Block {
  
  private :
    
  StringParam filename;
  
  ifstream ifile;
  Matrix <unsigned int> transfun;
  int kout,n,K,index;
  Vector <unsigned int> Status;
  unsigned int out,tempout;
    
  public :
  
  InPort <unsigned int> i1;
  OutPort <unsigned int> o1;
  
  ConvCod() : Block("Convolutional_Encoder"),
    filename("Inifile","convolutional/matrix12.dat","Initialization_File"){
    AddParameter(filename);
  }
  
  void Setup();
  void Run();

};
 
#endif /* __MUDISP_CONVCODER_H */
