/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#ifndef CXX_NMEADECODEPROCESS_H
#define CXX_NMEADECODEPROCESS_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
//  included files

#ifndef CXX_NMEADECODEFACTORY_H
#   include "NmeaDecodeFactory.h"
#endif
#ifndef CXX_NMEADECODSTATEMACHINE_H
#   include "NmeaDecodeStateMachine.h"
#endif
#ifndef CXX_NMEADECODEOBJECTLIST_H
#   include "NmeaDecodeObjectList.h"
#endif

/*---------------------------------------------------------------------------*/
// Class define

class CNmeaDecode;

/**
* CNmeaDecodeProcess class
*
* NMEA recode decode process
*
*/
class CNmeaDecodeProcess
{
public:

    /**
     * Constructor
     *
     * @param  NONE
     *
     * @return NONE
     */
    CNmeaDecodeProcess(CNmeaDecode* pcDecode);

    /**
     * Destructor
     *
     * @param  NONE
     *
     * @return NONE
     */
    virtual ~CNmeaDecodeProcess();

    /**
     * Decode NMEA recode in one line
     *
     * @param   pStrData    [IN]: input NMEA recode
     *
     * @return  BOOL        : Decode result
     */
    BOOL DecodeNmeaRecode(const CHAR* pStrData);

    /**
     * Decode NMEA recode in one line
     *
     * @param   rsGpsData   [OUT]: GPS data
     *
     * @return  BOOL        : can get GPS data of a complete packet
     */
    BOOL GetGpsData(GPS_DATA &rsGpsData);
    
private:

    CNmeaDecode*                m_pcDecode;         // decode parent pointer

    CNmeaDecodeFactory*         m_pcFactory;        // pointer to the NMEA recode factory
    CNmeaDecodeStateMachine*    m_pcStatMachine;    // pointer to the state machine
    CNmeaDecodeObjectList       m_cObjectList;      // NMEA recode object list to save

    // Data output
    BOOL                        m_bIsCompletePakct; // current is a complete packet or not
    GPS_DATA                    m_sGpsData;         // current packet GPS data

    // Debug
    DWORD                       m_dwCounter;        // for debug counter

    /**
     * Check sum of the data in NMEA recode in one line
     *
     * @param   pStrData    [IN]: input NMEA recode
     *
     * @return  BOOL        : check sum result 
     */
    BOOL CheckSumNmeaRecode(const CHAR* pStrData);

    /**
     * Install the GPS data by object
     *
     * @param   rsGpsData   [OUT]: the installation result
     *
     * @return  BOOL        : install process successful or not
     */
    BOOL InstallGpsData(GPS_DATA& rsGpsData);
};

/*---------------------------------------------------------------------------*/
#endif  // end of CXX_NMEADECODEPROCESS_H

/*---------------------------------------------------------------------------*/
/* EOF */
