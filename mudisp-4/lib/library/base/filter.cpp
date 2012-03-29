//
// MuDiSP3 (Multirate Digital Signal Processor 3)
// Copyright 1999,2005 Luca Simone Ronga
//
// Contributor: Simone Menci
// 
//  This file is part of MuDiSP3.
//
//  MuDiSP3 is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  MuDiSP3 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with MuDiSP3; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
/////////////////////////////////////////////////////////

//
//
//
//

#include <library/base/filter.h>

//
// FILTER_DOUBLE
//

IIR::IIR(double *b, double *a, const int b_length, const int a_length) {

  int j;
  mem_b = new double[b_length];
  mem_a = new double[a_length];
  coeff_b = new double[b_length];
  coeff_a = new double[a_length];
  for (j=0;j<b_length;j++) {
    mem_b[j]=0.0;
    coeff_b[j]=b[j];
  }
  for (j=0;j<a_length;j++) {
    mem_a[j]=0.0;
    coeff_a[j]=a[j];
  }
}


IIR::IIR(const char *bfname, const char *afname) {

//
// Reading from num and den data files
//
  ifstream ibfile(bfname), iafile(afname);
  int b_length, a_length;

  if (!ibfile) {
    cerr << "Error accessing IIR file: " << bfname << endl;
    exit(1001);
  }
  if (!iafile) {
    cerr << "Error accessing IIR file: " << afname << endl;
    exit(1002);
  }

  ibfile >> b_length;
  iafile >> a_length;
  lb=b_length;
  la=a_length;
  long j;
  mem_b = new double[b_length];
  mem_a = new double[a_length];
  coeff_b = new double[b_length];
  coeff_a = new double[a_length];

  for (j=0;j<b_length;j++) {
    mem_b[j]=0.0;
    ibfile >> coeff_b[j];
  }
  for (j=0;j<a_length;j++) {
    mem_a[j]=0.0;
    iafile >> coeff_a[j];
  }

  ibfile.close();
  iafile.close();
}


IIR::~IIR() {

  delete [] coeff_b;
  delete [] coeff_a;
  delete [] mem_b;
  delete [] mem_a;
}


double IIR::filtra(const double &val) {

  register int i;

  mem_a[0]=0.0;
  //
  // numerator and shift
  //
  mem_b[0]=val;
  for (i=lb-1;i>0;i--) {
    mem_a[0] += coeff_b[i]*mem_b[i];
    mem_b[i]=mem_b[i-1];
  }
  mem_a[0] += coeff_b[0]*mem_b[0];
  //
  // denom and shift
  //
  for (i=la-1;i>0;i--){
    mem_a[0] -= coeff_a[i]*mem_a[i];
    mem_a[i]=mem_a[i-1];
  }

  return mem_a[0]; 
}


FIR::~FIR() {

  delete [] coeff_b;
  delete [] mem_b;
}


FIR::FIR(double *b, const int b_length) {

  lb=b_length;
  mem_a=NULL;
  coeff_a=NULL;
  mem_b=new double[b_length];
  coeff_b=new double[b_length];

  long i;
  for (i=0;i<b_length;i++) { 
    mem_b[i]=0.0;
    coeff_b[i]=b[i];
  }
}


FIR::FIR(const char *bfname) {

//
// Reading from num and den data files
//
  ifstream ibfile(bfname);
  int b_length;

  if (!ibfile) {
    cerr << "Error accessing FIR coeff file: " << bfname << endl;
    exit(1001);
  }

  ibfile >> b_length;
  lb=b_length;
  long j;
  mem_b = new double[b_length];
  mem_a = NULL;
  coeff_b = new double[b_length];
  coeff_a = NULL;

  for (j=0;j<b_length;j++) {
    mem_b[j]=0.0;
    ibfile >> coeff_b[j];
  }

  ibfile.close();
}


double FIR::filtra(const double & val) {

  register int i; 

  double out=0.0; 
  //
  // numerator and shift
  //
  mem_b[0]=val;
  for (i=lb-1;i>0;i--) {
    out += coeff_b[i]*mem_b[i];
	mem_b[i]=mem_b[i-1];
  }
  out += coeff_b[0]*mem_b[0];

  return out;
}


