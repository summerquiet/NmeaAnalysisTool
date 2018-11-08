/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_AUTOSYNC_H
#   include "Ant_AutoSync.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_AutoSync::Ant_AutoSync(Ant_SyncObj& cSync)
: m_cSync(cSync)
{
    m_cSync.syncStart();
    return;
}

Ant_AutoSync::~Ant_AutoSync()
{
    m_cSync.syncEnd();
    return;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
