//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "maiallocator.h"
#include "gmccdma.h"

#define INIT_CARR_POWER 1.0
#define POWER_STEP 0.01
#define MAX_ERROR_RATE 0.01
#define MAX_POWER 10.0 

// FIXED_ALLOCATION
// each user is assigned a fixed set of carriers chosen with the modulus 
// function
// sig(mu,j)=(j M + mu)
//
// GIVE_BEST_CARR
// first we sort the channel responses of each user, then a first user is 
// selected randomly and the best J carriers 
// are assigned in roud-robin fashion (give N > JM)
// 
//
// SWAP_BAD_GOOD
// starting from a fixed allocation, we swap the worst carrier of current 
// users with the best carrier used by others.
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
//#define SPAWN_DEBUGGER
//#define PAUSED

//
//
// Matrix display
//
//
//extern void gsl_matrix_uint_show(gsl_matrix_uint *mat);
//extern void gsl_matrix_complex_show(gsl_matrix_complex *mat);

void MAIAllocator::AssignFree(std::string & sUid, 
			      std::string & sDeassign,
			      std::string & sAssign) {

  unsigned int foundj=J();
  unsigned int u = atoi(sUid.c_str());
  unsigned int newcarr = atoi(sAssign.c_str());
  unsigned int lastcarr = atoi(sDeassign.c_str());
  // we find the index of current allocation
  for (int j=0; j<J(); j++)
    if (gsl_matrix_uint_get(signature_frequencies,u,j) == lastcarr) {
      foundj = j;
      break;
    }
  // we perform the allocation and reset power
  gsl_matrix_uint_set(signature_frequencies,u,foundj,newcarr);
  gsl_matrix_set(signature_powers,u,foundj,INIT_CARR_POWER);

}


void MAIAllocator::SwapCarriers(std::string & sU1, 
				std::string & sC1,
				std::string & sU2,
				std::string & sC2) {

  unsigned int foundj1=J();
  unsigned int foundj2=foundj1;
  unsigned int u1 = atoi(sU1.c_str());
  unsigned int u2 = atoi(sU2.c_str());
  unsigned int c1 = atoi(sC1.c_str());
  unsigned int c2 = atoi(sC2.c_str());

  // we find the indexes of current allocations
  for (int j=0; j<J(); j++) {
    if (gsl_matrix_uint_get(signature_frequencies,u1,j) == c1)
      foundj1 = j;
    if (gsl_matrix_uint_get(signature_frequencies,u2,j) == c2)
      foundj2 = j;
  }
  // we perform the swap allocation and reset power
  gsl_matrix_uint_set(signature_frequencies,u1,foundj1,c2);
  gsl_matrix_uint_set(signature_frequencies,u2,foundj2,c1);
  gsl_matrix_set(signature_powers,u1,foundj1,INIT_CARR_POWER); 
  gsl_matrix_set(signature_powers,u1,foundj2,INIT_CARR_POWER); 
 
}


void MAIAllocator::IncreasePower(std::string & sUid, 
				 std::string & sCid) {

  unsigned int foundj=J();
  unsigned int u = atoi(sUid.c_str());
  unsigned int c = atoi(sCid.c_str());
  // we find the index of current allocation
  for (int j=0; j<J(); j++)
    if (gsl_matrix_uint_get(signature_frequencies,u,j) == c) {
      foundj = j;
      break;
    }
  // we perform the allocation and reset power
  gsl_matrix_set(signature_powers,u,foundj,
		 gsl_matrix_get(signature_powers,u,foundj)+POWER_STEP);

}






