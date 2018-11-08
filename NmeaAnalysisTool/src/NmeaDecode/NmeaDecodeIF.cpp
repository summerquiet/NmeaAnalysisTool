/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
//  Included files

#ifndef CXX_NMEADECODEIF_H
#   include "NmeaDecodeIF.h"
#endif
#ifndef CXX_NMEADECODE_H
#   include "NmeaDecode.h"
#endif

/*---------------------------------------------------------------------------*/
// Static members declare

CNmeaDecodeIF* CNmeaDecodeIF::m_pcInstance = NULL;
CNmeaDecode* CNmeaDecodeIF::m_pcImplement = NULL;

/*---------------------------------------------------------------------------*/
// Get instance of the decode process
CNmeaDecodeIF* CNmeaDecodeIF::Instance(VOID)
{
    if (m_pcInstance == NULL) {
        m_pcInstance = new CNmeaDecodeIF();
        m_pcImplement = new CNmeaDecode();
    }

    return m_pcInstance;
}

/*---------------------------------------------------------------------------*/
// Destroy all the decode process interface instance
VOID CNmeaDecodeIF::Destroy(VOID)
{
    if (m_pcInstance != NULL) {
        delete m_pcInstance;
        m_pcInstance = NULL;
        delete m_pcImplement;
        m_pcImplement = NULL;
    }
}

/*---------------------------------------------------------------------------*/
// Receive NMEA information
BOOL CNmeaDecodeIF::ReceiveNmeaInfo(NmeaReceiveSource eRecSrc, const VOID* pData, DWORD dwSize)
{
    return m_pcImplement->ReceiveNmeaInfo(eRecSrc, pData, dwSize);
}

/*---------------------------------------------------------------------------*/
// Get GPS data
BOOL CNmeaDecodeIF::GetGpsData(GPS_DATA& rsGpsData)
{
    return m_pcImplement->GetGpsData(rsGpsData);
}

/*---------------------------------------------------------------------------*/
/* EOF */
