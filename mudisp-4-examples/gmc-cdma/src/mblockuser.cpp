//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#include "mblockuser.h"
#include "stdio.h"
#include "gmccdma.h"

#define MAXNUMLEN 128
#define PI 3.1415926536

//#define SHOW_MATRIX

//
//
// Carrier Allocation Matrix
//
//
//gsl_matrix_uint * signature_frequencies; 

//
//
// Matrix complex display
//
//
void gsl_matrix_complex_show(gsl_matrix_complex *mat) {
  unsigned int N=mat->size1, M=mat->size2;
  for (int i=0; i<N; i++) {
    for (int j=0; j<M; j++) {
      gsl_complex el = gsl_matrix_complex_get(mat,i,j);
      cout << "(" 
	   << GSL_REAL(el) 
	   << ","
	   << GSL_IMAG(el) 
	   << ") ";
    }
    cout << endl;
  }
  cout << endl;
}


//
//
// Matrix uint display
//
//
void gsl_matrix_uint_show(gsl_matrix_uint *mat) {
  unsigned int N=mat->size1, M=mat->size2;
  for (int i=0; i<N; i++) {
    for (int j=0; j<M; j++) {
      unsigned int el = gsl_matrix_uint_get(mat,i,j);
      cout << setw(3) 
	   << el;
    }
    cout << endl;
  }
  cout << endl;
}


//
//
// BlockUser
//
//
void MBlockUser::Setup() {

  //////// initialization of dynamic data structures
  

  // number of symbols
  Ns=(1 << Nb());


  /// vector and matrix allocation 

  gray_encoding = gsl_vector_uint_alloc(Ns);

  symbol_arg = 2.0*double(PI/Ns);

  count=0;
				
  coding_mat = gsl_matrix_complex_calloc(J(),K());
  selection_mat = gsl_matrix_complex_calloc(N(),J());
  transform_mat = gsl_matrix_complex_calloc(N(),N());
  outmat = gsl_matrix_complex_calloc(N(),M()); // outmat(i,j) = symbol at time i from user j

  tmp = gsl_vector_complex_calloc(K());
  tmp1 = gsl_vector_complex_calloc(J());
  tmp2 = gsl_vector_complex_calloc(N());
  //  tmpout = gsl_vector_complex_calloc(N()); // tmpout is a view from outmat


  //
  //
  // IFFT MATRIX UNITARY
  //
  //
  double ifftarg=2.0*double(M_PI/N());
  double ifftamp=1.0/sqrt(double(N()));
  for (int i=0; i<N(); i++)
    for (int j=0; j<N(); j++)
      gsl_matrix_complex_set(transform_mat, 
			     i,
			     j, 
			     gsl_complex_polar(ifftamp,ifftarg*i*j) );


  //////// rate declaration for ports
  
  //  in1.SetRate( Nb()*K()*M() ); // M users K symbols Nb bits 
  
  
  ///////// Gray Encoder SetUp
    
    for (unsigned int i=0; i<Ns; i++) {
      gsl_vector_uint_set(gray_encoding,i,0);
      for (unsigned int k=0; k<Nb(); k++) {
	unsigned int t=(1<<k);
	unsigned int tt=2*t;
	unsigned int ttt= gsl_vector_uint_get(gray_encoding,i) 
	  + t * (((t+i)/tt) % 2);
	gsl_vector_uint_set(gray_encoding,i,ttt);
      }
    }
}


