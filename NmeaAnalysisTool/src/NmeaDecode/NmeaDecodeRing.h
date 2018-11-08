/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#ifndef CXX_NMEADECODERING_H
#define CXX_NMEADECODERING_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
//  Included files

/*---------------------------------------------------------------------------*/
// Class define

class CNmeaDecode;

/**
 * CNmeaDecodeRing class
 *
 * NMEA decode ring buffer
 */
class CNmeaDecodeRing
{
public:

    /**
     * Constructor
     *
     * @param  NONE
     *
     * @return NONE
     */
    CNmeaDecodeRing(CNmeaDecode* pcDecode);

    /**
     * Destructor
     *
     * @param  NONE
     *
     * @return NONE
     */
    virtual ~CNmeaDecodeRing();

    /**
     * Receive NMEA information
     *
     * @param   pData       [IN]: input NMEA information
     *          dwSize      [IN]: input data size
     *
     * @return  BOOL        : Decode result
     */
    BOOL ReceiveNmeaInfo(const VOID* pData, DWORD dwSize);

private:

    static const DWORD NMEA_DECODE_SEARCH_BUF_SIZE = 2048;                  // NMEA decode search buffer size
    static const DWORD NMEA_DECODE_SEND_BUF_SIZE = 256 + 1;                 // NMEA decode send buffer size

    CNmeaDecode*        m_pcDecode;                                         // decode parent pointer
    CHAR                m_pSearchBuffer[NMEA_DECODE_SEARCH_BUF_SIZE];       // NMEA decode search buffer
    DWORD               m_dwUsingBufSize;                                   // using buffer size
    CHAR                m_pSendBuffer[NMEA_DECODE_SEND_BUF_SIZE];           // NMEA decode send buffer
};

/*---------------------------------------------------------------------------*/
#endif  // CXX_NMEADECODERING_H

/*---------------------------------------------------------------------------*/
/* EOF */
