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

////////
////////   Here put your personal includes
////////

// Channel Matrix 
//

class Allocator : public Block {
 private:

  ////////   Parameters instances

  IntParam J,N; 
  
  ////////   Local Attributes
 
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
    {

      //////// local parameter registration
      AddParameter(N);
      AddParameter(J);

    }

  ~Allocator() {
  }

  void Setup();
  void Run();


  
};

#endif /* __MUDISP_MYBLOCK_H  */

