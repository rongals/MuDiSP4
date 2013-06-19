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
//the constant ERROR_REPORT_UPDATE_FR defines the number of frames
//received from each user before updating an error report.
//
#define ERROR_REPORT_UPDATE_FR 50

// symbol tyme in microseconds
#define OFDM_SYMBOL_TIME_US 126.0

// every x OFDM symbols a spatial channel update
#define GEO_UPDATE_INTERVAL 10


#endif // MUDISP_GMCCDMA_H
