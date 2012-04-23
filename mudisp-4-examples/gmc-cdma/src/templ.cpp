//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

////////////////////////////////////////////////////////
/////////
/////               Template Code
//                      for a
//
//                 CONTAINER BLOCK
//
//


#include "templ.h"


void MyBlock::Build() {

////////  connection between the blocks inside MyBlock
////////  Connect( block, block.port, block, block.port);


  Connect(sb1,sb1.out1,sb2,sb2.in1);

////////  connection between blocks and transports
////////   Connect( block, block.port, transport );
////////   Connect( transports, block, block.port); 

  Connect( in1, ,sb1, sb1.in1 );
  Connect( sb2, sb2.out1, out1 );
  
////////  PLEASE, REMEMEBER THIS

  Block::Build();

}


void MyBlock::Setup() {

//////// set of sub-blocks parameters

  sb1.SetParam("SubParam", fp() * 10 );


}



////////////////////////////////////////////////////////
/////////
/////               Template Code
//                      for a
//
//                 PRIMITIVE BLOCK
//
//


#include "templ.h"


void MyBlock::Setup() {

//////// initialization of dynamic data structures

  vint = Vector( ip() );

//////// rate declaration for ports

  out1.SetRate( ip() );

}


void MyBlock::Run() {

//////// fetch data objects

  tmp1 = in1.GetDataObj();
  tmp2 = in1.GetDataObj();
  ...

//////// production of data

  out1.DeliverDataObj( tmp );
  out1.DeliverDataObj( tmp );
  out1.DeliverDataObj( tmp );
  ...

}


void MyBlock::Finish() {

//////// post processing

  error_probability = err_count / data_count;

}
