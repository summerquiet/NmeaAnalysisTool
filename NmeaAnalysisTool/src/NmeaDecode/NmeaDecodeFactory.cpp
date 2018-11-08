/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef CXX_NMEADECODEFACTORY_H
#    include "NmeaDecodeFactory.h"
#endif
#ifndef CXX_NMEADECODEOBJECT_H
#   include "NmeaDecodeObject.h"
#endif

/*---------------------------------------------------------------------------*/
// Get the factory interface instance
CNmeaDecodeFactory* CNmeaDecodeFactory::Instance(VOID)
{
    return CNmeaDecodeConcreteFactory::Instance();
}

/*---------------------------------------------------------------------------*/
// Destroy the factory interface instance
VOID CNmeaDecodeFactory::Destroy(VOID)
{
    CNmeaDecodeConcreteFactory::Destroy();
}

/*---------------------------------------------------------------------------*/
// Constructor
CNmeaDecodeFactory::CNmeaDecodeFactory()
: m_iObjectCount(0)
{
    // Nothing to do
}

/*---------------------------------------------------------------------------*/
// Destructor
CNmeaDecodeFactory::~CNmeaDecodeFactory()
{
    // Nothing to do
}

/*---------------------------------------------------------------------------*/
// Get the NMEA recode object
CNmeaDecodeObject* CNmeaDecodeConcreteFactory::CreateNmeaDecodeObejct(const CHAR *pStrData)
{
    CNmeaDecodeObject* pcTemp = NULL;

    // Create different object by check input
    if (strstr(pStrData, "GGA") != NULL) {
        pcTemp = new CGGADecodeObject(pStrData);
    }
    else if (strstr(pStrData, "RMC") != NULL) {
        pcTemp = new CRMCDecodeObject(pStrData);
    }
    else if (strstr(pStrData, "GPGSA") != NULL) {
        pcTemp = new CGSADecodeObject(pStrData);
    }
    else if (strstr(pStrData, "GPGSV") != NULL) {
        pcTemp = new CGSVDecodeObject(pStrData);
    }
    else {
        // Nothing to do
    }

    // Create object successful increase the counter
    if (pcTemp != NULL) {
        m_iObjectCount++;
    }

    return pcTemp;
}

/*---------------------------------------------------------------------------*/
// Destroy the NMEA recode object
VOID CNmeaDecodeConcreteFactory::DestroyNmeaDecodeObejct(CNmeaDecodeObject* pcNmeaDecodeObject)
{
    // Check input
    if (pcNmeaDecodeObject != NULL) {
        // Release the space
        delete pcNmeaDecodeObject;
        pcNmeaDecodeObject = NULL;

        // decrease the counter
        m_iObjectCount--;
    }
}

/*---------------------------------------------------------------------------*/

// CGpsSnsNmeaFactory instance declare and create
CNmeaDecodeConcreteFactory* CNmeaDecodeConcreteFactory::s_pcInstance = NULL;

/*---------------------------------------------------------------------------*/
// Get the factory interface instance
CNmeaDecodeConcreteFactory* CNmeaDecodeConcreteFactory::Instance(VOID)
{
    // TODO: ATTENTION
    // Need to Synchronized

    if (s_pcInstance == NULL) {
        s_pcInstance = new CNmeaDecodeConcreteFactory;
    }

    return s_pcInstance;
}

/*---------------------------------------------------------------------------*/
// Destroy the factory interface instance
VOID CNmeaDecodeConcreteFactory::Destroy(VOID)
{
    if (s_pcInstance != NULL) {
        // delete the instance
        delete s_pcInstance;
        s_pcInstance = NULL;
    }
}

/*---------------------------------------------------------------------------*/
// Constructor
CNmeaDecodeConcreteFactory::CNmeaDecodeConcreteFactory()
: CNmeaDecodeFactory()
{
    // Nothing to do
}

/*---------------------------------------------------------------------------*/
// Destructor
CNmeaDecodeConcreteFactory::~CNmeaDecodeConcreteFactory()
{
    // Nothing to do
}

/*---------------------------------------------------------------------------*/
/* EOF */
