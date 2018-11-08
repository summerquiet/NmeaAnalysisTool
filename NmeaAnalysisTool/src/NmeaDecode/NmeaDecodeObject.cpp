/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
//  Include files

#ifndef CXX_NMEADECODEOBJECT_H
#   include "NmeaDecodeObject.h"
#endif

/*---------------------------------------------------------------------------*/
//  static parameter announce

static const INT    NMEA_FIELD_DATA_SIZE = 64;          // NMEA recode field data size
static const FLOAT  KNOT_TO_KILOPERHOUR = 1.852F;       // a knot is 1.852 km/h

/*---------------------------------------------------------------------------*/
//  static parameter set

/*---------------------------------------------------------------------------*/
// Is valid or not
BOOL CNmeaDecodeObject::IsValid(VOID)
{
    return m_bIsValid;
}

/*---------------------------------------------------------------------------*/
// Constructor
CNmeaDecodeObject::CNmeaDecodeObject(const CHAR* pStrData)
: m_bIsValid(FALSE)
, m_iCurFieldOffset(0)
, m_iCurFieldNum(0)
{

}

/*---------------------------------------------------------------------------*/
// Destructor
CNmeaDecodeObject::~CNmeaDecodeObject()
{

}

/*---------------------------------------------------------------------------*/
// Get filed total count in the record
INT CNmeaDecodeObject::GetFieldTotalCnt(const CHAR *pStrData)
{
    INT iOffsetCnt        = 0;    // move character
    INT iFieldTotalCnt    = 0;    // field totally count

    // Check input
    if (pStrData == NULL) {
        return 0;
    }

    // Find need field
    while (pStrData[iOffsetCnt] != '*') {
        if (pStrData[iOffsetCnt] == ',') {
            // Increase totally count
            iFieldTotalCnt++;
        }

        iOffsetCnt++;
    }

    // include field before *
    iFieldTotalCnt++;

    // Adding sum checking field
    iFieldTotalCnt++;

    return iFieldTotalCnt;
}

/*---------------------------------------------------------------------------*/
// Get field Recode
BOOL CNmeaDecodeObject::GetField(const CHAR *pStrData, INT iFieldNum, CHAR* pStrField)
{
    INT iFieldMoveCnt   = 0;    //
    INT iOffsetCnt      = 0;    //
    INT iCopyCnt        = 0;    //

    // Check input
    if (pStrData == NULL
        || iFieldNum < 0
        || pStrField == NULL) {
        return FALSE;
    }

    // Check field number with save number
    if (iFieldNum < m_iCurFieldNum) {
        // Reset save field number and offset
        m_iCurFieldNum = 0;
        m_iCurFieldOffset = 0;
    }

    // Find need field
    for (iFieldMoveCnt = 0; iFieldMoveCnt < iFieldNum - m_iCurFieldNum; iFieldMoveCnt++) {
        while (pStrData[m_iCurFieldOffset + iOffsetCnt] != ','
            && pStrData[m_iCurFieldOffset + iOffsetCnt] != '*') {
            iOffsetCnt++;
        }

        if (pStrData[m_iCurFieldOffset + iOffsetCnt] == '*') {
            // ERROR
            // Find field is overflow
            return FALSE;
        }

        // For next field
        iOffsetCnt++;
    }

    // Set current field number and offset
    m_iCurFieldNum = iFieldNum;
    m_iCurFieldOffset += iOffsetCnt;

    // Copy the data to field string
    while (pStrData[m_iCurFieldOffset + iCopyCnt] != ','
        && pStrData[m_iCurFieldOffset + iCopyCnt] != '*') {
        pStrField[iCopyCnt] = pStrData[m_iCurFieldOffset + iCopyCnt];
        iCopyCnt++;
    }

    // Add a end to the field string
    pStrField[iCopyCnt] = '\0';

    return TRUE;
}

