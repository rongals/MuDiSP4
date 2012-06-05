//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//
#include "blockrec.h"
#include "gsl/gsl_math.h"
#include "gsllapack.h"
#include "gmccdma.h"


#define MAXNUMLEN 128
#define PI 3.1415926536


extern void  gsl_matrix_complex_show( gsl_matrix_complex *);



void BlockRec::Setup() {

  //////// initialization of dynamic data structures


  Ns=(1 << Nb());


  /// vector and matrix allocation 

  gray_encoding = gsl_vector_uint_alloc(Ns);

  symbol_arg = 2.0*double(PI/Ns);
  


  count=0;
				
  coding_mat = gsl_matrix_complex_calloc(J(),K());
  selection_mat = gsl_matrix_complex_calloc(N(),J());
  transform_mat = gsl_matrix_complex_calloc(N(),N());

  tmp = gsl_vector_complex_calloc(N());
  tmp1 = gsl_vector_complex_calloc(N());
  tmp2 = gsl_vector_complex_calloc(J());
  tmpout = gsl_vector_complex_calloc(K());


  

  //
  //
  //  FFT Transform Matrix
  //  
  // 
  double fftarg=-2.0*double(M_PI/N());
  double fftamp=1.0/sqrt(double(N()));

  for (int i=0; i<N(); i++)
    for (int j=0; j<N(); j++)
      gsl_matrix_complex_set(transform_mat, i,j, gsl_complex_polar(fftamp,fftarg*i*j) );

  //////// rate declaration for ports
  
  out1.SetRate( Nb()*K() );
  
  
///////// Gray Decoder SetUp

  for (unsigned int i=0; i<Ns; i++) {
    unsigned int tmp=0;
    for (unsigned int k=0; k<Nb(); k++) {
      unsigned int t=(1<<k), tt=2*t;
      tmp += t * (((t+i)/tt) % 2);
    }
    gsl_vector_uint_set(gray_encoding,tmp,i);
  }
}


