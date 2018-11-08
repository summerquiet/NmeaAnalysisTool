/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef CXX_NMEADECODEPROCESS_H
#   include "NmeaDecodeProcess.h"
#endif
#ifndef CXX_NMEADECODE_H
#   include "NmeaDecode.h"
#endif

/*---------------------------------------------------------------------------*/
// Constructor
CNmeaDecodeProcess::CNmeaDecodeProcess(CNmeaDecode* pcDecode)
: m_pcDecode(pcDecode)
, m_pcFactory(NULL)
, m_pcStatMachine(NULL)
, m_cObjectList()
, m_bIsCompletePakct(FALSE)
, m_dwCounter(0)
{
    m_pcFactory = CNmeaDecodeFactory::Instance();
    m_pcStatMachine = CNmeaDecodeStateMachine::Instance();

    memset(&m_sGpsData, 0, sizeof(m_sGpsData));
}

/*---------------------------------------------------------------------------*/
// Destructor
CNmeaDecodeProcess::~CNmeaDecodeProcess()
{
    // Release all object in the object list
    CNmeaDecodeObject* pNmeaObject = NULL;

    while (m_cObjectList.Pop(pNmeaObject) == TRUE) {
        m_pcFactory->DestroyNmeaDecodeObejct(pNmeaObject);
    }

    // Release the factory instance
    CNmeaDecodeFactory::Destroy();
    m_pcFactory = NULL;

    // Release the state machine instance
    CNmeaDecodeStateMachine::Destroy();
    m_pcStatMachine = NULL;
}

