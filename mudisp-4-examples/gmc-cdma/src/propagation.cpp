/*
 * propagation.cpp
 *
 *  Created on: 14/giu/2013
 *      Author: ronga
 */


#include "propagation.h"

#include <math.h>
#include "gsl/gsl_math.h"


namespace mudisp {


double OkumuraHataCitydB(double distance_m, double freq_mhz, double hbs_m, double hm_m){

	double A = 69.55 + 26.16 * log10( freq_mhz ) - 13.82 * log10( hbs_m );
	double B = 44.9 - 6.55 * log10( hm_m );
	double C = 0;
	double ahm = (1.1 * log10( freq_mhz ) - 0.7) * hm_m - (1.56 * log10( freq_mhz ) - 0.8);

	return A - ahm + B * log10( distance_m / 1000.0 ) + C;

}

double OkumuraHataMetrodB(double distance_m, double freq_mhz, double hbs_m, double hm_m){

	double A = 69.55 + 26.16 * log10( freq_mhz ) - 13.82 * log10( hbs_m );
	double B = 44.9 - 6.55 * log10( hm_m );
	double C = 0;
	double ahm;
	if (freq_mhz < 400)
		ahm = (8.29 * gsl_pow_2(log10(  1.54 * hm_m )) - 1.1);
	else
		ahm = (3.2  * gsl_pow_2(log10( 11.75 * hm_m )) - 4.97);

	return A - ahm + B * log10( distance_m / 1000.0 ) + C;

}

double OkumuraHataSuburbandB(double distance_m, double freq_mhz, double hbs_m, double hm_m) {

	double A = 69.55 + 26.16 * log10( freq_mhz ) - 13.82 * log10( hbs_m );
	double B = 44.9 - 6.55 * log10( hm_m );
	double C = -2 * gsl_pow_2(log10( freq_mhz / 28.0 )) - 5.4;
	double ahm = (1.1 * log10( freq_mhz ) - 0.7) * hm_m - (1.56 * log10( freq_mhz ) - 0.8);

	return A - ahm + B * log10( distance_m / 1000.0 ) + C;

}


double OkumuraHataRuraldB(double distance_m, double freq_mhz, double hbs_m, double hm_m) {

	double A = 69.55 + 26.16 * log10( freq_mhz ) - 13.82 * log10( hbs_m );
	double B = 44.9 - 6.55 * log10( hm_m );
	double C = -4.78 * gsl_pow_2(log10( freq_mhz )) + 18.33 * log10( freq_mhz ) - 40.98;
	double ahm = (1.1 * log10( freq_mhz ) - 0.7) * hm_m - (1.56 * log10( freq_mhz ) - 0.8);

	return A - ahm + B * log10( distance_m / 1000.0 ) + C;

}

	 double dbtolin(double db){
		return pow( 10.0 , db/10.0 );
	}

	 double lintodb(double lin){
		return 10.0 * log10( lin );
	}

}
