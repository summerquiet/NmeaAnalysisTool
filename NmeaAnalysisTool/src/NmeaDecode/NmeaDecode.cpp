/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
//  Included files

#ifndef CXX_NMEADECODE_H
#   include "NmeaDecode.h"
#endif

/*---------------------------------------------------------------------------*/
// Constructor
CNmeaDecode::CNmeaDecode()
: m_cRingBuffer(this)
, m_cMainProcess(this)
, m_eRecSrc(NMEA_RECEIVE_SOURCE_NONE)
, m_bFullPacketFlag(FALSE)
{

}

/*---------------------------------------------------------------------------*/
// Destructor
CNmeaDecode::~CNmeaDecode()
{

}

/*---------------------------------------------------------------------------*/
// Receive NMEA information
BOOL CNmeaDecode::ReceiveNmeaInfo(NmeaReceiveSource eRecSrc, const VOID* pData, DWORD dwSize)
{
    // Check input
    if (eRecSrc == NMEA_RECEIVE_SOURCE_NONE
        || pData == NULL
        || dwSize == 0) {
        return FALSE;
    }

    m_eRecSrc = eRecSrc;
    return m_cRingBuffer.ReceiveNmeaInfo(pData, dwSize);
}

/*---------------------------------------------------------------------------*/
// Get GPS data
BOOL CNmeaDecode::GetGpsData(GPS_DATA& rsGpsData)
{
    return m_cMainProcess.GetGpsData(rsGpsData);
}

/*---------------------------------------------------------------------------*/
// Decode NMEA recode in one line
BOOL CNmeaDecode::DecodeNmeaRecode(const CHAR* pStrData)
{
    return m_cMainProcess.DecodeNmeaRecode(pStrData);
}

/*---------------------------------------------------------------------------*/
// Update GPS data
VOID CNmeaDecode::UpdateGpsData(const GPS_DATA& rsGpsData)
{
    // Update to outside
    // TODO:

    m_bFullPacketFlag = TRUE;
}

/*---------------------------------------------------------------------------*/
/* EOF */
