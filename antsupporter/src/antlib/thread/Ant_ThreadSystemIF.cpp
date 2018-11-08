/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_THREADSYSTEMIF_H
#   include "Ant_ThreadSystemIF.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_ThreadSystemIF* g_pcAntThreadSystem = NULL;

VOID Ant_SetThreadSystem(Ant_ThreadSystemIF* sp)
{
    g_pcAntThreadSystem = sp;
}

Ant_ThreadSystemIF* Ant_GetThreadSystem()
{
    return g_pcAntThreadSystem;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
