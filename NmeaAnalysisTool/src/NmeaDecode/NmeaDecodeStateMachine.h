/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#ifndef CXX_NMEADECODSTATEMACHINE_H
#define CXX_NMEADECODSTATEMACHINE_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
//  included files

#ifndef CXX_NMEADECODEOBJECT_H
#   include "NmeaDecodeObject.h"
#endif

/*---------------------------------------------------------------------------*/
// Class define

class CNmeaGpsStateMachine;

/**
 * CNmeaDecodeStateMachine class
 *
 * NMEA decode process state machine implement class
 */
class CNmeaDecodeStateMachine
{
public:

    /**
     * Get the factory instance
     *
     * @param   NONE
     *
     * @return  CNmeaDecodeFactory* an abstract pointer
     */
    static CNmeaDecodeStateMachine* Instance(VOID);

    /**
     * Destroy the factory instance
     *
     * @param  NONE
     *
     * @return NONE
     */
    static VOID Destroy(VOID);

    /**
     * Run the state machine
     *
     * @param   pcNmeaObj   [IN]: current get NMEA recode
     *
     * @return  running right or not
     */
    BOOL Run(CNmeaDecodeObject* pcNmeaObj);

    /**
     * Check the packet is beginning or not
     *
     * @param   NONE
     *
     * @return  check result
     */
    BOOL IsPacketBegin(VOID);

    /**
     * Check the packet is ending or not
     *
     * @param   NONE
     *
     * @return  check result
     */
    BOOL IsPacketEnd(VOID);

    /**
     * Clear the state to NONE
     *
     * @param   NONE
     *
     * @return  NONE
     */
    VOID ClearState(VOID);

protected:

    /**
     * Constructor
     *
     * @param   NONE
     *
     * @return  NONE
     */
    CNmeaDecodeStateMachine();

    /**
     * Destructor
     *
     * @param   NONE
     *
     * @return  NONE
     */
    virtual ~CNmeaDecodeStateMachine();

private:

    static CNmeaDecodeStateMachine*     s_pcInstance;               // static member to recode instance pointer

    CNmeaGpsStateMachine*               m_pcNmeaGpsStateMachine;    // NMEA recode GPS signal state machine
};

/*---------------------------------------------------------------------------*/
#endif  // CXX_NMEADECODSTATEMACHINE_H

/*---------------------------------------------------------------------------*/
/* EOF */
