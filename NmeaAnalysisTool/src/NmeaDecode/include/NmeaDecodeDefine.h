/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#ifndef CXX_NMEADECODEDEFINE_H
#define CXX_NMEADECODEDEFINE_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Included files

/*---------------------------------------------------------------------------*/
// Value define

static const INT    MAX_GPS_SATELLITE   = 64;

// Define NMEA receive source
enum NmeaReceiveSource
{
    NMEA_RECEIVE_SOURCE_NONE = 0,
    NMEA_RECEIVE_SOURCE_ANDROID,
    NMEA_RECEIVE_SOURCE_IPHONE,
    NMEA_RECEIVE_SOURCE_CRADLE,
    NMEA_RECEIVE_SOURCE_OTHER,
};

// Define NMEA recode type
enum NmeaDecodeTypeDef
{
    NMEA_DECODE_TYPE_NONE = 0,
    NMEA_DECODE_TYPE_GGA,
    NMEA_DECODE_TYPE_RMC,
    NMEA_DECODE_TYPE_GSA,
    NMEA_DECODE_TYPE_GSV,
};

/*---------------------------------------------------------------------------*/
//  structure define

// Satellite information
struct SatelliteInfo
{
    BOOL            bValid;
    BOOL            bUsed;
    BYTE            bySatelliteID;
    FLOAT           fElevation;
    FLOAT           fAzimuth;
    FLOAT           fSNR;
};

// Output GPS data value
struct GPS_DATA
{
    DOUBLE          dLongitude;         // [deg]
    DOUBLE          dLatitude;          // [deg]
    FLOAT           fAltitude;          // [m]
    INT             iSatelliteCnt;
    SYSTEMTIME      sUtcTime;
    FLOAT           fVelocity;          // [km/h]
    FLOAT           fDirection;         // [deg] (north is 0, clock-ward is positive)
    INT             iDimension;         // 0D/2D/3D
    FLOAT           fPdop;              // 0 ~ 25.5
    FLOAT           fHdop;              // 0 ~ 25.5
    FLOAT           fVdop;              // 0 ~ 25.5
    SatelliteInfo   sSatelliteInfo[MAX_GPS_SATELLITE];
};

/*---------------------------------------------------------------------------*/
#endif  // CXX_NMEADECODEDEFINE_H

/*---------------------------------------------------------------------------*/
/* EOF */
