//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//
#include "viterbidecoder.h"
#include "convolutional.h"

void ViterbiDecoder::Setup() {

//////// initialization of dynamic data structures



  ifstream ifile;

  unsigned int rows, columns;
  double el;

  string matfn( GPfn() );

  ifile.open( matfn.c_str() );
  if (! ifile) {
    cerr << BlockName << "error opening " << matfn.c_str() <<endl;
    exit(_ERROR_OPEN_FILE_);
  }

  ifile >> rows >> columns;

//   cout << "rows=" << rows << endl;
//   cout << "cols=" << columns << endl;


  gp_mat = gsl_matrix_calloc(rows,columns);
  for (int i = 0; i< rows; i++)
    for (int j=0; j<columns; j++) {
      ifile >> el;
      gsl_matrix_set(gp_mat,i,j,el);
    }
  ifile.close();


//////// rate declaration for ports

}


void ViterbiDecoder::Run() {

	int      DataLength, CodeLength, i, j, index;
	int     *g_encoder;
	int      nn, KK, mm, max_states, code_type, dec_type;
	double   elm;
	float   *input_c_float;
	int     *output_u_int;
	int     *out0, *out1, *state0, *state1;


	///////////////////////////////////////////////
	///////////////////////////////////////////////
	///////////////////////////////////////////////

	  /* first input is the data word */
	  gsl_vector_class inputobj = vin1.GetDataObj();
	
	  CodeLength = inputobj.vec->size; /* number of data bits */
	  
	  /* convert the input into float */			
	  input_c_float = (float *)calloc( CodeLength, sizeof(float) );
	  for (i=0;i<CodeLength;i++)
	    input_c_float[i] = gsl_vector_get(inputobj.vec,i);


	  /* default values */
	  code_type = CType();

	  nn = gp_mat->size1;
	  KK = gp_mat->size2;
	  
	  mm = KK - 1;
	  max_states = 1 << mm;			/* 2^mm */
	  
	  /* determine the DataLength */
	  DataLength = (CodeLength/nn)-mm;	

	  /* Convert code polynomial to binary */
	  g_encoder = (int*)calloc(nn, sizeof(int) );
	  
	  for (i = 0;i<nn;i++) {
	    for (j=0;j<KK;j++) {
	      elm = gsl_matrix_get(gp_mat,i,j);
	      if (elm != 0) {
		g_encoder[i] = g_encoder[i] + (int) pow(2,(KK-j-1)); 
	      }
	    }
	  }


	/* create appropriate transition matrices */
	  out0 = (int *)calloc( max_states, sizeof(int) );
	  out1 = (int *)calloc( max_states, sizeof(int) );
	  state0 = (int *)calloc( max_states, sizeof(int) );
	  state1 = (int *)calloc( max_states, sizeof(int) );


	if ( code_type ) {
		nsc_transit( out0, state0, 0, g_encoder, KK, nn );
		nsc_transit( out1, state1, 1, g_encoder, KK, nn );
	} else {
		rsc_transit( out0, state0, 0, g_encoder, KK, nn );
		rsc_transit( out1, state1, 1, g_encoder, KK, nn );
	}

	
	gsl_vector_uint *output = gsl_vector_uint_alloc(DataLength);
	output_u_int = (int *)calloc( DataLength, sizeof(int) );


	/* Run the Viterib algorithm */
	Viterbi( output_u_int, out0, state0, out1, state1,
		input_c_float, KK, nn, DataLength ); 

	/* cast to outputs */
	for (j=0;j<DataLength;j++) {
	  gsl_vector_uint_set(output,j,output_u_int[j]);
	}



	gsl_vector_uint_class outobj(output);
	//	outobj.show();

	vout1.DeliverDataObj(outobj);

	///////////////////////////////////////////////
	///////////////////////////////////////////////
	///////////////////////////////////////////////

	
	/* Clean up memory */
	free( out0 );
	free( out1 );
	free( state0 );
	free( state1 );
	free( g_encoder );
	free( input_c_float );
	free( output_u_int );

	gsl_vector_uint_free(output);


}




//
//
// Vector uint display
//
//
// void gsl_vector_uint_show(gsl_vector_uint *vec) {
//   unsigned int N=vec->size;
//   for (int i=0; i<N; i++) {
//       unsigned int el = gsl_vector_uint_get(vec,i);
//       cout << setw(3) 
// 	   << el;
//     }
//     cout << endl;
// }
// //
// void gsl_vector_show(gsl_vector *vec) {
//   unsigned int N=vec->size;
//   for (int i=0; i<N; i++) {
//       double el = gsl_vector_get(vec,i);
//       cout << setw(15) 
// 	   << el << endl;
//     }
//     cout << endl;
// }
//
//
// Matrix complex display
//
//
// void gsl_matrix_complex_show(gsl_matrix_complex *mat) {
//   unsigned int N=mat->size1, M=mat->size2;
//   for (int i=0; i<N; i++) {
//     for (int j=0; j<M; j++) {
//       gsl_complex el = gsl_matrix_complex_get(mat,i,j);
//       cout << "(" 
// 	   << GSL_REAL(el) 
// 	   << ","
// 	   << GSL_IMAG(el) 
// 	   << ") ";
//     }
//     cout << endl;
//   }
//   cout << endl;
// }
//
//
// Vector complex display
//
//
// void gsl_vector_complex_show(gsl_vector_complex *vec) {
//   unsigned int N=vec->size;
//   for (int i=0; i<N; i++) {
//       gsl_complex el = gsl_vector_complex_get(vec,i);
//       cout << "(" 
// 	   << GSL_REAL(el) 
// 	   << ","
// 	   << GSL_IMAG(el) 
// 	   << ") ";
//     }
//     cout << endl;
// }

//
//
// Matrix uint display
//
//
// void gsl_matrix_uint_show(gsl_matrix_uint *mat) {
//   unsigned int N=mat->size1, M=mat->size2;
//   for (int i=0; i<N; i++) {
//     for (int j=0; j<M; j++) {
//       unsigned int el = gsl_matrix_uint_get(mat,i,j);
//       cout << setw(3) 
// 	   << el;
//     }
//     cout << endl;
//   }
//   cout << endl;
// }

