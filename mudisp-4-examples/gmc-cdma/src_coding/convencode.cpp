//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//


#include "convencode.h"
#include "convolutional.h"
#include <iostream>


void ConvEncoder::Setup() {

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

  gsl_vector_uint *initVec = gsl_vector_uint_alloc(11);
  initVecClass = gsl_vector_uint_class(initVec);

//////// rate declaration for ports

}


void ConvEncoder::Run() {

  //double	*g_array;
	double	*output_p;
	int     DataLength, CodeLength, i, j, index;
	int     subs[] = {1,1};
	int     *g_encoder;
	int	nn, KK, mm, code_type, max_states;
	double  elm;
	int	*input_int, *output_int;
	int     *out0, *out1, *state0, *state1, *tail;



/// fetch data objects


	  /* first input is the data word */
	  gsl_vector_uint_class inputobj = vin1.GetDataObj(initVecClass);
	  gsl_vector_uint_class inputobj1 = vin1.GetDataObj(initVecClass);
	  gsl_vector_uint_class inputobj2 = vin1.GetDataObj(initVecClass);

	

	  DataLength = inputobj.vec->size; /* number of data bits */
	  
	  /* cast the input into a vector of integers */
	  input_int = (int*)calloc( DataLength, sizeof(int) );

	  for (i=0;i<DataLength;i++)
	    input_int[i] = (int) gsl_vector_uint_get(inputobj.vec,i);
	  
	  
	  //////// production of data
	  
	  code_type = CType(); /* Default:Code is RSC */
	  
	  
	  /* second input specifies the code polynomial */
	  //g_array = gp_mat->data; //mxGetPr(GENENCODER); *************************	
	  nn = gp_mat->size1;     // mxGetM(GENENCODER); *************************
	  KK = gp_mat->size2;     // mxGetN(GENENCODER); *********************

	  mm = KK - 1;
	  max_states = 1 << mm;
	  

	  
	  /* Determine the length of the output */
	  CodeLength = nn*(DataLength+mm);
	  
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
	  
	  /* create the output vector */		
	  output_int = (int*)calloc( CodeLength, sizeof( int ) );
	  gsl_vector_uint *output = gsl_vector_uint_alloc(CodeLength);
	  
	  /* create appropriate transition matrices */
	  out0 = (int*)calloc( max_states, sizeof(int) );
	  out1 = (int*)calloc( max_states, sizeof(int) );
	  state0 = (int*)calloc( max_states, sizeof(int) );
	  state1 = (int*)calloc( max_states, sizeof(int) );
	  tail = (int*)calloc( max_states, sizeof(int) );
	  
	  if ( code_type ) {
	    nsc_transit( out0, state0, 0, g_encoder, KK, nn );
	    nsc_transit( out1, state1, 1, g_encoder, KK, nn );
	  } else {
	    rsc_transit( out0, state0, 0, g_encoder, KK, nn );
	    rsc_transit( out1, state1, 1, g_encoder, KK, nn );
	    rsc_tail( tail, g_encoder, max_states, mm );
	  }
	  
	  /* Encode */
	  conv_encode( output_int, input_int, out0, state0, out1, state1, tail, KK, DataLength, nn );	
	  
	  /* cast to output */
	  for (i=0;i<CodeLength;i++) 			
	    gsl_vector_uint_set(output,i,output_int[i]);




	  vout1.DeliverDataObj( gsl_vector_uint_class(output) );


	  gsl_vector_uint_free(output);
	  	  
	  /* Clean up memory */
	  free( output_int );
	  free( input_int );
	  free( g_encoder );
	  free( out0 );
	  free( out1 );
	  free( state0 );
	  free( state1 );
	  free( tail );
 
}