void MAIAllocator::Setup() {

//////// initialization of dynamic data structures

  Hmat = gsl_matrix_complex_alloc(N(),M());
  Hchan = gsl_vector_complex_alloc(N());
  Hperm = gsl_matrix_uint_alloc(N(),M());
  p = gsl_permutation_alloc(N());
  huserabs = gsl_vector_alloc(N());
  nextcarr = gsl_vector_uint_alloc(M());
  usedcarr = gsl_vector_uint_alloc(N());
  errs = gsl_vector_uint_alloc(M());
  habs = gsl_matrix_alloc(N(),M());
  huu = gsl_matrix_complex_alloc(N(),M());

  framecount = 0;
  noDecisions = 0;
  ostringstream cmd;

  //
  // Random Generator
  //
  ran = gsl_rng_alloc( gsl_rng_default );

  // SIGNATURE FREQUENCIES INITIAL SETUP

  signature_frequencies = gsl_matrix_uint_alloc(M(),J());
  signature_frequencies_init = gsl_matrix_uint_alloc(M(),J());
  signature_powers = gsl_matrix_alloc(M(),J());


  for (int i=0; i<M(); i++)
    for (int j=0; j<J(); j++)
      gsl_matrix_uint_set(signature_frequencies_init,i,j,(j*M()+i) % N());

  //
  // INITIAL ALLOCATION
  //
  gsl_matrix_uint_memcpy(signature_frequencies,
			 signature_frequencies_init);
  // maximum initial powers for all carriers
  gsl_matrix_set_all(signature_powers,INIT_CARR_POWER); 

  gsl_vector_uint_set_zero(errs);


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
      gsl_matrix_complex_set(transform_mat,i,j,
			     gsl_complex_polar(fftamp,fftarg*i*j) );



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
    // SOAR INITIALIZATION
    //
    max_errors = MAX_ERROR_RATE * ERROR_REPORT_INTERVAL * Nb() * K();
    cout << BlockName << " - Max errors tuned to " << max_errors << " errors/frame." << endl;

    //
    // first we initialize the vectors and matrices
    // in the order of appearance in the header file.
    //
    umapUserVec =  vector < Identifier * > (M());
    umapUserUidVec = vector < IntElement * > (M());
    umapUserErrsVec = vector < IntElement * > (M());
    umapUserPowerVec = vector < FloatElement * > (M());
    umapUserCarrMat  = vector < Identifier * > (M()*J());
    umapUserCarrCidMat = vector < IntElement * > (M()*J());
    umapUserCarrPowerMat = vector < FloatElement * > (M()*J());
    
    chansCoeffMat = vector < Identifier * > (M()*N());
    chansCoeffUserMat = vector < IntElement * > (M()*N());
    chansCoeffCarrMat = vector < IntElement * > (M()*N());
    chansCoeffValueMat = vector < FloatElement * > (M()*N());
    
    carmapCarrVec = vector < Identifier * >  (N());
    carmapCarrCidVec = vector < IntElement * >   (N());
    

    //
    // then we create an instance of the Soar kernel in our process
    //

    pKernel = Kernel::CreateKernelInNewThread() ;
    //pKernel = Kernel::CreateRemoteConnection() ;
    
    // Check that nothing went wrong.  We will always get back a kernel object
    // even if something went wrong and we have to abort.
    if (pKernel->HadError())
      {
	cerr << BlockName << ".SOAR - " 
	     << pKernel->GetLastErrorDescription() << endl ;
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
    pAgent->LoadProductions(SoarFn());

    // spawn debugger
#ifdef SPAWN_DEBUGGER
    pAgent->SpawnDebugger();
#endif
    
    // Check that nothing went wrong
    // NOTE: No agent gets created if there's a problem, so we have to check for
    // errors through the kernel object.
    if (pKernel->HadError())
      {
	cerr << BlockName << ".SOAR - " 
	     << pKernel->GetLastErrorDescription() << endl ;
	exit(1);
      }

    // keypress 
    //cout << "pause maillocator:203 ... (press ENTER key)" << endl;
    //cin.ignore();

    //
    // we can now generate initial input link structure
    //

    // NO MORE adjust max-nil-output-cycle
    //cmd << "max-nil-output-cycles " << 120;
    //pAgent->ExecuteCommandLine(cmd.str().c_str());

    // the input-link
    pInputLink = pAgent->GetInputLink();

    // input-time
    input_time = 0;
    inputTime = pAgent->CreateIntWME(pInputLink,"input-time",input_time);

    // the usrmap structure (common wmes)
    umap = pAgent->CreateIdWME(pInputLink,"usrmap");

    // BITS_PER_REPORT = ERROR_REPORT_INTERVAL * Nb() * K()
    // MAX_ERRORS = MAX_ERROR_RATE * BITS_PER_REPORT
    umapMaxerr = pAgent->CreateIntWME(umap,"maxerr",max_errors);
    umapPstep = pAgent->CreateFloatWME(umap,"pstep",POWER_STEP);
    umapPmax = pAgent->CreateFloatWME(umap,"pmax",MAX_POWER);
    // the channels
    chans = pAgent->CreateIdWME(pInputLink,"channels");
    // the carmap
    carmap = pAgent->CreateIdWME(pInputLink,"carmap");
 
    // the usrmap structure (users substructure)
    for (int i=0;i<M();i++) { // user loop
      umapUserVec[i] = pAgent->CreateIdWME(umap,"user");
      umapUserUidVec[i] = pAgent->CreateIntWME(umapUserVec[i],"uid",i);
      umapUserErrsVec[i] = pAgent->CreateIntWME(umapUserVec[i],"errs",int(0));
      umapUserPowerVec[i] = pAgent->CreateFloatWME(umapUserVec[i],"power",J());
      // update the current allocation 
      for (int j=0;j<J();j++) { // allocated carriers loop
	unsigned int usedcarr = gsl_matrix_uint_get(signature_frequencies,i,j);
	double usedpow = gsl_matrix_get(signature_powers,i,j);
	umapUserCarrMat[i*J()+j] = pAgent->CreateIdWME(umapUserVec[i],"carr");
	umapUserCarrCidMat[i*J()+j] = 
	  pAgent->CreateIntWME(umapUserCarrMat[i*J()+j],"cid",usedcarr);
	umapUserCarrPowerMat[i*J()+j] = 
	  pAgent->CreateFloatWME(umapUserCarrMat[i*J()+j],"power",usedpow);
      } // allocated carriers loop
      // the channels
      for (int j=0;j<N();j++) { // all channels loop
	chansCoeffMat[i*N()+j] = pAgent->CreateIdWME(chans,"coeff");
	chansCoeffUserMat[i*N()+j] = pAgent->CreateIntWME(chansCoeffMat[i*N()+j],"user",i);
	chansCoeffCarrMat[i*N()+j] = pAgent->CreateIntWME(chansCoeffMat[i*N()+j],"carr",j);
	chansCoeffValueMat[i*N()+j] = pAgent->CreateFloatWME(chansCoeffMat[i*N()+j],"value",0.0);	
      } // all channels loop
    } // user loop

    // the carmap structure
    for (int j=0;j<N();j++) { // all carriers loop
	carmapCarrVec[j] = pAgent->CreateIdWME(carmap,"carr");
	carmapCarrCidVec[j] = pAgent->CreateIntWME(carmapCarrVec[j],"cid",j);
      } // all carriers loop
     
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


void MAIAllocator::Run() {

  // fetch channel matrix
  gsl_matrix_complex hmm  =  min1.GetDataObj();

  // update error reports at receiver rx_m
  if (framecount % ERROR_REPORT_INTERVAL == 0) { // once every ERI
    gsl_vector_uint temperr  =  vin2.GetDataObj();
    if (temperr.size == M()) {
      gsl_vector_uint_memcpy(errs,&temperr);

      for (int i=0;i<M();i++) {
      	cout << "u[" << i << "]= " 
                  << gsl_vector_uint_get(errs,i) << endl; 
      }

    }
  } 

  // extract huu (time domain response of channels tx_u --> rx_u)
  for (int u=0;u<M();u++) { // user loop

    // extract time domain response from hmm corresponding to txn-->rxn channel
    gsl_vector_complex_const_view hii = gsl_matrix_complex_const_row(&hmm,u*M()+u);

    // copy the N-sized vector hii into u-th column of huu
    gsl_matrix_complex_set_col(huu,u,&hii.vector);

  } // user loop


  //  huu matrix structure
  //
  //   +-                 -+
  //   | h(0) . . . . h(0) |
  //   |  11           uu  |
  //   |                   |
  //   | h(n) . . . . h(n) |
  //   |  11           uu  |
  //   +-                 -+
  // 
  //   where h(n) represents the channel impulse response
  //          ii
  //
  //   at time n, from tx_u to rx_u
  //   the matrix has N rows and M comumns.
  //
  //
  //
  // Hmat(NxM) = Fourier( huu(NxM) )
  // 

  gsl_blas_zgemm(CblasNoTrans,
		 CblasNoTrans,
		 gsl_complex_rect(1,0),
		 transform_mat,
		 huu,
		 gsl_complex_rect(0,0),
		 Hmat);

//		 gsl_complex_rect(sqrt(double(N())),0),
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
    // uses Hmat: the frequency responses of channels tx_m --> rx_1
    //

  for(int u=0; u<M(); u++) {

    gsl_vector_complex_const_view huser 
      = gsl_matrix_complex_const_column(Hmat,u);

    gsl_vector_uint_view sortindu = gsl_matrix_uint_column(Hperm,u);

    for (int j=0; j<N(); j++) {
      double currpower 
	= gsl_complex_abs2(gsl_vector_complex_get(&huser.vector,j));

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
  curruser = gsl_rng_uniform_int(ran,M());
  
 
  //
  // ASSIGN FREQUENCIES
  //
  gsl_vector_uint_set_all(nextcarr,0);
  gsl_vector_uint_set_all(usedcarr,0);
  for (int j=0; j<J(); j++) {
    for (int uu=0; uu<M(); uu++) {
      int u = (uu+curruser) % M();
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
  //   for (int u=0; u<M(); u++) {
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


  for(int u=0; u<M(); u++) {

    gsl_vector_complex_const_view huser 
      = gsl_matrix_complex_const_column(Hmat,u);
    gsl_vector_uint_view sortindu = gsl_matrix_uint_column(Hperm,u);
    gsl_vector_view huserabs = gsl_matrix_column(habs,u);

    for (int j=0; j<N(); j++) {
      double currpower 
	= gsl_complex_abs2(gsl_vector_complex_get(&huser.vector,j));
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
  curruser = gsl_rng_uniform_int(ran,M());
  
 
  //
  // ASSIGN FREQUENCIES
  //

  //
  // for each user ...
  //
  for (int uu=0; uu<M(); uu++) {
    int u = (uu+curruser) % M();

 
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
    for (int uuu=0; uuu<M()-1; uuu++) {
      unsigned int otheru = (uuu+u) % M();
      for (int j=0; j<J(); j++) {
	unsigned int chind 
	  = gsl_matrix_uint_get(signature_frequencies,otheru,j);
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
    unsigned int chind 
      = gsl_matrix_uint_get(signature_frequencies,u,worstjindex);
    gsl_matrix_uint_set(signature_frequencies,u,worstjindex,
			gsl_matrix_uint_get(signature_frequencies,
					    bestuser,bestjindex));
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

  for(int u=0; u<M(); u++) {

    gsl_vector_complex_const_view huser 
      = gsl_matrix_complex_const_column(Hmat,u);
    gsl_vector_uint_view sortindu = gsl_matrix_uint_column(Hperm,u);

    for (int j=0; j<N(); j++) {
      double currpower = gsl_complex_abs2(gsl_vector_complex_get(&huser.vector,
								 j));
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
  for (int u=0; u<M(); u++) {
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
    // SOAR
    //
    // we base the decisions on the channels tx_m --> rx_m extracted from hmm

    //-----------------------------------------------------------------------

    // extract frequency response for all users and all carriers
    // hmm (M**2xN)
    //
    // for each user 
    //   we get the hmm time response, 
    //   then Hmm frequency response,
    //   update the relevant sections of ^io.input-link
    //

    //    gsl_matrix_complex_show(Hmat);

    // keypress 
	//  cout << "pause maillocator:620 ... (press ENTER key)" << endl;
    // cin.ignore();




	  for (int u=0;u<M();u++) { // user loop

		  // update soarUserMap.wmeErrsVec[u]
		  pAgent->Update(umapUserErrsVec[u],gsl_vector_uint_get(errs,u));

		  // update soarChannelMap.wmeValueMat[i*M()+j]
		  for (int j=0;j<N();j++) {
			  double coeffVal = gsl_complex_abs2(gsl_matrix_complex_get(Hmat,j,u));
			  pAgent->Update(chansCoeffValueMat[u*N()+j],coeffVal);
		  } // j loop

		  // update
		  for (int j=0;j<J();j++) {
			  unsigned int carr = gsl_matrix_uint_get(signature_frequencies,u,j);
			  double power = gsl_matrix_get(signature_powers,u,j);
			  pAgent->Update(umapUserCarrCidMat[u*J()+j],carr);
			  pAgent->Update(umapUserCarrPowerMat[u*J()+j],power);
		  } // j loop
	  } // user loop


	  // Every GEO_UPDATE_INTERVAL we increase the input-time and allow decisions
	  if (framecount % GEO_UPDATE_INTERVAL == 0) {
		  noDecisions = 0;
		  pAgent->Update(inputTime,++input_time);
	  }
	  pAgent->Commit();

    // commit changes no longer needed
    //pAgent->Commit();

    // run agent till output
    numberCommands=0; 

    //while (! (numberCommands || noDecisions) ) { // while numberCommands=0 and noDecisions=0
    while (! (noDecisions) ) { // while numberCommands=0 and noDecisions=0

      //pAgent->RunSelf(1);
      pAgent->RunSelfTilOutput();
      
      numberCommands = pAgent->GetNumberCommands() ;
      

#ifdef PAUSED
      // keypress 
      cout << "pause maillocator:663 ... (press ENTER key)" << endl;
      cin.ignore();
#endif

    //} WHILE REMOVED HERE
    
    // cout << "Found " << numberCommands << " command/s." << endl;

    // keypress 
    // cout << "pause maillocator:671 ... (press ENTER key)" << endl;
    // cin.ignore();



    //-----------------------------------------------------------------------

    // collect results and generate signature_frequencies
    // and signature_powers

 // ^io
 //   ^output-link
 //     ^assign-free
 //       ^uid
 //       ^deassign
 //       ^assign
 //     ^swap-carriers
 //       ^u1
 //       ^c1
 //       ^u2
 //       ^c2
 //     ^increase-power
 //       ^uid
 //       ^cid
 //     ^<command>
 //       ^status


      for (int cmd = 0 ; cmd < numberCommands ; cmd++) {

    	  Identifier* pCommand = pAgent->GetCommand(cmd) ;
    	  string name  = pCommand->GetCommandName() ;

    	  //      cout << "the command is " << name << endl;


    	  if (name == "assign-free") {
    		  std::string sUid = pCommand->GetParameterValue("uid");
    		  std::string sDeassign = pCommand->GetParameterValue("deassign");
    		  std::string sAssign = pCommand->GetParameterValue("assign");
    		  cout << "assign-free command received [ u:"
    				  << sUid << " , -"
    				  << sDeassign << " , +"
    				  << sAssign << " ]"
    				  << endl;

    		  AssignFree(sUid,sDeassign,sAssign);
    		  pCommand->AddStatusComplete();

    	  } else if (name == "swap-carriers") {

    		  std::string sU1 = pCommand->GetParameterValue("u1");
    		  std::string sC1 = pCommand->GetParameterValue("c1");
    		  std::string sU2 = pCommand->GetParameterValue("u2");
    		  std::string sC2 = pCommand->GetParameterValue("c2");
    		  cout << "swap-carriers command received [ u1:"
    				  << sU1 << " , c1:"
    				  << sC1 << " , u2:"
    				  << sU2 << " , c2:"
    				  << sC2 << " ]" << endl;

    		  SwapCarriers(sU1,sC1,sU2,sC2);
    		  pCommand->AddStatusComplete();

    	  } else if (name == "increase-power") {

    		  std::string sUid = pCommand->GetParameterValue("uid");
    		  std::string sCid = pCommand->GetParameterValue("cid");

    		  cout << "increase-power command received [ u:"
    				  << sUid << " , c:"
    				  << sCid << " ]" << endl;

    		  IncreasePower(sUid,sCid);
    		  pCommand->AddStatusComplete();

    	  } else if (name == "no-choices") {


    		  cout << "no-choices command received" << endl;
    		  noDecisions = 1;
    		  pCommand->AddStatusComplete();

    		  break;


    	  } else {
    		  cout << "ignoring unknown output command from SOAR" << endl;
    	  }

    	  cout << "framecount = " << framecount << endl;

      } // end for (int cmd = 0 ; cmd < numberCommands ; cmd++)

    } // while (! (noDecisions) )

      break;

  } // switch (Mode())



  //////// production of data
  framecount++;
  mout1.DeliverDataObj( *signature_frequencies );
  mout2.DeliverDataObj( *signature_powers );

}

void MAIAllocator::Finish() {

  gsl_vector_complex_free(Hchan);
  gsl_matrix_complex_free(Hmat);
  gsl_matrix_complex_free(huu);
  gsl_matrix_uint_free(Hperm);
  gsl_permutation_free(p);
  gsl_vector_free(huserabs);
  gsl_vector_uint_free(nextcarr);
  gsl_vector_uint_free( usedcarr);
  gsl_vector_uint_free( errs );
  gsl_matrix_free(habs);

  gsl_matrix_uint_free(signature_frequencies);
  gsl_matrix_uint_free(signature_frequencies_init);

  gsl_matrix_free(signature_powers);

  gsl_rng_free(ran);

  gsl_matrix_complex_free(transform_mat);


  switch (Mode()) {
    
  case 4:
    // destroy the Kernel
    pKernel->Shutdown() ;
    delete pKernel;    
    break;
  }    

}

//  ____   ___  _   _  ___     ___  _   _ ___ _
// / ___| / _ \| \ | |/ _ \   / _ \| | | |_ _| |
// \___ \| | | |  \| | | | | | | | | | | || || |
//  ___) | |_| | |\  | |_| | | |_| | |_| || ||_|
// |____/ \___/|_| \_|\___/   \__\_\\___/|___(_)
//