//
// FILTER_COMPLEX
//

IIRCx::IIRCx(double *b, double *a, const int b_length, const int a_length) : lb(b_length), la(a_length) {

  int j;
  mem_bx = new complex < double >[b_length];
  mem_ax = new complex < double >[a_length];
  coeff_b = new double[b_length];
  coeff_a = new double[a_length];
  for (j=0;j<b_length;j++) {
    coeff_b[j]=b[j];
  }
  for (j=0;j<a_length;j++) {
    coeff_a[j]=a[j];
  }
}


IIRCx::IIRCx(const char *bfname, const char *afname) {

//
// Reading from num and den data files
//
  ifstream ibfile(bfname), iafile(afname);
  int b_length, a_length;

  if (!ibfile) {
    cerr << "Error accessing IIR file: " << bfname << endl;
    exit(1001);
  }

  if (!iafile) {
    cerr << "Error accessing IIR file: " << afname << endl;
    exit(1002);
  }

  ibfile >> b_length;
  iafile >> a_length;
  lb=b_length;
  la=a_length;
  long j;
  mem_bx = new complex < double >[b_length];
  mem_ax = new complex < double >[a_length];
  coeff_b = new double[b_length];
  coeff_a = new double[a_length];

  for (j=0;j<b_length;j++) {
    ibfile >> coeff_b[j];
  }
  for (j=0;j<a_length;j++) {
    iafile >> coeff_a[j];
  }

  ibfile.close();
  iafile.close();
}


IIRCx::~IIRCx() {

  delete [] coeff_b;
  delete [] coeff_a;
  delete [] mem_bx;
  delete [] mem_ax;
}


complex < double > IIRCx::filtracx(const complex < double > &val) {

  register int i;

  //
  // numerator and shift
  //
  mem_bx[0]=val;
  mem_ax[0]=complex < double > (0,0);
  for (i=lb-1;i>0;i--) {
  mem_ax[0] += mem_bx[i]*(complex < double > (coeff_b[i],0));
    mem_bx[i]=mem_bx[i-1];
  }
  mem_ax[0] += mem_bx[0]*(complex < double > (coeff_b[0],0));

  //
  // denom and shift
  //
  for (i=la-1;i>0;i--){
    mem_ax[0] -= mem_ax[i]*(complex < double > (coeff_a[i],0));
    mem_ax[i]=mem_ax[i-1];
  }

  return mem_ax[0];
}


FIRCx::~FIRCx() {

  delete [] coeff_b;
  delete [] mem_bx;
}


FIRCx::FIRCx(double *b, const int b_length) {

  lb=b_length;
  mem_ax=NULL;
  coeff_a=NULL;
  mem_bx=new complex < double >[b_length];
  coeff_b=new double[b_length];

  long i;
  for (i=0;i<b_length;i++) {
    mem_bx[i]=complex <double>(0,0);
    coeff_b[i]=b[i];
  }
}


FIRCx::FIRCx(const char *bfname) {

//
// Reading from num and den data files
//
  ifstream ibfile(bfname);
  int b_length;

  if (!ibfile) {
    cerr << "Error accessing FIR coeff file: " << bfname << endl;
    exit(1001);
  }

  ibfile >> b_length;
  lb=b_length;
  long j;
  mem_bx = new complex < double >[b_length];
  mem_ax = NULL;
  coeff_b = new double[b_length];
  coeff_a = NULL;

  for (j=0;j<b_length;j++) {
    ibfile >> coeff_b[j];
  }

  ibfile.close();
}


complex < double > FIRCx::filtracx(const complex < double > &val) {

  register int i;

  complex < double > out=complex < double >(0.0,0.0);
  //
  // numerator and shift
  //
  mem_bx[0]=val;
  for (i=lb-1;i>0;i--) {
    out += coeff_b[i]*mem_bx[i];
    mem_bx[i]=mem_bx[i-1];
  }
  out += coeff_b[0]*mem_bx[0];

  return out;
}


