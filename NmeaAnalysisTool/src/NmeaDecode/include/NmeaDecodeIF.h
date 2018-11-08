/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#ifndef CXX_NMEADECODEIF_H
#define CXX_NMEADECODEIF_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
//  included files

#ifndef CXX_NMEADECODEDEFINE_H
#   include "NmeaDecodeDefine.h"
#endif

/*---------------------------------------------------------------------------*/
// Class define

class CNmeaDecode;

/*
 * CNmeaDeocdeIF class
 *
 * Interface of NMEA information decode
 */
class CNmeaDecodeIF
{
public:

    /*
     * Get instance of the decode process
     *
     * @param  NONE
     *
     * @return CNmeaDecodeIF* an abstract pointer
     */
    static CNmeaDecodeIF* Instance(VOID);

    /*
     * Destroy all the decode process interface instance
     *
     * @param  NONE
     *
     * @return NONE
     */
    static VOID Destroy(VOID);

    /*
     * Receive NMEA information
     *
     * @param   eRecSrc     [IN]: input NMEA receive source
     *          pData       [IN]: input NMEA information
     *          dwSize      [IN]: input data size
     *
     * @return BOOL : Decode result
     */
    BOOL ReceiveNmeaInfo(NmeaReceiveSource eRecSrc, const VOID* pData, DWORD dwSize);

    /*
     * Get GPS data
     *
     * @param   rsGpsData   [OUT]:
     *
     * @return  BOOL        : can get a total packet or not
     */
    BOOL GetGpsData(GPS_DATA& rsGpsData);

private:

    static CNmeaDecodeIF*   m_pcInstance;
    static CNmeaDecode*     m_pcImplement;

private:

    /*
     * Constructor
     *
     * @param  NONE
     *
     * @return NONE
     */
    CNmeaDecodeIF() {}

    /*
     * Destructor
     *
     * @param  NONE
     *
     * @return NONE
     */
    virtual ~CNmeaDecodeIF() {}
};

/*---------------------------------------------------------------------------*/
#endif  // CXX_NMEADECODEIF_H

/*---------------------------------------------------------------------------*/
/* EOF */