void MBlockUser::Run() {

  //
  // Allocation Matrices
  //
  gsl_matrix_uint signature_frequencies=min2.GetDataObj();
  gsl_matrix signature_powers=min3.GetDataObj();

  //
  // input bits
  //
  gsl_matrix_uint inputbits = min1.GetDataObj();

  //
  // outer loop: the users 
  //
  for (int u=0;u<M();u++) {

    gsl_vector_complex_view tmpout = gsl_matrix_complex_column(outmat,u);


    //
    //
    // FETCH K INPUT SYMBOLS
    //
    //
    for (int j=0;j<K();j++) {
      
      symbol_id=0;
      
      //////// I take Nb bits from input and map it in new_symbol
      
      for (int i=0;i<Nb();i++) {
	symbol_id = (symbol_id << 1);
	//	symbol_id += in1.GetDataObj();
	symbol_id += gsl_matrix_uint_get(&inputbits,u,j*Nb()+i);
      }
      
      new_symbol = gsl_complex_polar(1.0,
				     symbol_arg * 
				     double(gsl_vector_uint_get(gray_encoding, 
								symbol_id)));
      gsl_vector_complex_set(tmp,j,new_symbol);
    }
    
    
    
    //
    //
    // SELECTION MATRIX UPDATE and POWER
    //
    //
    //  gsl_matrix_complex_set_identity(selection_mat);
    gsl_matrix_complex_set_zero(selection_mat);
    for (int i=0;i<J(); i++) {
      unsigned int carrier=gsl_matrix_uint_get(&signature_frequencies,u,i);
      double power=gsl_matrix_get(&signature_powers,u,i);
      gsl_complex one=gsl_complex_polar(power,0.0);
      gsl_matrix_complex_set(selection_mat,carrier,i,one);
    }
    

    //
    //
    // PRECODING MATRIX UPDATE
    //
    //
#ifdef GIANNAKIS_PRECODING
    double roarg=2.0*double(M_PI/N());
    for (int i=0;i<J(); i++) {
      unsigned int carrier=gsl_matrix_uint_get(&signature_frequencies,u,i);
      for (int j=0; j<K(); j++) {
	gsl_complex ro=gsl_complex_polar(sqrt(1.0/double(J())),-j*carrier*roarg);
	gsl_matrix_complex_set(coding_mat,i,j,ro);
      }
    }
#else
    double roarg=2.0*double(M_PI/J());
    for (int i=0;i<J(); i++) {
      for (int j=0; j<K(); j++) {
	gsl_complex ro=gsl_complex_polar(sqrt(1.0/double(J())),-j*i*roarg);
	gsl_matrix_complex_set(coding_mat,i,j,ro);
      }
    }
#endif

#ifdef SHOW_MATRIX

    cout << endl << BlockName << " user: " << u << endl;

    //
    // compute the trace of coding_mat
    //
    //
    gsl_matrix_complex *THT = gsl_matrix_complex_calloc(K(),K());
    gsl_blas_zgemm(CblasConjTrans,
		   CblasNoTrans,
		   gsl_complex_rect(1,0), 
		   coding_mat,
		   coding_mat,
		   gsl_complex_rect(0,0),
		   THT);
    gsl_complex trace=gsl_complex_rect(0,0);
    for (int j=0; j<K(); j++) {
      gsl_complex ctmp= gsl_matrix_complex_get(THT,j,j);
      trace=gsl_complex_add(trace,ctmp);
    }

    cout << endl << "coding matrix (theta) = " << endl;
    gsl_matrix_complex_show(coding_mat);

    cout << "T^h*T matrix = " << endl;
    gsl_matrix_complex_show(THT);

    cout << "T^h*T trace = " 
	 << GSL_REAL(trace) 
	 << ", "
	 << GSL_IMAG(trace) 
	 << endl;

    gsl_matrix_complex_free(THT);
#endif


    //
    //
    // PRECODING
    //
    //
    gsl_blas_zgemv(CblasNoTrans, 
		   gsl_complex_rect(1.0,0), 
		   coding_mat, 
		   tmp,
		   gsl_complex_rect(0,0), 
		   tmp1);

    //
    //
    // CARRIER SELECTION
    //
    //
    gsl_blas_zgemv(CblasNoTrans, 
		   gsl_complex_rect(1.0,0), 
		   selection_mat, 
		   tmp1,
		   gsl_complex_rect(0,0), 
		   tmp2);


    //
    //
    // IFFT TRANSFORM
    //
    //
    gsl_blas_zgemv(CblasNoTrans, 
		   gsl_complex_rect(1.0,0), 
		   transform_mat, 
		   tmp2,
		   gsl_complex_rect(0,0), 
		   &tmpout.vector);


    //   cout << "\n\n symbols (user " << u << ") = " << endl;
    //   gsl_vector_complex_fprintf(stdout,tmp,"%f");

#ifdef SHOW_MATRIX
    cout << "\n\n symbols (user " << u << ") = " << endl;
    gsl_vector_complex_fprintf(stdout,tmp,"%f");

    cout << "\n\n precoded = " << endl;
    gsl_vector_complex_fprintf(stdout,tmp1,"%f");

    cout << "\n\n precoded selected = " << endl;
    gsl_vector_complex_fprintf(stdout,tmp2,"%f");

    cout << "\n\n precoded selected transformed = " << endl;
    gsl_vector_complex_fprintf(stdout,&tmpout.vector,"%f");
#endif


  } // close user loop

    mout1.DeliverDataObj(*outmat);

}


void MBlockUser::Finish() {

//////// post processing

  gsl_vector_uint_free(gray_encoding);
				
  gsl_matrix_complex_free(coding_mat);
  gsl_matrix_complex_free(selection_mat);
  gsl_matrix_complex_free(transform_mat);
  gsl_matrix_complex_free(outmat);

  gsl_vector_complex_free(tmp);
  gsl_vector_complex_free(tmp1);
  gsl_vector_complex_free(tmp2);



}

