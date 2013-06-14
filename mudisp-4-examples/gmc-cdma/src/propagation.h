/*
 * propagation.h
 *
 *  Created on: 14/giu/2013
 *      Author: ronga
 */

#ifndef MUDISP3_PROPAGATION_H_
#define MUDIPS3_PROPAGATION_H_

namespace mudisp {


	double OkumuraHataCitydB(double distance_m, double freq_mhz, double hbs_m=200, double hm_m=3);

	double OkumuraHataMetrodB(double distance_m, double freq_mhz, double hbs_m=200, double hm_m=3);

	double OkumuraHataSuburbandB(double distance_m, double freq_mhz, double hbs_m=200, double hm_m=3);

	double OkumuraHataRuraldB(double distance_m, double freq_mhz, double hbs_m=200, double hm_m=3);


	double  dbtolin(double dB);

	double  lintodb(double lin);

}


#endif /* PROPAGATION_H_ */
