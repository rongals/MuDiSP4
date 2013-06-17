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

// symbol tyme in microseconds
#define OFDM_SYMBOL_TIME_US 126.0

// every x OFDM symbols a spatial channel update
#define GEO_UPDATE_INTERVAL 10


#endif // MUDISP_GMCCDMA_H