/*---------------------------------------------------------------------------*/
// Constructor
CGGADecodeObject::CGGADecodeObject(const CHAR* pStrData)
: CNmeaDecodeObject(pStrData)
, m_dLongitude(0.0F)
, m_dLatitude(0.0F)
, m_fAltitude(0.0F)
, m_iSatellite(0)
{
    CHAR    pFieldData[NMEA_FIELD_DATA_SIZE] = {0};
    DOUBLE  ddeg        = 0.0;
    DOUBLE  dmin        = 0.0;
    CHAR    szTemp[4]   = {0};

    // Set valid flag
    m_bIsValid = TRUE;

    // Get Longitude
    // In field 2
    if (GetField(pStrData, 2, pFieldData)) {
        memcpy(szTemp, &pFieldData[0], 2);
        ddeg = atof(szTemp);
        memset(&pFieldData[0], '0', 2);
        dmin = atof(pFieldData);
        m_dLatitude = static_cast<DOUBLE>(ddeg * 3600.0 + dmin * 60.0);
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get Longitude N/S
    // In field 3
    if (GetField(pStrData, 3, pFieldData)) {
        if (pFieldData[0] != 'N') {
            if (pFieldData[0] == 'S') {
                m_dLongitude = -m_dLongitude;
            }
        }
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get Latitude
    // In field 4
    if (GetField(pStrData, 4, pFieldData)) {
        memcpy(szTemp, &pFieldData[0], 3);
        ddeg = atof(szTemp);
        memset(&pFieldData[0], '0', 3);
        dmin = atof(pFieldData);
        m_dLongitude = static_cast<DOUBLE>(ddeg * 3600.0 + dmin * 60.0);
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get Latitude E/W
    // In field 5
    if (GetField(pStrData, 5, pFieldData)) {
        if (pFieldData[0] != 'E') {
            if (pFieldData[0] == 'W') {
                m_dLatitude = -m_dLatitude;
            }
        }
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get satellite number in using
    // In field 7
    if (GetField(pStrData, 7, pFieldData)) {
        m_iSatellite = atoi(pFieldData);
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get altitude
    // In field 9
    if (GetField(pStrData, 9, pFieldData)) {
        m_fAltitude = static_cast<FLOAT>(atof(pFieldData));
    }
    else {
        m_bIsValid = FALSE;
    }
}

/*---------------------------------------------------------------------------*/
// Destructor
CGGADecodeObject::~CGGADecodeObject()
{

}

/*---------------------------------------------------------------------------*/
// Get current NMEA recode type
NmeaDecodeTypeDef CGGADecodeObject::GetNmeaType(VOID)
{
    return NMEA_DECODE_TYPE_GGA;
}

/*---------------------------------------------------------------------------*/
// Get current longitude in the GGA record
DOUBLE CGGADecodeObject::GetLongitude(VOID)
{
    return m_dLongitude;
}

/*---------------------------------------------------------------------------*/
// Get current latitude in the GGA record
DOUBLE CGGADecodeObject::GetLatitude(VOID)
{
    return m_dLatitude;
}

/*---------------------------------------------------------------------------*/
// Get current altitude in the GGA record
FLOAT CGGADecodeObject::GetAltitude(VOID)
{
    return m_fAltitude;
}

/*---------------------------------------------------------------------------*/
// Get current satellite count in GGA record
INT CGGADecodeObject::GetSatelliteCnt(VOID)
{
    return m_iSatellite;
}

/*---------------------------------------------------------------------------*/
// Constructor
CRMCDecodeObject::CRMCDecodeObject(const CHAR* pStrData)
: CNmeaDecodeObject(pStrData)
, m_fSpeed(0.0F)
, m_fAngle(0.0F)
{
    CHAR pFieldData[NMEA_FIELD_DATA_SIZE] = {0};
    CHAR szTemp[4]    = {0};

    // Initialize
    memset(&m_sTime, 0, sizeof(m_sTime));
    m_sTime.wYear = 2011;
    m_sTime.byMonth = 1;
    m_sTime.byDay = 1;

    // Set valid flag
    m_bIsValid = TRUE;

    // Get hour/minute/second/millisecond
    // In field 1
    if (GetField(pStrData, 1, pFieldData)) {
        // Hour
        memset(szTemp, 0, sizeof(szTemp));
        memcpy(szTemp, &pFieldData[0], 2);
        m_sTime.byHour = atoi(szTemp);

        // Minute
        memset(szTemp, 0, sizeof(szTemp));
        memcpy(szTemp, &pFieldData[2], 2);
        m_sTime.byMinute = atoi(szTemp);

        // Second
        memset(szTemp, 0, sizeof(szTemp));
        memcpy(szTemp, &pFieldData[4], 2);
        m_sTime.bySecond = atoi(szTemp);

        // Millisecond
        memset(szTemp, 0, sizeof(szTemp));
        memcpy(szTemp, &pFieldData[7], 3);
        m_sTime.wMilliSec = atoi(szTemp);
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get speed
    // In field 7
    if (GetField(pStrData, 7, pFieldData)) {
        // Speed from knot to km/h
        m_fSpeed = static_cast<FLOAT>(atof(pFieldData) * KNOT_TO_KILOPERHOUR);
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get direction
    // In field 8
    if (GetField(pStrData, 8, pFieldData)) {
        // Direction
        m_fAngle = static_cast<FLOAT>(atof(pFieldData));
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get year/month/day
    // In field 9
    if (GetField(pStrData, 9, pFieldData)) {
        // Day
        memset(szTemp, 0, sizeof(szTemp));
        memcpy(szTemp, &pFieldData[0], 2);
        m_sTime.byDay = atoi(szTemp);

        // Month
        memset(szTemp, 0, sizeof(szTemp));
        memcpy(szTemp, &pFieldData[2], 2);
        m_sTime.byMonth = atoi(szTemp);

        // Year
        memset(szTemp, 0, sizeof(szTemp));
        memcpy(szTemp, &pFieldData[4], 2);
        m_sTime.wYear = atoi(szTemp) + 2000;
    }
    else {
        m_bIsValid = FALSE;
    }

    // Check data number
    if (m_sTime.wYear < 1900) {
        m_sTime.wYear = 1900;
    }

    if (m_sTime.byMonth < 1 || m_sTime.byMonth > 12) {
        m_sTime.byMonth = 1;
    }

    if (m_sTime.byDay < 1 || m_sTime.byMonth > 31) {
        m_sTime.byDay = 1;
    }

    if (m_sTime.byHour < 0 || m_sTime.byHour > 23) {
        m_sTime.byHour = 0;
    }

    if (m_sTime.byMinute < 0 || m_sTime.byMinute > 59) {
        m_sTime.byMinute = 0;
    }

    if (m_sTime.bySecond < 0 || m_sTime.bySecond > 59) {
        m_sTime.bySecond = 0;
    }
}

/*---------------------------------------------------------------------------*/
// Destructor
CRMCDecodeObject::~CRMCDecodeObject()
{

}

/*---------------------------------------------------------------------------*/
// Get current NMEA recode type
NmeaDecodeTypeDef CRMCDecodeObject::GetNmeaType(VOID)
{
    return NMEA_DECODE_TYPE_RMC;
}

/*---------------------------------------------------------------------------*/
// Get current time in RMC record
CRMCDecodeObject::STime CRMCDecodeObject::GetTime(VOID)
{
    return m_sTime;
}

/*---------------------------------------------------------------------------*/
// Get current speed in RMC record
FLOAT CRMCDecodeObject::GetSpeed(VOID)
{
    return m_fSpeed;
}

/*---------------------------------------------------------------------------*/
// Get current angle in RMC recode
FLOAT CRMCDecodeObject::GetAngle(VOID)
{
    return m_fAngle;
}

/*---------------------------------------------------------------------------*/
// Constructor
CGSADecodeObject::CGSADecodeObject(const CHAR* pStrData)
: CNmeaDecodeObject(pStrData)
, m_byDimension(0)
{
    CHAR pFieldData[NMEA_FIELD_DATA_SIZE] = {0};

    // Initialize
    memset(&m_sPDOP, 0, sizeof(m_sPDOP));
    memset(m_pbySatList, 0, sizeof(BYTE) * SATELLITE_CHANNEL_MAX);

    // Set valid flag
    m_bIsValid = TRUE;

    // Get dimension
    // In field 2
    if (GetField(pStrData, 2, pFieldData)) {
        m_byDimension = atoi(pFieldData);

        // Do not dimension is 0
        if (m_byDimension == 1) {
            m_byDimension = 0;
        }
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get satellite PNG in every channel
    // In field from 3 to 14
    for (INT iFieldCnt = 3; iFieldCnt <= 14; iFieldCnt++) {
        if (GetField(pStrData, iFieldCnt, pFieldData)) {
            m_pbySatList[iFieldCnt - 3] = atoi(pFieldData);
        }
        else {
            m_bIsValid = FALSE;
        }
    }

    // Get PDOP
    // In field 15
    if (GetField(pStrData, 15, pFieldData)) {
        m_sPDOP.fPDOP = static_cast<FLOAT>(atof(pFieldData));
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get HDOP
    // In field 16
    if (GetField(pStrData, 16, pFieldData)) {
        m_sPDOP.fHDOP = static_cast<FLOAT>(atof(pFieldData));
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get VDOP
    // In field 17
    if (GetField(pStrData, 17, pFieldData)) {
        m_sPDOP.fVDOP = static_cast<FLOAT>(atof(pFieldData));
    }
    else {
        m_bIsValid = FALSE;
    }
}

/*---------------------------------------------------------------------------*/
// Destructor
CGSADecodeObject::~CGSADecodeObject()
{

}

/*---------------------------------------------------------------------------*/
// Get current NMEA recode type
NmeaDecodeTypeDef CGSADecodeObject::GetNmeaType(VOID)
{
    return NMEA_DECODE_TYPE_GSA;
}

/*---------------------------------------------------------------------------*/
// Get current dimension in GSA recode
BYTE CGSADecodeObject::GetDimension(VOID)
{
    return m_byDimension;
}

/*---------------------------------------------------------------------------*/
// Get current DOP in RMC recode
CGSADecodeObject::SDOP CGSADecodeObject::GetDOP(VOID)
{
    return m_sPDOP;
}

/*---------------------------------------------------------------------------*/
// Get satellite PRN code in different channel
INT CGSADecodeObject::GetSatelliteInChannel(INT iChannelCnt)
{
    // Check input
    if (iChannelCnt > SATELLITE_CHANNEL_MAX
        || iChannelCnt < 1) {
        return 0;
    }

    return m_pbySatList[iChannelCnt - 1];
}

/*---------------------------------------------------------------------------*/
// Constructor
CGSVDecodeObject::CGSVDecodeObject(const CHAR* pStrData)
: CNmeaDecodeObject(pStrData)
, m_iRecordTotal(0)
, m_iRecordIndex(0)
, m_iSatCnt(0)
, m_iSatInfoCnt(0)
{
    CHAR pFieldData[NMEA_FIELD_DATA_SIZE] = {0};

    // Initialize
    memset(m_sSatInfo, 0, sizeof(m_sSatInfo));

    // Set valid flag
    m_bIsValid = TRUE;

    // Get GSV record total
    // In field 1
    if (GetField(pStrData, 1, pFieldData)) {
        m_iRecordTotal = atoi(pFieldData);
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get GSV record index
    // In field 2
    if (GetField(pStrData, 2, pFieldData)) {
        m_iRecordIndex = atoi(pFieldData);
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get current satellite count
    // In field 3
    if (GetField(pStrData, 3, pFieldData)) {
        m_iSatCnt = atoi(pFieldData);
    }
    else {
        m_bIsValid = FALSE;
    }

    // Get satellite information
    // Get field totally count
    INT iFieldTotalCnt = GetFieldTotalCnt(pStrData);

    // Check filed count
    if (iFieldTotalCnt == 0
        || (iFieldTotalCnt - 5) % 4 != 0) {
        m_bIsValid = FALSE;
    }
    else {
        // In field (4 5 6 7)...
        for (INT iInfoCnt = 0; iInfoCnt < ((iFieldTotalCnt - 5) / 4); iInfoCnt++) {
            // PRN code (01 ~ 32)
            // In field 4
            if (GetField(pStrData, 4 + iInfoCnt * 4, pFieldData)) {
                m_sSatInfo[iInfoCnt].iPRGCode = atoi(pFieldData);
            }
            else {
                m_bIsValid = FALSE;
            }

            // satellite elevation (00 ~ 90) [deg]
            // In field 5
            if (GetField(pStrData, 4 + iInfoCnt * 4 + 1, pFieldData)) {
                m_sSatInfo[iInfoCnt].iElevation = atoi(pFieldData);
            }
            else {
                m_bIsValid = FALSE;
            }

            // satellite azimuth (00 ~ 359) [deg]
            // In field 6
            if (GetField(pStrData, 4 + iInfoCnt * 4 + 2, pFieldData)) {
                m_sSatInfo[iInfoCnt].iAzimuth = atoi(pFieldData);
            }
            else {
                m_bIsValid = FALSE;
            }

            // satellite signal strength (00 ~ 99) [dbHz]
            // In field 7
            if (GetField(pStrData, 4 + iInfoCnt * 4 + 3, pFieldData)) {
                m_sSatInfo[iInfoCnt].iSNR = atoi(pFieldData);
            }
            else {
                m_bIsValid = FALSE;
            }

            if (m_bIsValid) {
                m_sSatInfo[iInfoCnt].bIsValid = TRUE;
            }
        }
    }
}

/*---------------------------------------------------------------------------*/
// Destructor
CGSVDecodeObject::~CGSVDecodeObject()
{

}

/*---------------------------------------------------------------------------*/
// Get current NMEA recode type
NmeaDecodeTypeDef CGSVDecodeObject::GetNmeaType(VOID)
{
    return NMEA_DECODE_TYPE_GSV;
}

/*---------------------------------------------------------------------------*/
// Get GSV recode totally count
INT CGSVDecodeObject::GetRecodeTotal(VOID)
{
    return m_iRecordTotal;
}

/*---------------------------------------------------------------------------*/
// Get current recode index
INT CGSVDecodeObject::GetRecodeIndex(VOID)
{
    return m_iRecordIndex;
}

/*---------------------------------------------------------------------------*/
// Get satellite information in GSV record by index
CGSVDecodeObject::SGSVInfo CGSVDecodeObject::GetSatInfo(INT iIndex)
{
    // Check input
    if (iIndex < 0
        || iIndex > SATELLITE_INFO_MAX) {
        // ERROR
        SGSVInfo sTemp = {0};
        return sTemp;
    }

    return m_sSatInfo[iIndex];
}

/*---------------------------------------------------------------------------*/
/* EOF */
