/* File convolutional.h
   
   Description: General functions used to implement convolutional encoding.   

   Copyright (C) 2006, Matthew C. Valenti

   Last updated on Mar. 14, 2006

   Functions  itob, parity_counter, nsc_enc_bit, rsc_enc_bit, nsc_transit, 
   rsc_transit, rsc_tail, conv_encode, and Gamma are part of the Iterative Solutions 
   Coded Modulation Library. The Iterative Solutions Coded Modulation 
   Library is free software; you can redistribute it and/or modify it 
   under the terms of the GNU Lesser General Public License as published 
   by the Free Software Foundation; either version 2.1 of the License, 
   or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
  
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include"convolutional.h"
#include<stdlib.h>

/* function itob()

  Description: Converts an integer symbol into a vector of bits

	Output parameters:
		binvec_p: The binary vector
		
    Input parameters:
	    symbol:  The integer-valued symbol
		length:  The length of the binary vector
	
  This function is used by conv_encode()  */

void itob(
						int	binvec_p[],
						int symbol,
						int length )
{
	int counter;

	/* Go through each bit in the vector */
	for (counter=0;counter<length;counter++) {
		binvec_p[length-counter-1] = (symbol&1);
		symbol = symbol>>1;
	}

	return;
}

/* function parity_counter()

  Copyright 2003, Matthew C. Valenti.
	
  Description: Determines if a symbol has odd (1) or even (0) parity

	Output parameters:
		(returned int): The symbol's parity = 1 for odd and 0 for even
		
    Input parameters:
	    symbol:  The integer-valued symbol
		length:  The highest bit position in the symbol
	
  This function is used by nsc_enc_bit(), rsc_enc_bit(), and rsc_tail()  */

int parity_counter( int symbol, int length )
{
	int counter;
	int temp_parity = 0;

	for (counter=0;counter<length;counter++) {
		temp_parity = temp_parity^(symbol&1);
		symbol = symbol>>1;
	}

	return( temp_parity );
}


/* Function nsc_enc_bit() 

  (c) Copyright 2003, Matthew C. Valenti.

  Description: Convolutionally encodes a single bit using a rate 1/n encoder.
  Takes in one input bit at a time, and produces a n-bit output.
  
	Input parameters:
		input		The input data bit (i.e. a 0 or 1).
		state_in	The starting state of the encoder (an int from 0 to 2^m-1).
		g[]			An n-element vector containing the code generators in binary form.
		KK			The constraint length of the convolutional code.

	Output parameters:
		output_p[]		An n-element vector containing the encoded bits.
		state_out_p[]	An integer containing the final state of the encoder	
						(i.e. the state after encoding this bit)
	
  This function is used by rsc_encode(), nsc_transit(), rsc_transit(), and nsc_transit() */

int nsc_enc_bit(
				   int	state_out_p[],
				   int	input,
				   int	state_in, 
				   int  g[],
				   int  KK,
				   int  nn )
{
	/* declare variables */
	int state, i;
	int out = 0;	

	/* create a word made up of state and new input */
	state = (input<<(KK-1))^state_in;
	
	/* AND the word with the generators */
	for (i=0;i<nn;i++)
    {		
		/* update output symbol */
		out = (out<<1) + parity_counter( state&g[i], KK ); 		
    }
	
	/* shift the state to make the new state */
	state_out_p[0] = state>>1; 	
	return(out);
}

int rsc_enc_bit(
				   int	state_out_p[],
				   int	input,
				   int	state_in, 
				   int  g[],
				   int  KK,
				   int  nn )
{
	/* declare variables */
	int state, i, out, a_k;	

	/* systematic output */
	out = input;

	/* determine feedback bit */
	a_k = input^parity_counter( g[0]&state_in, KK );

	/* create a word made up of state and feedback bit */
	state = (a_k<<(KK-1))^state_in;

	/* AND the word with the generators */
	for (i=1;i<nn;i++)
    {		
		/* update output symbol */
		out = (out<<1) + parity_counter( state&g[i], KK ); 		
    }
	
	/* shift the state to make the new state */
	state_out_p[0] = state>>1; 	
	return(out);
}