void BlockRec::Run() {


  gsl_matrix_complex cmat  =  min2.GetDataObj();
  gsl_matrix_uint signature_frequencies = min3.GetDataObj();


  //
  //
  // CARRIER SELECTION MATRIX UPDATE
  //
  // 
  gsl_matrix_complex_set_zero(selection_mat);
  for (int i=0;i<J(); i++) {
    unsigned int carrier=gsl_matrix_uint_get(&signature_frequencies,Cd(),i);
    gsl_complex one=gsl_complex_polar(1.0,0.0);
    gsl_matrix_complex_set(selection_mat,carrier,i,one);
  }
  

  //
  //
  // EQUALIZATION MATRIX 
  //
  //


  //
  // fetch time domain channel response
  //
  gsl_vector_complex_const_view hchan = gsl_matrix_complex_const_row(&cmat,Cd());
  

  //
  // find frequency response
  //
  gsl_vector_complex *Hchan = gsl_vector_complex_alloc(N());
  gsl_blas_zgemv(CblasNoTrans, 
		 gsl_complex_rect(sqrt(double(N())),0), 
		 transform_mat, 
		 &(hchan.vector),
		 gsl_complex_rect(0,0), 
		 Hchan);

  //  cout << "\n\n channel time response = " << endl;
  // gsl_vector_complex_fprintf(stdout,&(hchan.vector),"%f");


#ifdef SHOW_MATRIX

  cout << endl << BlockName << endl;

  cout << "\n\n channel frequency response = " << endl;
  gsl_vector_complex_fprintf(stdout,Hchan,"%f");
#endif

  //
  //
  // compute pre-equalization at the receiver (D matrix)
  //
  //
#ifdef GIANNAKIS_PRECODING
  double roarg=2.0*double(M_PI/N());
  for (int i=0;i<J(); i++) {
    unsigned int carrier=gsl_matrix_uint_get(&signature_frequencies,Cd(),i);
    gsl_complex hro = gsl_vector_complex_get(Hchan,carrier);
    for (int j=0; j<K(); j++) {
      gsl_complex ro=gsl_complex_polar(sqrt(1.0/double(J())),-j*carrier*roarg);
      gsl_matrix_complex_set(coding_mat,i,j,gsl_complex_mul(ro,hro));
    }
  }
#else
  double roarg=2.0*double(M_PI/J());
  for (int i=0;i<J(); i++) {
    unsigned int carrier=gsl_matrix_uint_get(&signature_frequencies,Cd(),i);
    gsl_complex hro = gsl_vector_complex_get(Hchan,carrier);
    for (int j=0; j<K(); j++) {
      gsl_complex ro=gsl_complex_polar(sqrt(1.0/double(J())),-j*i*roarg);
      gsl_matrix_complex_set(coding_mat,i,j,gsl_complex_mul(ro,hro));
    }
  }
#endif

  //
  //
  //
  gsl_vector_complex_free(Hchan);

  #ifdef SHOW_MATRIX
  cout << "\n\nEquivalent Channel+Coding mat:" << endl;
  gsl_matrix_complex_show(coding_mat);
  #endif 

  // fetch a received block
  //gsl_vector_complex_memcpy(tmp,&vin1.GetDataObj());
  *tmp=vin1.GetDataObj();

#ifdef SHOW_MATRIX
   cout << "\n\n received  = " << endl;
   gsl_vector_complex_fprintf(stdout,tmp,"%f");
#endif 

 
  // compute FFT on received block ( y = A x)
   gsl_blas_zgemv(CblasNoTrans, 
		 gsl_complex_rect(1.0,0), 
		 transform_mat, 
		 tmp,
		 gsl_complex_rect(0,0), 
		 tmp1);


#ifdef SHOW_MATRIX
   cout << "\n\n transformed received = " << endl;
   gsl_vector_complex_fprintf(stdout,tmp1,"%f");
#endif 
   
 
  
   // compute carrier selection ( y = A x )
   gsl_blas_zgemv(CblasTrans, 
		 gsl_complex_rect(1.0,0), 
		 selection_mat, 
		 tmp1,
		 gsl_complex_rect(0,0), 
		 tmp2);

#ifdef SHOW_MATRIX
   cout << "\n\n transformed selected received = " << endl;
   gsl_vector_complex_fprintf(stdout,tmp2,"%f");
#endif 

   //  USE FOR TEST ONLY !!!
   //  gsl_matrix_complex_set_identity(coding_mat);
   //    gsl_blas_zgemv(CblasNoTrans, 
   // 		  gsl_complex_rect(1.0,0), 
   // 		  coding_mat, 
   // 		  tmp2,
   // 		  gsl_complex_rect(0,0), 
   // 		  tmpout);



   //
   // SVD solution of min||Ax-b|| 
   //
   int svdres = gsl_lapack_solveSVD_complex(tmp2, 
			       coding_mat,
			       tmpout);

   #ifdef SHOW_MATRIX
   cout << "\n\nrank of A matrix  = " << svdres << endl;
   cout << "\n\ndetected symbols =" << endl;
   gsl_vector_complex_fprintf(stdout,tmpout,"%f");

   
   gsl_blas_zgemv( CblasNoTrans, 
		   gsl_complex_rect(1,0),
		   coding_mat, 
		   tmpout, // gsl_vector_complex * x,
		   gsl_complex_rect(-1.0,0), // const gsl_complex beta, 
		   tmp2 ); // gsl_vector_complex * y)
   
   double residual = gsl_blas_dznrm2(tmp2);
   
   cout << "\n\nsvd residual error = " << residual << endl;
   gsl_vector_complex_fprintf(stdout,tmp2,"%f");
   #endif 
   
   
   
   // symbol rate loop
   for (int j=0;j<K();j++) {

     //
     // Decision process
     //
     
     //unsigned int Ns=(1 << Nb());
     symbol_id=Ns;
     unsigned int outbit;
     double mindist=GSL_POSINF;
     
     for (unsigned int sid=0; sid<Ns; sid++){
       if (distance(sid,gsl_vector_complex_get(tmpout,j))<mindist) {
	 symbol_id=sid;
	 mindist=distance(sid, gsl_vector_complex_get(tmpout,j) );
       }
     }
     
//      cout << "symb id = "
// 	  << symbol_id
// 	  << "  gray = "
// 	  << gsl_vector_uint_get(gray_encoding, symbol_id)
// 	  << endl;


     for (int i=0; i<Nb(); i++) {
       outbit=( gsl_vector_uint_get(gray_encoding, symbol_id) >> (Nb()-i-1) ) & 0x1;
       out1.DeliverDataObj( outbit );

     }
   }


}

inline double BlockRec::distance(unsigned int symbol_id, gsl_complex ycorr) {
  gsl_complex refsym = gsl_complex_polar(1.0,symbol_arg * symbol_id );
  return gsl_complex_abs( gsl_complex_sub(refsym,ycorr) );
}

void BlockRec::Finish() {


  gsl_vector_uint_free(gray_encoding);

  gsl_matrix_complex_free(coding_mat);
  gsl_matrix_complex_free(selection_mat);
  gsl_matrix_complex_free(transform_mat);

  gsl_vector_complex_free(tmp);
  gsl_vector_complex_free(tmp1);
  gsl_vector_complex_free(tmp2);
  gsl_vector_complex_free(tmpout);


//////// post processing

}

