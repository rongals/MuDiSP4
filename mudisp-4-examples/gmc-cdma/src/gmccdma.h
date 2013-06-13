//
// GMC CDMA Defines for System
//
//
#ifndef MUDISP_GMCCDMA_H
#define MUDISP_GMCCDMA_H 
//
//
// NOTE: not used in gmc-cdma-ai
// 
#define USERS 8


//
// uncomment for verbose debug output
//
//#define SHOW_MATRIX

//
// uncomment for original Giannakis setting 
// for theta matrix. The Vandermonde matrix is suitable
// for FCA subcarrier allocation only. For other subcarrier
// allocation methods the proposed Vandermonde encoding is
// preferred.
// 
//#define GIANNAKIS_PRECODING

//
//the constant ERROR_REPORT_INTERVAL defines the number of bits
//received from each user before creating a error report. The expected 
//rate of reports depends on the number of bits per users in a GMC-CDMA
//frame, i.e. bit_per_user_nbu = symbols_per_frame_K * bits_per_symbols_Nb
//
//Example: K=2, Nb=1, ERROR_REPORT_INTERVAL 1024 --> a report every 1024/2=512 frames
//
#define ERROR_REPORT_INTERVAL 50
#define GEO_UPDATE_INTERVAL 10

//
// PATHLOSS MODEL
//
// the following model has been adopted:
//
// the user choose the distance at which the mean Es/No is 0 (ESNO_ZERO_DISTANCE_M, desno0) and the
// distance at which the ploss is 0dB (PLOSS_ZERO_DISTANCE_M, dploss0)
// both expressed in metres, then the following apply
//
// EsNo(d) = Es [db] - ploss(d) [db] - No [db]
//
// EsNo(desno0) = Es [db] - ploss(desno0) [db] - No [db] = 0
//
// No [dB] = Es [dB] - ploss(desno0) [db]
//
// assuming Es [dB] = 0
//
// No = -ploss(desno0) [db]
//
// ploss(d) [dB] = 10 Log ( d^-3 / dploss^-3 ) = -30 Log(d) + 30 Log(dploss0)
//
//
#define PLOSS_ZERO_DISTANCE_M 1.0
#define ESNO_ZERO_DISTANCE_M 200000.0

#endif // MUDISP_GMCCDMA_H
