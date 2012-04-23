//
// MuDiSP3
// Multirate Digital Signal Processor 3
// By Luca Simone Ronga (C)
// 
//
#include "gsl_mudisp.h"
#include <iostream>
#include <iomanip>

using namespace std;

gsl_vector_class::gsl_vector_class() {
  //  vec = gsl_vector_alloc(s);
}


gsl_vector_class::gsl_vector_class(const gsl_vector_class &tvecc){
  unsigned int size = tvecc.vec->size;
  vec = gsl_vector_alloc(size);
  gsl_vector_memcpy(vec,tvecc.vec);
}


gsl_vector_class::gsl_vector_class(gsl_vector *tvec){
  unsigned int size = tvec->size;
  vec = gsl_vector_alloc(size);
  gsl_vector_memcpy(vec,tvec);
}


// denstructor
gsl_vector_class::~gsl_vector_class(){
  gsl_vector_free(vec);
}


// debugging method
void gsl_vector_class::show(){
  unsigned int size=vec->size;
  for (int i=0; i<size; i++) {
    double el = gsl_vector_get(vec,i);
    cout << setw(15) 
	 << el << endl;
  }
  cout << endl;
}


//
//
//
gsl_vector_complex_class::gsl_vector_complex_class() {
  //  vec = gsl_vector_complex_alloc(s);
}


gsl_vector_complex_class::gsl_vector_complex_class(const gsl_vector_complex_class &tvecc){
  unsigned int size = tvecc.vec->size;
  vec = gsl_vector_complex_alloc(size);
  gsl_vector_complex_memcpy(vec,tvecc.vec);
}


gsl_vector_complex_class::gsl_vector_complex_class(gsl_vector_complex *tvec){
  unsigned int size = tvec->size;
  vec = gsl_vector_complex_alloc(size);
  gsl_vector_complex_memcpy(vec,tvec);
}


// denstructor
gsl_vector_complex_class::~gsl_vector_complex_class(){
  gsl_vector_complex_free(vec);
}


// debugging method
void gsl_vector_complex_class::show(){
  unsigned int size=vec->size;
  for (int i=0; i<size; i++) {
    gsl_complex el = gsl_vector_complex_get(vec,i);
    cout << "(" 
	 << GSL_REAL(el) 
	 << ","
	 << GSL_IMAG(el) 
	 << ") ";
  }
  cout << endl;
}



//
//
//
gsl_vector_uint_class::gsl_vector_uint_class() {
  //  vec = gsl_vector_uint_alloc(s);
}



gsl_vector_uint_class::gsl_vector_uint_class( const gsl_vector_uint_class &tvecc ){
  unsigned int size = tvecc.vec->size;
  vec = gsl_vector_uint_alloc(size);
  gsl_vector_uint_memcpy(vec,tvecc.vec);
}


gsl_vector_uint_class::gsl_vector_uint_class(gsl_vector_uint *tvec){
  unsigned int size = tvec->size;
  vec = gsl_vector_uint_alloc(size);
  gsl_vector_uint_memcpy(vec,tvec);
}


// denstructor
gsl_vector_uint_class::~gsl_vector_uint_class(){
  gsl_vector_uint_free(vec);
}


// debugging method
void gsl_vector_uint_class::show(){
  unsigned int size=vec->size;
  for (int i=0; i<size; i++) {
    unsigned int el = gsl_vector_uint_get(vec,i);
    cout << setw(15) 
	 << el << endl;
  }
  cout << endl;
}

//
// MATRIX
//

gsl_matrix_class::gsl_matrix_class() {
  //  mat = gsl_matrix_alloc(s1,s2);
}


gsl_matrix_class::gsl_matrix_class(const gsl_matrix_class &tmatc){
  unsigned int size1 = tmatc.mat->size1;
  unsigned int size2 = tmatc.mat->size2;
  mat = gsl_matrix_alloc(size1,size2);
  gsl_matrix_memcpy(mat,tmatc.mat);
}


gsl_matrix_class::gsl_matrix_class(gsl_matrix *tmat){
  unsigned int size1 = tmat->size1;
  unsigned int size2 = tmat->size2;
  mat = gsl_matrix_alloc(size1,size2);
  gsl_matrix_memcpy(mat,tmat);
}


// denstructor
gsl_matrix_class::~gsl_matrix_class(){
  gsl_matrix_free(mat);
}


// debugging method
void gsl_matrix_class::show(){
  unsigned int size1=mat->size1;
  unsigned int size2=mat->size2;
  for (int i=0; i<size1; i++) {
    for (int j=0; j<size2; j++) {
      double el = gsl_matrix_get(mat,i,j);
      cout << setw(15) 
	   << el << endl;
    }
    cout << endl;
  }
  cout << endl;
}


//
//
//

gsl_matrix_complex_class::gsl_matrix_complex_class() {
  //  mat = gsl_matrix_complex_alloc(s1,s2);
}


gsl_matrix_complex_class::gsl_matrix_complex_class(const gsl_matrix_complex_class &tmatc){
  unsigned int size1 = tmatc.mat->size1;
  unsigned int size2 = tmatc.mat->size2;
  mat = gsl_matrix_complex_alloc(size1,size2);
  gsl_matrix_complex_memcpy(mat,tmatc.mat);
}


gsl_matrix_complex_class::gsl_matrix_complex_class(gsl_matrix_complex *tmat){
  unsigned int size1 = tmat->size1;
  unsigned int size2 = tmat->size2;
  mat = gsl_matrix_complex_alloc(size1,size2);
  gsl_matrix_complex_memcpy(mat,tmat);
}


// denstructor
gsl_matrix_complex_class::~gsl_matrix_complex_class(){
  gsl_matrix_complex_free(mat);
}


// debugging method
void gsl_matrix_complex_class::show(){
  unsigned int size1=mat->size1;
  unsigned int size2=mat->size2;
  for (int i=0; i<size1; i++) {
    for (int j=0; j<size2; j++) {
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
//
gsl_matrix_uint_class::gsl_matrix_uint_class() {
  //  mat = gsl_matrix_uint_alloc(s1,s2);
}


gsl_matrix_uint_class::gsl_matrix_uint_class( const gsl_matrix_uint_class &tmatc ){
  unsigned int size1 = tmatc.mat->size1;
  unsigned int size2 = tmatc.mat->size2;
  mat = gsl_matrix_uint_alloc(size1,size2);
  gsl_matrix_uint_memcpy(mat,tmatc.mat);
}


gsl_matrix_uint_class::gsl_matrix_uint_class(gsl_matrix_uint *tmat){
  unsigned int size1 = tmat->size1;
  unsigned int size2 = tmat->size2;
  mat = gsl_matrix_uint_alloc(size1,size2);
  gsl_matrix_uint_memcpy(mat,tmat);
}


// denstructor
gsl_matrix_uint_class::~gsl_matrix_uint_class(){
  gsl_matrix_uint_free(mat);
}


// debugging method
void gsl_matrix_uint_class::show(){
  unsigned int size1=mat->size1;
  unsigned int size2=mat->size2;
  for (int i=0; i<size1; i++) {
    for (int j=0; j<size2; j++) {
      unsigned int el = gsl_matrix_uint_get(mat,i,j);
      cout << setw(15) 
	   << el << endl;
    }
    cout << endl;
  }
  cout << endl;
}

