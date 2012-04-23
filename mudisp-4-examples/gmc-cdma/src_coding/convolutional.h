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

#ifndef MUDISP3_CONVOLUTIONAL_H
#define MUDISP3_CONVOLUTIONAL_H

/* define constants used throughout the library */
#define MAXLOG 1e7  /* Define infinity */

/* function itob()

  Description: Converts an integer symbol into a vector of bits

	Output parameters:
		binvec_p: The binary vector
		
    Input parameters:
	    symbol:  The integer-valued symbol
		length:  The length of the binary vector
	
  This function is used by conv_encode()  */

void itob( int*, int, int );

/* function parity_counter()

  Copyright 2003, Matthew C. Valenti.
	
  Description: Determines if a symbol has odd (1) or even (0) parity

	Output parameters:
		(returned int): The symbol's parity = 1 for odd and 0 for even
		
    Input parameters:
	    symbol:  The integer-valued symbol
		length:  The highest bit position in the symbol
	
  This function is used by nsc_enc_bit(), rsc_enc_bit(), and rsc_tail()  */

int parity_counter( int, int );


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
				   int  nn );
int rsc_enc_bit(
				   int	state_out_p[],
				   int	input,
				   int	state_in, 
				   int  g[],
				   int  KK,
				   int  nn );
/* function that creates the transit and output vectors */
void nsc_transit(
						int		output_p[],
						int		trans_p[],
						int		input,
						int     g[],
						int     KK,
						int     nn );

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
						int nn );

void rsc_tail(
						int	tail_p[],
						int g[],
						int max_states,
						int mm );

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
	     int        nn );

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
		   int    nn );

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
				);



#endif
