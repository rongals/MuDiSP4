//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "aiallocator.h"
#include "gmccdma.h"

// FIXED_ALLOCATION
// each user is assigned a fixed set of carriers chosen with the modulus function
// sig(mu,j)=(j M + mu)
//
// GIVE_BEST_CARR
// first we sort the channel responses of each user, then a first user is selected randomly and the best J carriers 
// are assigned in roud-robin fashion (give N > JM)
// 
//
// SWAP_BAD_GOOD
// starting from a fixed allocation, we swap the worst carrier of current users with the best carrier used by others.
// Users served in round-robin with random start.
//
//
// BEST_OVERLAP
// each user takes her best J carriers possibly overlapping with others
//
//
// SOAR_AI
// artificial intelligence SOAR connection
//
//





//#define SHOW_POWER
//#define SHOW_MATRIX

//
//
// Matrix display
//
//
extern void gsl_matrix_uint_show(gsl_matrix_uint *mat);
extern void gsl_matrix_complex_show(gsl_matrix_complex *mat);


void AIAllocator::Setup() {

//////// initialization of dynamic data structures

  Hchan = gsl_vector_complex_alloc(N());
  Hmat = gsl_matrix_complex_alloc(N(),USERS);
  Hperm = gsl_matrix_uint_alloc(N(),USERS);
  p = gsl_permutation_alloc(N());
  huserabs = gsl_vector_alloc(N());
  nextcarr = gsl_vector_uint_alloc(USERS);
  usedcarr = gsl_vector_uint_alloc(N());
  habs = gsl_matrix_alloc(N(),USERS);

  //
  // Random Generator
  //
  ran = gsl_rng_alloc( gsl_rng_default );

  // SIGNATURE FREQUENCIES INITIAL SETUP

  signature_frequencies = gsl_matrix_uint_alloc(USERS,J());
  signature_frequencies_init = gsl_matrix_uint_alloc(USERS,J());

  for (int i=0; i<USERS; i++)
    for (int j=0; j<J(); j++)
      gsl_matrix_uint_set(signature_frequencies_init,i,j,(j*USERS+i) % N());

  //
  // INITIAL ALLOCATION
  //
  gsl_matrix_uint_memcpy(signature_frequencies,
			 signature_frequencies_init);


  //
  //
  //  FFT Transform Matrix
  //  
  // 
  transform_mat = gsl_matrix_complex_calloc(N(),N());
  double fftarg=-2.0*double(M_PI/N());
  double fftamp=1.0/sqrt(double(N()));

  for (int i=0; i<N(); i++)
    for (int j=0; j<N(); j++)
      gsl_matrix_complex_set(transform_mat, i,j, gsl_complex_polar(fftamp,fftarg*i*j) );


  switch (Mode()) {
  case 0:
    cout << BlockName << " - Allocator type FIXED_ALLOCATION selected" << endl;
    break;
  case 1:
    cout << BlockName << " - Allocator type GIVE_BEST_CARR selected" << endl;
    break;
  case 2:
    cout << BlockName << " - Allocator type SWAP_BAD_GOOD selected" << endl;
    break;
  case 3:
    cout << BlockName << " - Allocator type BEST_OVERLAP selected" << endl;
    break;
  case 4:
    cout << BlockName << " - Allocator type SOAR_AI selected" << endl;

    //
    //
    // SOAR INITIALIZATION
    //
    //
    // Create an instance of the Soar kernel in our process
    //pKernel = Kernel::CreateKernelInNewThread() ;
    pKernel = Kernel::CreateRemoteConnection() ;
    
    // Check that nothing went wrong.  We will always get back a kernel object
    // even if something went wrong and we have to abort.
    if (pKernel->HadError())
      {
	cerr << BlockName << ".SOAR - " << pKernel->GetLastErrorDescription() << endl ;
	exit(1);
      }
    
    // We check if an agent has been prevoiusly created, otherwise we create it 
    // NOTE: We don't delete the agent pointer.  It's owned by the kernel
    pAgent = pKernel->GetAgent("AIAllocator") ;
    if (! pKernel->IsAgentValid(pAgent)) {
      pAgent = pKernel->CreateAgent("AIAllocator") ;
    }
    
    
    // Check that nothing went wrong
    // NOTE: No agent gets created if there's a problem, so we have to check for
    // errors through the kernel object.
    if (pKernel->HadError())
      {
	cerr << BlockName << ".SOAR - " << pKernel->GetLastErrorDescription() << endl ;
	exit(1);
      }
    
    //
    // load productions
    //
    pAgent->LoadProductions("/usr/local/sandbox_ronga/git/MuDiSP4/mudisp-4-examples/gmc-cdma/aiallocator.soar");
    
    // Check that nothing went wrong
    // NOTE: No agent gets created if there's a problem, so we have to check for
    // errors through the kernel object.
    if (pKernel->HadError())
      {
	cerr << BlockName << ".SOAR - " << pKernel->GetLastErrorDescription() << endl ;
	exit(1);
      }
    
    //
    // END OF SOAR INITIALIZAZION
    //
    
    break;  
  default:
    cerr << BlockName << " - Unhandled allocator type !" << endl;
    exit(1);
  }
  

  //////// rate declaration for ports


}


