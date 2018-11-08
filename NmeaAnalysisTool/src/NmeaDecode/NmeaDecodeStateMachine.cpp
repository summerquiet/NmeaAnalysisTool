/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
//  included files

#ifndef CXX_NMEADECODSTATEMACHINE_H
#   include "NmeaDecodeStateMachine.h"
#endif
#ifndef CXX_NMEAGPSSTATEMACHINE_H
#   include "NmeaGpsStateMachine.h"
#endif

/*---------------------------------------------------------------------------*/
// CNmeaDecodeStateMachine instance declare and create
CNmeaDecodeStateMachine* CNmeaDecodeStateMachine::s_pcInstance = NULL;

/*---------------------------------------------------------------------------*/
// Get the state machine interface instance
CNmeaDecodeStateMachine* CNmeaDecodeStateMachine::Instance(VOID)
{
    // TODO: ATTENTION
    // Need to Synchronized
    if (s_pcInstance == NULL) {
        s_pcInstance = new CNmeaDecodeStateMachine;
    }

    return s_pcInstance;
}

/*---------------------------------------------------------------------------*/
// Destroy the factory interface instance
VOID CNmeaDecodeStateMachine::Destroy(VOID)
{
    if (s_pcInstance != NULL) {
        // delete the instance
        delete s_pcInstance;
        s_pcInstance = NULL;
    }
}

/*---------------------------------------------------------------------------*/
// Run the state machine
BOOL CNmeaDecodeStateMachine::Run(CNmeaDecodeObject* pcNmeaObj)
{
    return m_pcNmeaGpsStateMachine->Run(pcNmeaObj);
}

/*---------------------------------------------------------------------------*/
// Check the packet is beginning or not
BOOL CNmeaDecodeStateMachine::IsPacketBegin(VOID)
{
    return m_pcNmeaGpsStateMachine->IsPacketBegin();
}

/*---------------------------------------------------------------------------*/
// Check the packet is ending or not
BOOL CNmeaDecodeStateMachine::IsPacketEnd(VOID)
{
    return m_pcNmeaGpsStateMachine->IsPacketEnd();
}

/*---------------------------------------------------------------------------*/
// Clear the state to NONE
VOID CNmeaDecodeStateMachine::ClearState(VOID)
{
    // Clear
    m_pcNmeaGpsStateMachine->ClearState();
}

/*---------------------------------------------------------------------------*/
// Constructor
CNmeaDecodeStateMachine::CNmeaDecodeStateMachine()
: m_pcNmeaGpsStateMachine(NULL)
{
    m_pcNmeaGpsStateMachine = new CNmeaGpsStateMachine();

    // Initialize the GPS state machine
    m_pcNmeaGpsStateMachine->Initialize();

    // Start the GPS state machine
    m_pcNmeaGpsStateMachine->Start();
}

/*---------------------------------------------------------------------------*/
// Destructor
CNmeaDecodeStateMachine::~CNmeaDecodeStateMachine()
{
    // Stop the GPS state machine
    m_pcNmeaGpsStateMachine->Stop();
}

/*---------------------------------------------------------------------------*/
/* EOF */
