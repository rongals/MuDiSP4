//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP2_ALLOCATOR_H 
#define __MUDISP2_ALLOCATOR_H

#include "mudisp.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_complex_math.h"
#include "gsl/gsl_blas.h"
#include "gsl/gsl_math.h"
#include "gsl/gsl_rng.h"
#include "gsl/gsl_sort_vector.h"

////////
////////   Here put your personal includes
////////

// Channel Matrix 
//

class Allocator : public Block {
 private:

  ////////   Parameters instances

  IntParam J,N,Mode; 
  
  ////////   Local Attributes
 
  unsigned int curruser;
  gsl_matrix_uint *Hperm;
  gsl_permutation *p; 
  gsl_vector *huserabs;
  gsl_vector_uint *nextcarr;
  gsl_vector_uint *usedcarr;
  gsl_matrix *habs; 



  // Carrier Allocation Matrix
  gsl_matrix_uint * signature_frequencies, * signature_frequencies_init; 
  gsl_matrix_complex * transform_mat, * Hmat;
  gsl_vector_complex * Hchan;
  gsl_rng * ran;
  
public:

  ////////   InPorts and OutPorts

  //
  // Channel coefficient 
  //
  InPort < gsl_matrix_complex > min1;
  
  //
  // Allocation Matrix
  //
  OutPort < gsl_matrix_uint > mout1;  



  Allocator():Block("Allocator")
    ////////  parameters initializazion
    ,N("Carriers",16,"number of carriers")
    ,J("CodedSymbs",16,"coded symbols")
    ,Mode("AllocatorMode",0,"0=fca,1=bst,2=swp,3=ovl")
    {

      //////// local parameter registration
      AddParameter(N);
      AddParameter(J);
      AddParameter(Mode);

    }

  ~Allocator() {
  }

  void Setup();
  void Run();


  
};

#endif /* __MUDISP_MYBLOCK_H  */