/*---------------------------------------------------------------------------*/
// Decode NMEA recode in one line
BOOL CNmeaDecodeProcess::DecodeNmeaRecode(const CHAR* pStrData)
{
    // Check input
    if (pStrData == NULL) {
        return FALSE;
    }

    // Adding counter
    m_dwCounter++;

    // Check the NMEA recode
    if (!CheckSumNmeaRecode(pStrData)) {
        return FALSE;
    }

    // Create current NMEA recode object
    CNmeaDecodeObject* pcNmeaObj = m_pcFactory->CreateNmeaDecodeObejct(pStrData);

    // Check pointer
    if (pcNmeaObj == NULL) {
        return FALSE;
    }

    // Check decode result
    if (!pcNmeaObj->IsValid()) {
        // Release the object
        m_pcFactory->DestroyNmeaDecodeObejct(pcNmeaObj);

        // Return
        return FALSE;
    }

    // State machine run to next state
    if (m_pcStatMachine->Run(pcNmeaObj)) {
        // Check packet begin
        if (m_pcStatMachine->IsPacketBegin()) {
            // Clear all the object in the list
            CNmeaDecodeObject* pcClearNmeaObj = NULL;
            while (m_cObjectList.Pop(pcClearNmeaObj)) {
                // Release the object
                m_pcFactory->DestroyNmeaDecodeObejct(pcClearNmeaObj);
                pcClearNmeaObj = NULL;
            }

            // Clear OLD GPS_DATA
            memset(&m_sGpsData, 0, sizeof(GPS_DATA));

            // Reset the flag
            m_bIsCompletePakct = FALSE;
        }

        // Push current to the object list
        // To wait a complete packet to install a GPS data
        m_cObjectList.Push(pcNmeaObj);

        // Check packet end
        if (m_pcStatMachine->IsPacketEnd()) {
            GPS_DATA sTempData;
            memset(&sTempData, 0, sizeof(GPS_DATA));

            // Install the GPS data
            if (InstallGpsData(sTempData)) {

                // Get a complete packet
                m_bIsCompletePakct = TRUE;
                m_sGpsData = sTempData;

                // Update GPS data
                m_pcDecode->UpdateGpsData(sTempData);

                // Clear state of state machine to NONE
                m_pcStatMachine->ClearState();
            }
        }
    }
    else {
        // Release the object
        m_pcFactory->DestroyNmeaDecodeObejct(pcNmeaObj);
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*/
// Get GPS data of a complete packet
BOOL CNmeaDecodeProcess::GetGpsData(GPS_DATA& rsGpsData)
{
    BOOL bRet = FALSE;

    // Check get a complete packet
    if (m_bIsCompletePakct) {
        rsGpsData = m_sGpsData;
        bRet = TRUE;
    }

    // Clear flag
    m_bIsCompletePakct = FALSE;

    return bRet;
}

/*---------------------------------------------------------------------------*/
// Check sum of the data in NMEA recode in one line
BOOL CNmeaDecodeProcess::CheckSumNmeaRecode(const CHAR* pStrData)
{
    INT     iLength     = 0;        // string data length
    INT     iCount      = 0;        // count for cycle
    BYTE    chksum      = 0x00;     // check sum
    CHAR    chkCap[3]   = {0};      // check sum buffer capital letter
    CHAR    chkLow[3]   = {0};      // check sum buffer lower case letter

    // Check input
    if (pStrData == NULL) {
        return FALSE;
    }

    // Get the string length
    iLength = static_cast<INT>(strlen(pStrData));

    // Get the check sum in the string data (without the '$' at begin)
    for (iCount = 1; iCount < iLength; iCount++) {
        if (pStrData[iCount] == '*') {
            if ((iCount + 4) <= iLength) {
                // Check sum
                sprintf(chkCap, "%02X", chksum);
                sprintf(chkLow, "%02x", chksum);

                if ((pStrData[iCount + 1] == chkCap[0] && pStrData[iCount + 2] == chkCap[1])
                    || (pStrData[iCount + 1] == chkLow[0] && pStrData[iCount + 2] == chkLow[1])) {
                    return TRUE;
                }
                else {
                    return FALSE;
                }
            }
            else {
                return FALSE;
            }
        }

        // Check sum
        chksum = chksum ^ (BYTE)pStrData[iCount];
    }

    // ERROR
    return FALSE;
}

/*---------------------------------------------------------------------------*/
// Install the GPS data by object
BOOL CNmeaDecodeProcess::InstallGpsData(GPS_DATA& rsGpsData)
{
    CNmeaDecodeObject* pNmeaObject = NULL;

    // Check the object list
    if (m_cObjectList.IsEmpty()) {
        return FALSE;
    }

    BYTE byUsedSatelliteID[CGSADecodeObject::SATELLITE_CHANNEL_MAX];
    memset(&byUsedSatelliteID, 0, sizeof(byUsedSatelliteID));
    BOOL bUsedSatelliteListValid = FALSE;

    // Get object for the list
    while (m_cObjectList.Pop(pNmeaObject)) {
        // Makeup the GPS data
        switch (pNmeaObject->GetNmeaType()) {
        case NMEA_DECODE_TYPE_NONE:
            // Nothing to do
            break;
        case NMEA_DECODE_TYPE_GGA:
            {
                // Change to GGA object
                CGGADecodeObject *pcGGAObj = static_cast<CGGADecodeObject *>(pNmeaObject);

                // Get longitude
                rsGpsData.dLongitude = pcGGAObj->GetLongitude() / 3600.0;

                // Get latitude
                rsGpsData.dLatitude = pcGGAObj->GetLatitude() / 3600.0;

                // Get altitude
                rsGpsData.fAltitude = pcGGAObj->GetAltitude();

                // Get satellite
                rsGpsData.iSatelliteCnt = pcGGAObj->GetSatelliteCnt();
            }
            break;
        case NMEA_DECODE_TYPE_RMC:
            {
                // Change to RMC object
                CRMCDecodeObject *pcGGAObj = static_cast<CRMCDecodeObject *>(pNmeaObject);

                // Get time
                CRMCDecodeObject::STime sTime = pcGGAObj->GetTime();

                // UTC time
                rsGpsData.sUtcTime.wYear        = sTime.wYear;
                rsGpsData.sUtcTime.wMonth       = sTime.byMonth;
                rsGpsData.sUtcTime.wDay         = sTime.byDay;
                rsGpsData.sUtcTime.wHour        = sTime.byHour;
                rsGpsData.sUtcTime.wMinute      = sTime.byMinute;
                rsGpsData.sUtcTime.wSecond      = sTime.bySecond;
                rsGpsData.sUtcTime.wMilliseconds = sTime.wMilliSec;

                // Get velocity
                rsGpsData.fVelocity = pcGGAObj->GetSpeed();

                // Get direction
                rsGpsData.fDirection = pcGGAObj->GetAngle();
            }
            break;
        case NMEA_DECODE_TYPE_GSA:
            {
                // Change to GSA object
                CGSADecodeObject *pcGGAObj = static_cast<CGSADecodeObject *>(pNmeaObject);

                // Get dimension
                rsGpsData.iDimension = pcGGAObj->GetDimension();

                // Get satellite ID
                for (INT iChannelCnt = 0; iChannelCnt < CGSADecodeObject::SATELLITE_CHANNEL_MAX; iChannelCnt++) {
                    INT iPRNCode = pcGGAObj->GetSatelliteInChannel(iChannelCnt + 1);

                    if (iPRNCode != 0) {
                        byUsedSatelliteID[iChannelCnt] = iPRNCode;
                    }
                }
                bUsedSatelliteListValid = TRUE;

                // Get PDOP
                rsGpsData.fPdop = pcGGAObj->GetDOP().fPDOP;

                // Get HDOP
                rsGpsData.fHdop = pcGGAObj->GetDOP().fHDOP;

                // Get VDOP
                rsGpsData.fVdop = pcGGAObj->GetDOP().fVDOP;
            }
            break;
        case NMEA_DECODE_TYPE_GSV:
            {
                // Change to GSV object
                CGSVDecodeObject *pcGGAObj = static_cast<CGSVDecodeObject *>(pNmeaObject);

                // Get satellite information
                // Get can use buffer
                INT iSatCnt = 0;
                for (iSatCnt = 0; iSatCnt < MAX_GPS_SATELLITE; iSatCnt++) {
                    if (rsGpsData.sSatelliteInfo[iSatCnt].bValid != 1) {
                        break;
                    }
                }

                if (iSatCnt == MAX_GPS_SATELLITE) {
                    // ERROR
                }
                else {
                    INT iValidInfoCnt = 0;

                    // Get information
                    for (INT iIndex = 0; iIndex < CGSVDecodeObject::SATELLITE_INFO_MAX; iIndex++) {
                        CGSVDecodeObject::SGSVInfo sSatInfo = pcGGAObj->GetSatInfo(iIndex);

                        // Check valid flag
                        if (sSatInfo.bIsValid) {
                            // Fix struct
                            rsGpsData.sSatelliteInfo[iSatCnt + iValidInfoCnt].bValid = TRUE;
                            rsGpsData.sSatelliteInfo[iSatCnt + iValidInfoCnt].bySatelliteID = static_cast<BYTE>(sSatInfo.iPRGCode);
                            rsGpsData.sSatelliteInfo[iSatCnt + iValidInfoCnt].fElevation = static_cast<FLOAT>(sSatInfo.iElevation);
                            rsGpsData.sSatelliteInfo[iSatCnt + iValidInfoCnt].fAzimuth = static_cast<FLOAT>(sSatInfo.iAzimuth);
                            rsGpsData.sSatelliteInfo[iSatCnt + iValidInfoCnt].fSNR = static_cast<FLOAT>(sSatInfo.iSNR);

                            if (bUsedSatelliteListValid) {
                                for (INT i = 0; i < CGSADecodeObject::SATELLITE_CHANNEL_MAX; i++) {
                                    if (byUsedSatelliteID[i] == rsGpsData.sSatelliteInfo[iSatCnt + iValidInfoCnt].bySatelliteID) {
                                        rsGpsData.sSatelliteInfo[iSatCnt + iValidInfoCnt].bUsed = TRUE;     // Set to used
                                        break;
                                    }
                                }
                            }

                            // Increase valid information count
                            iValidInfoCnt++;
                        }
                    }
                }
            }
            break;
        default:
            break;
        }

        // Release the object
        m_pcFactory->DestroyNmeaDecodeObejct(pNmeaObject);
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*/
/* EOF */
