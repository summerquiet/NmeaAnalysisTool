/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#ifndef CXX_NMEADECODE_H
#define CXX_NMEADECODE_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
//  Included files

#ifndef CXX_NMEADECODEDEFINE_H
#   include "NmeaDecodeDefine.h"
#endif
#ifndef CXX_NMEADECODERING_H
#   include "NmeaDecodeRing.h"
#endif
#ifndef CXX_NMEADECODEPROCESS_H
#   include "NmeaDecodeProcess.h"
#endif

/*---------------------------------------------------------------------------*/
// Class define
/*
 * CNmeaDeocde class
 *
 * Implement of NMEA information decode
*/

class CNmeaDecode
{
public:

    /**
     * Constructor
     *
     * @param  NONE
     *
     * @return NONE
     */
    CNmeaDecode();

    /**
     * Destructor
     *
     * @param  NONE
     *
     * @return NONE
     */
    virtual ~CNmeaDecode();

    /**
     * Receive NMEA information
     *
     * @param   eRecSrc     [IN]: input NMEA receive source
     *          pData       [IN]: input NMEA information
     *          dwSize      [IN]: input data size
     *
     * @return  BOOL : Decode result
     */
    BOOL ReceiveNmeaInfo(NmeaReceiveSource eRecSrc, const VOID* pData, DWORD dwSize);

    /**
     * Get GPS data
     *
     * @param   &rsGpsData  [OUT]:
     *
     * @return  BOOL        : can get a total packet or not
     */
    BOOL GetGpsData(GPS_DATA& rsGpsData);

    /**
     * Decode NMEA recode in one line
     *
     * @param   pStrData    [IN]: input NMEA recode
     *
     * @return  BOOL        : Decode result
     */
    BOOL DecodeNmeaRecode(const CHAR* pStrData);

    /**
     * Update GPS data
     *
     * @param   rsGpsData   [IN]: a total packet of GPS data
     *
     * @return  NONE
     */
    VOID UpdateGpsData(const GPS_DATA &rsGpsData);

    /**
     * Get signing full packet flag
     *
     * @param   NONE
     *
     * @return  BOOL        : Yes/No
     */
    BOOL GetFullPacketFlag(VOID) {return m_bFullPacketFlag;}

    /**
     * Reset the full packet flag to invalid
     *
     * @param   NONE
     *
     * @return  NONE
     */
    VOID ResetFullPacketFlag(VOID) {m_bFullPacketFlag = FALSE;}

private:

    // All members
    CNmeaDecodeRing     m_cRingBuffer;                  // NMEA decode ring buffer
    CNmeaDecodeProcess  m_cMainProcess;                 // NMEA decode main process
    NmeaReceiveSource   m_eRecSrc;                      // NMEA information receive source
    BOOL                m_bFullPacketFlag;              // a flag to sign got a full packet of GPS data
};

/*---------------------------------------------------------------------------*/
#endif  // CXX_NMEADECODE_H

/*---------------------------------------------------------------------------*/
/* EOF */
