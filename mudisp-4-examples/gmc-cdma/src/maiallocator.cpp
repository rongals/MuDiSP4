//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "maiallocator.h"
#include "gmccdma.h"

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

//
//
// Matrix display
//
//
extern void gsl_matrix_uint_show(gsl_matrix_uint *mat);
extern void gsl_matrix_complex_show(gsl_matrix_complex *mat);


void MAIAllocator::Setup() {

//////// initialization of dynamic data structures

  Hchan = gsl_vector_complex_alloc(N());
  Hmat = gsl_matrix_complex_alloc(N(),M());
  Hperm = gsl_matrix_uint_alloc(N(),M());
  p = gsl_permutation_alloc(N());
  huserabs = gsl_vector_alloc(N());
  nextcarr = gsl_vector_uint_alloc(M());
  usedcarr = gsl_vector_uint_alloc(N());
  errs = gsl_vector_uint_alloc(M());
  habs = gsl_matrix_alloc(N(),M());

  framecount = 0;

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
  gsl_matrix_set_all(signature_powers,1.0); 

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


  //
  //
  //
  // Identifier *pInputLink;
  // Identifier *pID1, *pID2;
  // IntElement *pWMEint1, *pWMEint2, *pWMEint3, *pWMEint4;
  // StringElement *pWMEstr1;
  //  int numberCommands;


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
    // init pointers to Identifiers and WMEs
    //
    soarUserMap.userIDVec = vector < Identifier * > (M());
    soarUserMap.wmeUidVec = vector < IntElement * > (M());
    soarUserMap.wmeRequiresVec = vector < IntElement * > (M());
    soarUserMap.wmeErrsVec = vector < IntElement * > (M());

    soarChannelMap.coeffIDMat = vector < Identifier * > (M()*N());
    soarChannelMap.wmeUserMat = vector < IntElement * > (M()*N());
    soarChannelMap.wmeCarrMat = vector < IntElement * > (M()*N());
    soarChannelMap.wmeValueMat = vector < FloatElement * > (M()*N());


    //
    //
    // Create an instance of the Soar kernel in our process
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
    pAgent->SpawnDebugger();

    
    // Check that nothing went wrong
    // NOTE: No agent gets created if there's a problem, so we have to check for
    // errors through the kernel object.
    if (pKernel->HadError())
      {
	cerr << BlockName << ".SOAR - " 
	     << pKernel->GetLastErrorDescription() << endl ;
	exit(1);
      }

    //
    // generate initial input link structure
    //
    //
    // #
    // # <s> ^io.input-link
    // #
    // # usrmap
    // #   nusers 0
    // #   maxerr 0
    // #   pstep 0.0
    // #   *user
    // #     uid 0
    // #     requires 0
    // #     errs 0
    // #
    // # channels
    // #   *coeff
    // #     user 0
    // #     carr 0
    // #     value 0.0
    // #
    // # carmap
    // #   *carr
    // #     cid 0
    // #
    // # input-time 0
    // #


    // Identifier *pInputLink;
    // Identifier *pID1, *pID2;
    // IntElement *pWMEint1, *pWMEint2, *pWMEint3, *pWMEint4;
    // StringElement *pWMEstr1;


    // Input link
    pInputLink = pAgent->GetInputLink();

    // input-time
    wmeTime = pAgent->CreateIntWME(pInputLink,"input-time",0);

    // usrmap
    userMapID = pAgent->CreateIdWME(pInputLink,"usrmap");
    wmeNusers = pAgent->CreateIntWME(userMapID,"nusers",M());
    wmeMaxerr = pAgent->CreateIntWME(userMapID,"maxerr",100);
    wmePstep = pAgent->CreateFloatWME(userMapID,"pstep",0.01);
    for (int j=0;j<M();j++) { // usrmap population
      soarUserMap.userIDVec[j] = pAgent->CreateIdWME(userMapID,"user"); 
      soarUserMap.wmeUidVec[j] = pAgent->CreateIntWME(soarUserMap.userIDVec[j],"uid",j);
      soarUserMap.wmeRequiresVec[j] = pAgent->CreateIntWME(soarUserMap.userIDVec[j],"requires",J());
      soarUserMap.wmeErrsVec[j] = pAgent->CreateIntWME(soarUserMap.userIDVec[j],"errs",0);
    } // usrmap population

    // carmap
    carrMapID = pAgent->CreateIdWME(pInputLink,"carmap");
    for (int i=0;i<N();i++) { // carmap population
      Identifier *carrID = pAgent->CreateIdWME(carrMapID,"carr");
      IntElement *wmeCid = pAgent->CreateIntWME(carrID,"cid",i);
    } // carmap

    // channels
    channelsID = pAgent->CreateIdWME(pInputLink,"channels");
    for (int i=0;i<M();i++) { // i loop (users)
      for (int j=0;j<N();j++) { // j loop (time)
	Identifier *coeffID = soarChannelMap.coeffIDMat[i*N()+j] 
	  = pAgent->CreateIdWME(channelsID,"coeff");
	soarChannelMap.wmeUserMat[i*N()+j] = pAgent->CreateIntWME(coeffID,"user",i);
	soarChannelMap.wmeCarrMat[i*N()+j] = pAgent->CreateIntWME(coeffID,"carr",j);
	soarChannelMap.wmeValueMat[i*N()+j] = pAgent->CreateFloatWME(coeffID,"value",1.0);
      } // end j
    } // end i
    
     
    //
    // END OF SOAR INITIALIZAZION
    //

    // TEST TEST TEST 


    // numberCommands=0; 

    // while (! numberCommands) {
    //   pAgent->RunSelf(1);
    //   numberCommands = pAgent->GetNumberCommands() ;
    // }

    // cout << "Found " << numberCommands << " commands." << endl;
    // char c;
    // cin >> c;

    // TEST TEST TEST
    
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


  // extract channels tx_m --> rx_1 only (all carriers)
  gsl_matrix_complex_view hm1 = gsl_matrix_complex_submatrix(&hmm,0,0,M(),N());

  // update error reports at receiver rx_m
  if (++framecount % ERROR_REPORT_INTERVAL == 0) { // once every ERI
    gsl_vector_uint temperr  =  vin2.GetDataObj();
    framecount = 0;
    if (temperr.size == M()) {
      gsl_vector_uint_memcpy(errs,&temperr);

      // for (int i=0;i<M();i++) {
      // 	cout << "u[" << i << "]= " 
      //             << gsl_vector_uint_get(errs,i) << endl; 
      // }

    }
  } 
  




  //  hm1 matrix structure
  //
  //   +-                 -+
  //   | h(0) . . . . h(n) |
  //   |  11           11  |
  //   |                   |
  //   | h(0) . . . . h(n) |
  //   |  12           12  |
  //   +-                 -+
  // 
  //   where h(n) represents the channel impulse response
  //          i1
  //
  //   at time n, from tx i to rx 1
  //   the matrix has M rows and N comumns.
  //
  //
  //
  // Hmat = Fourier( cmat )
  //

  gsl_blas_zgemm(CblasNoTrans,
		 CblasTrans,
		 gsl_complex_rect(sqrt(double(N())),0),
		 transform_mat,
		 &hm1.matrix,
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

    for (int u=0;u<M();u++) { // user loop

      // extract view from hnn
      gsl_vector_complex_const_view huu = gsl_matrix_complex_const_row(&hmm,u*M()+u);
      
      // FFT(huu) --> Hchan 
      gsl_blas_zgemv(CblasNoTrans,
		     gsl_complex_rect(1,0),
		     transform_mat,
		     &huu.vector,
		     gsl_complex_rect(0,0),
		     Hchan);

      // cout << "Hchan(u=" << u << ")=" << endl; 
      // gsl_vector_complex_fprintf(stdout,Hchan,"%f");


      // update soarUserMap.wmeErrsVec[u]
      pAgent->Update(soarUserMap.wmeErrsVec[u],gsl_vector_uint_get(errs,u));

      // update soarChannelMap.wmeValueMat[i*M()+j]
      for (int j=0;j<N();j++) {
	double coeffVal = gsl_complex_abs2(gsl_vector_complex_get(Hchan,u));
	pAgent->Update(soarChannelMap.wmeValueMat[u*N()+j],coeffVal);
      } // j loop

    } // user loop


    // commit changes
    pAgent->Commit();

    // run agent till output
    numberCommands=0; 

    while (! numberCommands) {
      pAgent->RunSelf(1);
      numberCommands = pAgent->GetNumberCommands() ;
    // char c;
    // cin >> c;
    }

    cout << "Found " << numberCommands << " commands." << endl;
    char c;
    cin >> c;


  //  ____   ___  _   _  ___     ___  _   _ ___ _
  // / ___| / _ \| \ | |/ _ \   / _ \| | | |_ _| |
  // \___ \| | | |  \| | | | | | | | | | | || || |
  //  ___) | |_| | |\  | |_| | | |_| | |_| || ||_|
  // |____/ \___/|_| \_|\___/   \__\_\\___/|___(_)



    //-----------------------------------------------------------------------

    // collect results and generate signature_frequencies
    // and signature_powers

    //    ^io.output-link.allocation-map
    //      *user
    //        uid <num>
    //        needs <num>
    //        *allocation
    //          cid <num>
    //          power <num>
    //

    for (int i = 0 ; i < numberCommands ; i++) {
      Identifier* pCommand = pAgent->GetCommand(i) ;
      
      string name  = pCommand->GetCommandName() ;
      
      if (name != "allocation-map") {
	cerr << "uncoherent command received from SOAR agent" << endl;
	exit(1);
      } else { // ok proceed

	// only users as children
	int nusers = pCommand->GetNumberChildren();
	
	for (int i=0;i<nusers;i++) { // user loop
	  
	  Identifier *pUser = pCommand->GetChild(i)->ConvertToIdentifier();
	  string sUid = pUser->GetParameterValue("uid");
	  string sNeeds = pUser->GetParameterValue("needs");
	  
	  // only allocations as children
	  int nallocs = pUser->GetNumberChildren();
	  
	  cout << "^io.output-link.allocation-map user id:"
	       << sUid
	       << " needs:"
	       << sNeeds
	       << " allocations["
	       << nallocs-2 
	       << "]= ";
	  
	  // iterate among children

	  Identifier::ChildrenIter child;
	  int allIdx = 0;

	  for (child = pUser->GetChildrenBegin();
	       child != pUser->GetChildrenEnd();
	       child++) {

	    string childAttr = (*child)->GetAttribute();

	    if (childAttr == "allocation") { // so it's an identifier
	      Identifier *alloc = (*child)->ConvertToIdentifier();

	      string sCid = alloc->GetParameterValue("cid");
	      string sPower = alloc->GetParameterValue("power");
	      
	      cout << sCid << "(" << sPower << "), ";

	      gsl_matrix_uint_set(signature_frequencies,
				  i,
				  allIdx,
				  atoi(sCid.c_str()));

	      gsl_matrix_set(signature_powers,
				  i,
				  allIdx,
				  atof(sPower.c_str()));

	      allIdx++;
	      
	    } // if is allocation 
	  } // children loop
	  
	  cout << endl;

	} // user (i) loop 

	cout << endl;

      }


      
      // Then mark the command as completed
      pCommand->AddStatusComplete() ;
      
    }
    
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
  mout2.DeliverDataObj( *signature_powers );

  gsl_matrix_uint_show(signature_frequencies);

}

void MAIAllocator::Finish() {

  gsl_vector_complex_free(Hchan);
  gsl_matrix_complex_free(Hmat);
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
