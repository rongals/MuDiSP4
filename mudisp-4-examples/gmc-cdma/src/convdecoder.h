//
//
//
//
//

#ifndef __MUDISP2_CONVDECODER_H
#define __MUDISP2_CONVDECODER_H

#include "mudisp.h"
#include <iostream>
#include "tnt.h"
#include "vec.h"
#include "cmat.h"
#include <math>

using namespace std;
using namespace TNT;

class ConvDecod : public Block {
  
  private :
  
  StringParam filename;

  ifstream ifile;
  Matrix <unsigned int> 
    transfun,TM,TMB,TMF,
    BRANCHMAT,STATE_MET_MAT,
    PATH_STATE_MAT,TSM;
  int 
    kout,n,K, L, 
    index,s,d,minbranch;
  unsigned int 
    out,tempout,output,
    nstates,mask,actinput,
    DestStatus,State,dest,
    cmp,tmpbranch,received,
    branch,src,decinfo,
    minsrcmet,min,pathmet,
    minpath,minstate;
  Vector <unsigned int> 
    Status,TempStatus,
    TempDestStatus,in,
    OUT,source,desti;
  
  public :
  InPort <unsigned int> i1;
  OutPort <unsigned int> o1;
  
  ConvDecod() : Block("Convolutional_Decoder"),
    filename("Inifile","convolutional/matrix12.dat","Initialization_File"){
    AddParameter(filename);
  }
  
  void Setup();
  void Run();

};
 
#endif /* __MUDISP_CONVDECODER_H */
