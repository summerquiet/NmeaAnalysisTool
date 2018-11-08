/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_AUTOPROCSYNC_H
#   include "Ant_AutoProcSync.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
void Ant_ProcSyncObj::del(const char * name)
{
#if defined(_WIN32) || defined(_WIN32_WCE)
#else
    sem_unlink(name);
#endif
}

void Ant_ProcSyncObj::del()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
#else
    sem_unlink(m_semname);
#endif
}

Ant_ProcSyncObj::Ant_ProcSyncObj(const char* name, int value)
{
#if defined(_WIN32) || defined(_WIN32_WCE)
#else
    strcpy(m_semname, name);
    do {
        m_psem = sem_open(name, O_RDWR|O_CREAT, 00777, value);
    } while ((m_psem == SEM_FAILED) && (errno == EINTR));
#endif
}

Ant_ProcSyncObj::~Ant_ProcSyncObj()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
#else
    sem_unlink(m_semname);
    sem_close(m_psem);
#endif
}

bool Ant_ProcSyncObj::sync_p()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    return false;
#else
    int rc = 0;
    do {
        rc = sem_wait(m_psem);
    } while ((rc) && (errno == EINTR));
    return (0 == rc)?true:false;
#endif
}

bool Ant_ProcSyncObj::sync_v()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    return false;
#else
    return (0 == sem_post(m_psem))?true:false;
#endif
}

Ant_AutoProcSync::Ant_AutoProcSync(Ant_ProcSyncObj& syncobj)
: m_syncobj(syncobj)
{
    m_syncobj.sync_p();
}

Ant_AutoProcSync::~Ant_AutoProcSync()
{
    m_syncobj.sync_v();
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
