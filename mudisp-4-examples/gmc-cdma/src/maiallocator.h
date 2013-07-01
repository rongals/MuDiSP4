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

  IntParam K,Nb,J,N,Mode,M;
  StringParam SoarFn;
  
  ////////   Local Attributes
 
  unsigned int curruser,framecount, ericount, numberCommands, input_time, max_errors, noDecisions;
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
 // ^io                      ^io
 //   ^input-link              ^output-link
 //     ^usrmap                  ^command
 //       ^maxerr                  ^name assign-free
 //       ^pstep                   ^uid
 //       ^pmax                    ^deassign
 //       ^user      []            ^assign
 //         ^uid     []          ^command
 //         ^errs    []            ^name swap-carriers
 //         ^power   []            ^u1
 //         ^carr    [[]]          ^c1
 //           ^cid   [[]]          ^u2
 //           ^power [[]]          ^c2
 //     ^channels                ^command
 //       ^coeff     []            ^name increase-power
 //         ^user    []            ^uid
 //         ^carr    []            ^cid
 //         ^value   []          ^command
 //     ^carrmap                   ^status
 //       ^carr      []
 //         ^cid     []
 //     ^input-time


  Kernel *pKernel;
  Agent *pAgent;

  Identifier *pInputLink; // from here the input link structure

  Identifier *umap;
  IntElement *umapMaxerr;
  FloatElement *umapPstep;
  FloatElement *umapPmax;
  vector < Identifier * > umapUserVec;
  vector < IntElement * > umapUserUidVec;
  vector < IntElement * > umapUserErrsVec;
  vector < FloatElement * > umapUserPowerVec;

  vector < Identifier * > umapUserCarrMat;
  vector < IntElement * > umapUserCarrCidMat;
  vector < FloatElement * > umapUserCarrPowerMat;

  Identifier *chans;
  vector < Identifier * >    chansCoeffMat;
  vector < IntElement * >    chansCoeffUserMat;
  vector < IntElement * >    chansCoeffCarrMat;
  vector < FloatElement * >  chansCoeffValueMat;

  Identifier *carmap;
  vector < Identifier * >    carmapCarrVec;
  vector < IntElement * >    carmapCarrCidVec;

  IntElement *inputTime;

  //
  // helper functions
  //
  void AssignFree(std::string & sUid, 
		  std::string & sDeassign,
		  std::string & aAssign);

  void SwapCarriers(std::string & sU1, 
		    std::string & sC1,
		    std::string & sU2,
		    std::string & sC2);

  void IncreasePower(std::string & sUid, 
		     std::string & sCid);

  void UpdateInputLink();

public:

  ////////   InPorts and OutPorts

  //
  // Channel (power/instantaneous) coefficient
  //
  InPort < gsl_matrix_complex > min1;

  //
  // Error reports (always consumed but updated once every ERROR_REPORT_INTERVAL frames, i.e. runs)
  //
  InPort < gsl_vector_uint > vin2;
  
  //
  // Allocation Matrices
  //
  OutPort < gsl_matrix_uint > mout1;  
  OutPort < gsl_matrix > mout2;  



  MAIAllocator():Block("MAIAllocator")
    ////////  parameters initializazion
    ,K("SourceSymbs",16,"source symbols")
    ,Nb("BitSymb",2,"bits per symbol (PSK mapping)")
    ,M("NumUsers",2,"number of users")
    ,N("Carriers",16,"number of carriers")
    ,J("CodedSymbs",16,"coded symbols")
    ,Mode("AllocatorMode",0,"0=fca,1=bst,2=swp,3=ovl,4=SOAR") // note: leave default to non SOAR mode
    ,SoarFn("SoarFn","./soar-agents/crai-rev2.soar","SOAR agent filename")
    {

      //////// local parameter registration
      AddParameter(K);
      AddParameter(Nb);
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

