//
// MuDiSP3
// Multirate Digital Signal Processor 3
// By Luca Simone Ronga (C)
// 
//
// set of C++ wrappers to GSL vector and matrix data structures
// for MuDiSP port transit
//
//
#ifndef MUDISP_GSL_H
#define MUDISP_GSL_H

#include "gsl/gsl_vector.h"
#include "gsl/gsl_matrix.h"


class gsl_vector_class {

public: 

  gsl_vector *vec;

  // conctructors
  gsl_vector_class();
  gsl_vector_class(const gsl_vector_class &);
  gsl_vector_class(gsl_vector *);

  // denstructor
  ~gsl_vector_class();

  // debugging method
  void show();

};


class gsl_vector_complex_class {

public: 

  gsl_vector_complex *vec;

  // conctructors

  gsl_vector_complex_class();
  gsl_vector_complex_class(const gsl_vector_complex_class &);
  gsl_vector_complex_class(gsl_vector_complex *);

  // denstructor
  ~gsl_vector_complex_class();

  // debugging method
  void show();

};


class gsl_vector_uint_class {

public: 

  gsl_vector_uint *vec;

  // conctructors
  gsl_vector_uint_class();
  gsl_vector_uint_class(const gsl_vector_uint_class &);
  gsl_vector_uint_class(gsl_vector_uint *);

  // denstructor
  ~gsl_vector_uint_class();

  // debugging method
  void show();

};

//
//
//


class gsl_matrix_class {

public: 

  gsl_matrix *mat;

  // conctructors
  gsl_matrix_class();
  gsl_matrix_class(const gsl_matrix_class &);
  gsl_matrix_class(gsl_matrix *);

  // denstructor
  ~gsl_matrix_class();

  // debugging method
  void show();

};


class gsl_matrix_complex_class {

public: 

  gsl_matrix_complex *mat;

  // conctructors
  gsl_matrix_complex_class();
  gsl_matrix_complex_class(const gsl_matrix_complex_class &);
  gsl_matrix_complex_class(gsl_matrix_complex *);

  // denstructor
  ~gsl_matrix_complex_class();

  // debugging method
  void show();

};


class gsl_matrix_uint_class {

public: 

  gsl_matrix_uint *mat;

  // conctructors
  gsl_matrix_uint_class();
  gsl_matrix_uint_class(const gsl_matrix_uint_class &);
  gsl_matrix_uint_class(gsl_matrix_uint *);

  // denstructor
  ~gsl_matrix_uint_class();

  // debugging method
  void show();

};



#endif // MUDISP_GSL_H
