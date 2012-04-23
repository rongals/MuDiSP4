//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

////////////////////////////////////////////////////////
/////////
/////              Template Header
//                      for a
//
//                 CONTAINER BLOCK
//
//

#ifndef __MUDISP2_MYBLOCK_H 
#define __MUDISP2_MYBLOCK_H

#include "mudisp.h"

////////
////////   Here put your personal includes
////////


class MyBlock : public Block {
private:

////////   SubBlocks instances

  SubBlock sb1,sb2;
  
////////   Parameters instances

  FloatParam fp;

////////   Local Attributes

  unsigned int count;

////////   TransPorts

public:

  TransPort <double> in1;
  TransPort <unsigned int> out1;  


  MyBlock():Block("MyBlock")
    
////////  parameters initializazion

    ,fp("ParamName",1.1,"comment ...")

    {

//////// local parameter registration

      AddParameter(fp);

//////// export of sub-block parameter

      sb1.ExportParameter("ParamName");
      sb2.ExportParameter("ParamName");

    }

  void Build();
  void Setup();
  
};

#endif /* __MUDISP_MYBLOCK_H  */

////////////////////////////////////////////////////////
/////////
/////              Template Header
//                      for a
//
//                 PRIMITIVE BLOCK
//
//

#ifndef __MUDISP2_MYBLOCK_H 
#define __MUDISP2_MYBLOCK_H

#include "mudisp.h"

////////
////////   Here put your personal includes
////////


class MyBlock : public Block {
private:

////////   Parameters instances

  FloatParam fp;

////////   Local Attributes

  unsigned int count;

////////   InPorts and OutPorts

  InPort <double> in1;
  OutPort <unsigned int> out1;  

public:

  MyBlock():Block("MyBlock")
    
////////  parameters initializazion

    ,fp("ParamName",1.1,"comment ...")

    {

//////// local parameter registration

      AddParameter(fp);

    }

  void Setup();
  void Run();
  void Finish();

  
};

#endif /* __MUDISP_MYBLOCK_H  */