void AIAllocator::Run() {

  // fetch data objects
  gsl_matrix_complex cmat  =  min1.GetDataObj();




  //
  // Hmat = Fourier( cmat )
  //
  gsl_blas_zgemm(CblasNoTrans,
		 CblasTrans,
		 gsl_complex_rect(sqrt(double(N())),0),
		 transform_mat,
		 &cmat,
		 gsl_complex_rect(0,0),
		 Hmat);

  //
  // ***********************************************************
  // CARRIER ALLOCATION STRATEGIES
  // ***********************************************************
  //
    gsl_matrix_uint_memcpy(signature_frequencies,
			   signature_frequencies_init);

  switch (Mode()) {

  case 0: // FIXED_ALLOCATION

    break;

  case 1: // GIVE_BEST_CARR

  //
  // SORT CARRIERS OF EACH USERS
  //

  for(int u=0; u<USERS; u++) {

    gsl_vector_complex_const_view huser = gsl_matrix_complex_const_column(Hmat,u);
    gsl_vector_uint_view sortindu = gsl_matrix_uint_column(Hperm,u);

    for (int j=0; j<N(); j++) {
      double currpower = gsl_complex_abs2(gsl_vector_complex_get(&huser.vector,j));
      gsl_vector_set(huserabs,j,currpower);
    }

    gsl_sort_vector_index(p,huserabs);

    for (int j=0; j<N(); j++) {
      uint currindex = p->data[j];
      gsl_vector_uint_set(&sortindu.vector,j,currindex);
    }
    
  }

  //
  // FIND INITIAL USER RANDOMLY
  //
  curruser = gsl_rng_uniform_int(ran,USERS);
  
 
  //
  // ASSIGN FREQUENCIES
  //
  gsl_vector_uint_set_all(nextcarr,0);
  gsl_vector_uint_set_all(usedcarr,0);
  for (int j=0; j<J(); j++) {
    for (int uu=0; uu<USERS; uu++) {
      int u = (uu+curruser) % USERS;
      int isassigned = 0;
      while (! isassigned) {
	int tag = gsl_vector_uint_get(nextcarr,u);
	gsl_vector_uint_set(nextcarr,u,++tag);
	int carrier = gsl_matrix_uint_get(Hperm,N()-tag,u);
	if (! gsl_vector_uint_get(usedcarr,carrier)) {
	  isassigned = 1;
	  gsl_vector_uint_set(usedcarr,carrier,isassigned);
	  gsl_matrix_uint_set(signature_frequencies,u,j,carrier);
	} else if (tag==N()) {
	  cerr << "Block: " << BlockName << " allocation problem." << endl;
	  exit(1);
	}
      }
    }
  }

  //
  // sort signatures 
  //
  //   for (int u=0; u<USERS; u++) {
  //     gsl_vector_uint_view signature_u = 
  //       gsl_matrix_uint_row(signature_frequencies,u);
  //     gsl_sort_vector_uint(&signature_u.vector);
  //   }
  
 
  //
  // show channels and permutations 
  //
  //  gsl_matrix_complex_show(Hmat);
  //gsl_matrix_uint_show(Hperm);
  //gsl_matrix_uint_show(signature_frequencies);

    break;

  case 2: // SWAP_BAD_GOOD

  //
  // HPERM CONSTRUCTION
  //

  // SORT CARRIERS OF EACH USERS
  //


  for(int u=0; u<USERS; u++) {

    gsl_vector_complex_const_view huser = gsl_matrix_complex_const_column(Hmat,u);
    gsl_vector_uint_view sortindu = gsl_matrix_uint_column(Hperm,u);
    gsl_vector_view huserabs = gsl_matrix_column(habs,u);

    for (int j=0; j<N(); j++) {
      double currpower = gsl_complex_abs2(gsl_vector_complex_get(&huser.vector,j));
      gsl_vector_set(&huserabs.vector,j,currpower);
    }


    //
    // sort channels for user <u>
    //
    gsl_sort_vector_index(p,&huserabs.vector);


    for (int j=0; j<N(); j++) {
      uint currindex = p->data[j];
      gsl_vector_uint_set(&sortindu.vector,j,currindex);
    }

  }

  //
  // Hperm(N,USERS) contains sorted channels index for each users
  // habs(N,USERS) contains channel energy per each user
  //
  
  //
  // FIND INITIAL USER RANDOMLY for fairness
  //
  curruser = gsl_rng_uniform_int(ran,USERS);
  
 
  //
  // ASSIGN FREQUENCIES
  //

  //
  // for each user ...
  //
  for (int uu=0; uu<USERS; uu++) {
    int u = (uu+curruser) % USERS;

 
    //
    // worst allocated channel for user u
    //
    double worstvalue=GSL_POSINF;
    unsigned int worstjindex;
    for (int j=0; j<J(); j++) {
      unsigned int chind = gsl_matrix_uint_get(signature_frequencies,u,j);
      double currh = gsl_matrix_get(habs,chind,u);
	if (currh < worstvalue) {
	  worstvalue = currh;
	  worstjindex = j;
	}
      }


    //
    // find best channel allocated by other users
    // 
    //
    double bestvalue=0;
    unsigned int bestuser, bestjindex;
    for (int uuu=0; uuu<USERS-1; uuu++) {
      unsigned int otheru = (uuu+u) % USERS;
      for (int j=0; j<J(); j++) {
	unsigned int chind = gsl_matrix_uint_get(signature_frequencies,otheru,j);
	double currh = gsl_matrix_get(habs,chind,otheru);
	if (currh > bestvalue) {
	  bestvalue = currh;
	  bestjindex = j;
	  bestuser = otheru;
	}
      }
    }


    //
    // finally the swap !
    //
    unsigned int chind = gsl_matrix_uint_get(signature_frequencies,u,worstjindex);
    gsl_matrix_uint_set(signature_frequencies,u,worstjindex,
			gsl_matrix_uint_get(signature_frequencies,bestuser,bestjindex));
    gsl_matrix_uint_set(signature_frequencies,bestuser,bestjindex,chind);


//    cout << "\n\nProcessing user " << u << endl
// 	 << "\tSwapped " << u << "." << worstjindex 
// 	 << " <-> " << bestuser << "." << bestjindex << endl;
    

  }


  break;
  case 3:   //  BEST_OVERLAP

  //
  // SORT CARRIERS OF EACH USERS
  //

  for(int u=0; u<USERS; u++) {

    gsl_vector_complex_const_view huser = gsl_matrix_complex_const_column(Hmat,u);
    gsl_vector_uint_view sortindu = gsl_matrix_uint_column(Hperm,u);

    for (int j=0; j<N(); j++) {
      double currpower = gsl_complex_abs2(gsl_vector_complex_get(&huser.vector,j));
      gsl_vector_set(huserabs,j,currpower);
    }

    gsl_sort_vector_index(p,huserabs);

    for (int j=0; j<N(); j++) {
      uint currindex = p->data[j];
      gsl_vector_uint_set(&sortindu.vector,j,currindex);
    }
    
  }

 
  //
  // each user take his best carriers allowing carrier overlap
  //
  for (int u=0; u<USERS; u++) {
    for (int j=0; j<J(); j++) {
      int carrier = gsl_matrix_uint_get(Hperm,N()-j-1,u);
      gsl_matrix_uint_set(signature_frequencies,u,j,carrier);
    }
  }
 
  //
  // show channels and permutations 
  //
  //gsl_matrix_complex_show(Hmat);
  //gsl_matrix_uint_show(Hperm);
  //gsl_matrix_uint_show(signature_frequencies);

  break;
  case 4:   //  SOAR_AI

    //
    // SOAR - we populate the input link with elements from Hperm
    //
    Identifier* pInputLink = pAgent->GetInputLink();
    Identifier* pID = pAgent->CreateIdWME(pInputLink,"spectrum");

    //    IntElement* pWME1 = pAgent->CreateIntWME(pID,"n-users",USERS);
    //    IntElement* pWME2 = pAgent->CreateIntWME(pID,"n-ucarr",J());
    // IntElement* pWME3 = pAgent->CreateIntWME(pID,"n-carriers",N());
    // Identifier* pID2 = pAgent->CreateIdWME(pInputLink,"hperm");

    //
    // SORT CARRIERS OF EACH USERS
    // Hperm(i,u) = i-th sorted carrier index of user u (worst carrier first)
    //   
    for(int u=0; u<USERS; u++) {
      
      gsl_vector_complex_const_view huser = gsl_matrix_complex_const_column(Hmat,u);
      gsl_vector_uint_view sortindu = gsl_matrix_uint_column(Hperm,u);

      for (int j=0; j<N(); j++) {
	double currpower = gsl_complex_abs2(gsl_vector_complex_get(&huser.vector,j));
	gsl_vector_set(huserabs,j,currpower);
      }
      
      gsl_sort_vector_index(p,huserabs);
      
      for (int j=0; j<N(); j++) {
	uint currindex = p->data[j];
	gsl_vector_uint_set(&sortindu.vector,j,currindex);

	// String identifierName("hperm-");

	ostringstream idName;

	idName << "helem-" << j << "-" << u; 

	//cout << idName.str() << endl;
	Identifier* pID2 = pAgent->CreateIdWME(pID,idName.str().c_str());
	
	IntElement* pWMEu =  pAgent->CreateIntWME(pID2,"user",u);
	IntElement* pWMEj =  pAgent->CreateIntWME(pID2,"carrier",j);
	IntElement* pWMEv =  pAgent->CreateIntWME(pID2,"index",currindex);

      }
      
    }
    
 
    //    pAgent->Commit();
    pAgent->RunSelf(1);

    
   
    //
    // show channels and permutations 
    //
    //gsl_matrix_complex_show(Hmat);
    //gsl_matrix_uint_show(Hperm);
    //gsl_matrix_uint_show(signature_frequencies);
    
    break;
  }
  
  
  //#ifdef SHOW_MATRIX
  //  gsl_matrix_complex_show(Hmat);
  //  gsl_matrix_uint_show(signature_frequencies);
  //#endif
  
  //
  // SHOW POWER USER 0
  //
  //   double power = 0.0;
  //   for (int j=0; j<J(); j++) {
  //     int carrier = gsl_matrix_uint_get(signature_frequencies,0,j);
  //     power += gsl_complex_abs2(gsl_matrix_complex_get(Hmat,carrier,0));
  //   }
  //   cout << "\ncurrent allocated power user 0 = " << power << endl;
  //   power = 0.0;
  //   for (int j=0; j<J(); j++) {
  //     int carrier = gsl_matrix_uint_get(signature_frequencies_init,0,j);
  //     power += gsl_complex_abs2(gsl_matrix_complex_get(Hmat,carrier,0));
  //   }
  //   cout << "fixed  allocated power user 0 = " << power << endl;

  //////// production of data
  mout1.DeliverDataObj( *signature_frequencies );

}

void AIAllocator::Finish() {

  switch (Mode()) {
    
  case 4:
    // destroy the Agent
    pKernel->Shutdown() ;
    delete pKernel;    
    break;
  }    

}