/* function that creates the transit and output vectors */
void nsc_transit(
						int		output_p[],
						int		trans_p[],
						int		input,
						int     g[],
						int     KK,
						int     nn )
{
	int nextstate[1];
	int state, states;
	states = (1<<(KK-1));  /* The number of states: 2^mm */

	/* Determine the output and next state for each possible starting state */
	for(state=0;state<states;state++) {
		output_p[state]  = nsc_enc_bit( nextstate, input, state, g, KK, nn ); 
		trans_p[state]  = nextstate[0];
	}
	return;
}

/* Function rsc_transit()

  Copyright 2001, Matthew C. Valenti.
	
  Description: Calculates the "transition matrix" for the trellis.
  This information tells the decoder what the next state and output bits
  will be given the current state and input bit.

	Input parameters:
		input		Either 0 or 1 --- the input data bit.
		g[]			A two element vector containing the code generators.
		KK			The constraint length of the convolutional code.

	Output parameters:
		output_p[]	A vector of length max_states = 2^(KK-1) containing
		            the output symbols.
		trans_p[]   A vector of length max_states that tells the decoder
					what the next state will be given the input and current state.
	
  This function is used by turbo_decode()   */

void rsc_transit(
						int	output_p[],
						int trans_p[],
						int	input,
						int g[],
						int KK,
						int nn )
{
	int nextstate[1];
	int state, states; 

	states = 1 << (KK-1); /* The number of states: 2^mm */

	/* Determine the output and next state for each possible starting state */
	for(state=0;state<states;state++) {
		output_p[state] = rsc_enc_bit( nextstate, input, state, g, KK, nn ); 
		trans_p[state]  = nextstate[0];
	}	
	return;
}

void rsc_tail(
						int	tail_p[],
						int g[],
						int max_states,
						int mm )
{
	int state;

	/* Determine the tail for each state */
	for(state=0;state<max_states;state++) {		
		/* determine feedback word */
		tail_p[state] = parity_counter( g[0]&state, mm );
	}
	return;
}

void conv_encode(
	     int		output_p[],
	     int		input[],
		 int		out0[], 
		 int		state0[], 
		 int		out1[], 
		 int		state1[],
         int		tail[],	 
         int        KK,
         int        LL,
		 int        nn )
{
  int i, j, inbit, outsym;
  int *bin_vec;
  int state = 0;

  bin_vec = (int*)calloc( nn, sizeof(int) );

  /* encode one bit at a time */
  for (i=0;i<LL+KK-1;i++) {
	  if (i<LL)
		  inbit = input[i]; /* data bit */
	  else
		  inbit = tail[state]; /* tail bit */
	  
	  if (inbit) {
		  /* Input is a one */
		  outsym = out1[state];  /* The output symbol */
		  
		  /* Determine next state */
		  state = state1[state];
	  } else {
		  /* Input is a zero */
		  outsym = out0[state];  /* The output symbol */
		  
		  /* Determine next state */
		  state = state0[state];
	  }

	  /* Convert symbol to a binary vector	*/
	  itob( bin_vec, outsym, nn );
		  
	  /* Assign to output */
	  for (j=0;j<nn;j++)
		  output_p[nn*i+j] = bin_vec[j];
  }

  free(bin_vec);

  return;
}


/* function Gamma()

  Description: Computes the branch metric used for decoding.

	Output parameters:
		(returned float) 	The metric between the hypothetical symbol and the recevieved vector
	
	Input parameters:
		rec_array			The received vector, of length nn
		symbol				The hypothetical symbol
		nn					The length of the received vector
	
  This function is used by siso()  */


float Gamma(float  rec_array[],
				   int    symbol,
				   int    nn )
{
	float rm = 0;
	int i;
	int mask;
	
	mask = 1;
	for (i=0;i<nn;i++) {
		if (symbol&mask)
			rm += rec_array[nn-i-1];
		mask = mask<<1;
	} 
	
	return(rm);
} 


