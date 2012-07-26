//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP2_MAIALLOCATOR_H 
#define __MUDISP2_MAIALLOCATOR_H

#include "mudisp.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_blas.h"
#include "gsl/gsl_math.h"
#include "gsl/gsl_rng.h"
#include "gsl/gsl_sort_vector.h"

//
// SOAR Artificial Intelligence Support 
//
#include "sml_Client.h"

using namespace sml;

////////
////////   Here put your personal includes
////////

// Channel Matrix 
//

class MAIAllocator : public Block {
 private:

  ////////   Parameters instances

  IntParam J,N,Mode,M; 
  StringParam SoarFn;
  
  ////////   Local Attributes
 
  unsigned int curruser,framecount, numberCommands, input_time;
  gsl_matrix_uint *Hperm;
  gsl_permutation *p; 
  gsl_vector *huserabs;
  gsl_vector_uint *nextcarr;
  gsl_vector_uint *usedcarr, *errs;
  gsl_matrix *habs; 



  // Carrier Allocation Matrices
  gsl_matrix_uint * signature_frequencies, * signature_frequencies_init; 
  gsl_matrix *signature_powers;
  gsl_matrix_complex * transform_mat, * Hmat, * huu;
  gsl_vector_complex *Hchan;
  gsl_rng * ran;
  
  // SOAR related
  Kernel *pKernel;
  Agent *pAgent;

  /* Identifier *inputLinkId, *carMapId, *usrMapId, *channelsId, *allMapId; */
  /* IntElement *ncarrIntWme; */
  /* vector <Identifier *> carrIdVec; */
  /* vector <IntElement *> cidIntWmeVec; */
  /* vector <StringElement *> usedStringWmeVec; */

  Identifier *pInputLink, *carrMapID, *userMapID, *channelsID;
  IntElement *wmeNusers, *wmeMaxerr, *wmeTime;
  FloatElement  *wmePstep;

  struct SoarUserMapStruct {
    vector < Identifier * > userIDVec;
    vector < IntElement * > wmeUidVec;
    vector < IntElement * > wmeRequiresVec;
    vector < IntElement * > wmeErrsVec;
  };

  struct SoarChannelStruct {
    vector < Identifier * >    coeffIDMat;
    vector < IntElement * >    wmeUserMat;
    vector < IntElement * >    wmeCarrMat;
    vector < FloatElement * >  wmeValueMat;
  };

  SoarUserMapStruct soarUserMap;
  SoarChannelStruct soarChannelMap;

public:

  ////////   InPorts and OutPorts

  //
  // Channel coefficient 
  //
  InPort < gsl_matrix_complex > min1;

  //
  // Error reports (once every ERROR_REPORT_INTERVAL frames, i.e. runs) 
  //
  InPort < gsl_vector_uint > vin2;

  
  //
  // Allocation Matrices
  //
  OutPort < gsl_matrix_uint > mout1;  
  OutPort < gsl_matrix > mout2;  



  MAIAllocator():Block("MAIAllocator")
    ////////  parameters initializazion
    ,M("NumUsers",2,"number of users")
    ,N("Carriers",16,"number of carriers")
    ,J("CodedSymbs",16,"coded symbols")
    ,Mode("AllocatorMode",0,"0=fca,1=bst,2=swp,3=ovl,4=SOAR") // note: leave default to non SOAR mode
    ,SoarFn("SoarFn","./soar-agents/crai-rev2.soar","SOAR agent filename")
    {

      //////// local parameter registration
      AddParameter(M);
      AddParameter(N);
      AddParameter(J);
      AddParameter(Mode);
      AddParameter(SoarFn);

    }

  ~MAIAllocator() {
  }

  void Setup();
  void Run();
  void Finish();

  
};

#endif /* __MUDISP_MYBLOCK_H  */

