//
// MuDiSP3 (Multirate Digital Signal Processor 3)
// Copyright 1999, Luca Simone Ronga
//
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



#include <mudisp/engine/lcm.h>


unsigned int GCD( unsigned int a, unsigned int b ) {

  if ( b > a )
    if ( b % a == 0 )
      return a;
    else
      return GCD( a , b%a);
  else
    if ( a % b == 0 )
      return b;
    else
      return GCD(b, a%b);

}

unsigned int LCM( unsigned int a, unsigned int b ) {

  unsigned int tmp=a/GCD(a,b);
  return tmp*b;

}

// #include <iostream>

// void main()
// {

//   unsigned int a,b;

//   cout << "a= ";
//   cin >> a;
 
//   cout << "b= ";
//   cin >> b;
 
//   cout << "GCD= " << GCD(a,b) << "  LCM= " << LCM(a,b) << endl;

// }