/* Function Viterbi()

  Description: Uses the Viterbi algorithm to perform hard-decision decoding of a convolutional code.

	Input parameters:
		out0[]		The output bits for each state if input is a 0 (generated by rsc_transit).
		state0[]	The next state if input is a 0 (generated by rsc_transit).
		out1[]		The output bits for each state if input is a 1 (generated by rsc_transit).
		state1[]	The next state if input is a 1 (generated by rsc_transit).
		r[]			The received signal in LLR-form. For BPSK, must be in form r = 2*a*y/(sigma^2).
		KK			The constraint length of the convolutional code.
		LL			The number of data bits.
		DecOpts		Decoder termination option = 0 for unterminated and = 1 for terminated.

	Output parameters:
		est[]		The log-likelihood ratio of each data bit.
	
  This function is used by turbo_decode()   
  
*/


void Viterbi(
				int    output_u_int[],
				int    out0[], 
				int    state0[], 
				int    out1[], 
				int    state1[],
				float  input_c[],
				int    KK,
				int    nn,
				int    LL
				)
{
	int i, t, state, bit, mm, states;
	int number_symbols;
	float metric;
	float *trellis;
	int *prev_bit;
	int *prev_state;
	float *metric_c;	/* Set of all possible branch metrics */
	float *rec_array;   /* Received values for one trellis section */
	float max_val;
	
	/* some derived constants */
	mm = KK-1;
	states = 1 << mm;			/* 2^mm */
	number_symbols = 1 << nn;	    /* 2^nn */
	
	/* dynamically allocate memory */ 
	trellis = (float*)calloc( states*(LL+KK), sizeof(float) );
	prev_bit = (int*)calloc( states*(LL+KK), sizeof(int) );
	prev_state = (int*)calloc( states*(LL+KK), sizeof(int) );
	rec_array = (float*)calloc( nn, sizeof(float) );
	metric_c = (float*)calloc( number_symbols, sizeof(float) );
	
	/* initialize trellis */
	for (state=0;state<states;state++) {
		for (t=0;t<LL+KK;t++) {
			trellis[states*t+state] = -MAXLOG;
		}
	}
	trellis[0] = 0; /* start in all-zeros state */
	
	/* go through trellis */
	for (t=0;t<LL+mm;t++) {
		for (i=0;i<nn;i++)
			rec_array[i] = input_c[nn*t+i];
		
		/* precompute all possible branch metrics */
		for (i=0;i<number_symbols;i++)
			metric_c[i] = Gamma( rec_array, i, nn ); 
		
		/* step through all states */
		for (state=0;state<states;state++) {
			
			/* hypothesis: info bit is a zero */
			metric = trellis[t*states+state] + metric_c[ out0[ state ] ];
			
			/* store new metric if more than metric in storage */
			if ( metric > trellis[(t+1)*states+state0[state]] ) {
				trellis[(t+1)*states+state0[state]] = metric;
				prev_state[(t+1)*states+state0[state]] = state;
				prev_bit[(t+1)*states+state0[state]] = 0;
			}
			
			/* hypothesis: info bit is a one */
			metric = trellis[t*states+state] + metric_c[ out1[ state ] ];
			
			/* store new metric if more than metric in storage */
			if ( metric > trellis[(t+1)*states+state1[state]] ) {
				trellis[(t+1)*states+state1[state]] = metric;
				prev_state[(t+1)*states+state1[state]] = state;
				prev_bit[(t+1)*states+state1[state]] = 1;
			}
		}

		/* normalize */
		max_val = 0;
		for (state=0;state<states;state++)
			if (trellis[(t+1)*states+state]>max_val)
				max_val = trellis[(t+1)*states+state];		
			for (state=0;state<states;state++) 
				trellis[(t+1)*states+state] = trellis[(t+1)*states+state]-max_val;
	}
	
	/* trace-back operation */
	state = 0;

	/* tail, no need to output */
	for (t=LL+mm; t>LL; t--) {
		bit = prev_bit[t*states+state];
		state = prev_state[t*states+state];
	}

	for (t=LL; t>0; t--) {
		bit = prev_bit[t*states+state];
		state = prev_state[t*states+state];
		output_u_int[t-1] = bit;
		/* printf( "out[%d] = %d\n", t, bit ); */
	}
	
	/* free the dynamically allocated memory */
	free(trellis);
	free(prev_bit);
	free(prev_state);
	free(rec_array);
	free(metric_c); 
	
}

