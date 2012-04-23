//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "allocator.h"
#include "gmccdma.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_sort_vector.h"

#define FIXED_ALLOCATION
//#define GIVE_BEST_CARR
//#define SWAP_BAD_GOOD
//#define BEST_OVERLAP

//#define SHOW_POWER
//#define SHOW_MATRIX

//
//
// Matrix display
//
//
extern void gsl_matrix_uint_show(gsl_matrix_uint *mat);
extern void gsl_matrix_complex_show(gsl_matrix_complex *mat);


void Allocator::Setup() {



//////// initialization of dynamic data structures
  Hchan = gsl_vector_complex_alloc(N());
  Hmat = gsl_matrix_complex_alloc(N(),USERS);

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

  
//////// rate declaration for ports


}


void Allocator::Run() {

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


#ifdef FIXED_ALLOCATION

  gsl_matrix_uint_memcpy(signature_frequencies,
			 signature_frequencies_init);

#endif // FIXED_ALLOCATION


#ifdef GIVE_BEST_CARR


  //
  // SORT CARRIERS OF EACH USERS
  //
  gsl_matrix_uint *Hperm = gsl_matrix_uint_alloc(N(),USERS);
  gsl_permutation *p = gsl_permutation_alloc(N());
  gsl_vector *huserabs = gsl_vector_alloc(N());

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
  unsigned int curruser = gsl_rng_uniform_int(ran,USERS);
  
 
  //
  // ASSIGN FREQUENCIES
  //
  gsl_vector_uint *nextcarr = gsl_vector_uint_alloc(USERS);
  gsl_vector_uint *usedcarr = gsl_vector_uint_alloc(N());
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
  //gsl_matrix_complex_show(Hmat);
  //gsl_matrix_uint_show(Hperm);
  //gsl_matrix_uint_show(signature_frequencies);

  //
  // FREE ALLOCATIONS
  //
  gsl_vector_free(huserabs);
  gsl_permutation_free(p);
  gsl_matrix_uint_free(Hperm);
  gsl_vector_uint_free(nextcarr);
  gsl_vector_uint_free(usedcarr);

#endif // GIVE_BEST_CARR
  


#ifdef SWAP_BAD_GOOD


 
  //
  // HPERM CONSTRUCTION
  //
  // SORT CARRIERS OF EACH USERS
  //
  gsl_matrix_uint *Hperm = gsl_matrix_uint_alloc(N(),USERS);
  gsl_permutation *p = gsl_permutation_alloc(N());
  gsl_matrix *habs = gsl_matrix_alloc(N(),USERS);

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

  // here
  //
  // Hperm(N,USERS) contains sorted channels index for each users
  // habs(N,USERS) contains channel energy per each user
  //
  
  //
  // FIND INITIAL USER RANDOMLY for fairness
  //
  unsigned int curruser = gsl_rng_uniform_int(ran,USERS);
  
 
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


  //
  // FREE ALLOCATIONS
  //
  gsl_matrix_free(habs);
  gsl_permutation_free(p);
  gsl_matrix_uint_free(Hperm);


#endif // SWAP_BAD_GOOD


#ifdef BEST_OVERLAP


  //
  // SORT CARRIERS OF EACH USERS
  //
  gsl_matrix_uint *Hperm = gsl_matrix_uint_alloc(N(),USERS);
  gsl_permutation *p = gsl_permutation_alloc(N());
  gsl_vector *huserabs = gsl_vector_alloc(N());

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

  //
  // FREE ALLOCATIONS
  //
  gsl_vector_free(huserabs);
  gsl_permutation_free(p);
  gsl_matrix_uint_free(Hperm);


#endif // BEST_OVERLAP
  


  //#ifdef SHOW_MATRIX
  //  gsl_matrix_complex_show(Hmat);
  //   gsl_matrix_uint_show(signature_frequencies);
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

