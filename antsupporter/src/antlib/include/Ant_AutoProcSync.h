/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_AUTOPROCSYNC_H
#define ANT_AUTOPROCSYNC_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_OBJECT_H
#   include "Ant_Object.h"
#endif

#if defined(_WIN32) || defined(_WIN32_WCE)
#else
#include <semaphore.h> // semaphore
#include <fcntl.h>
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

const char ANT_PROCSYNCID_PROCSHAREDMEMORY[] = "ANT_PROCSYNCID_PROCSHAREDMEMORY";

/*---------------------------------------------------------------------------*/
// Class declare

/**
 * @brief proc sync object
 *
 * @detail do synchronization between processes.
 */
class ANTLIB_API Ant_ProcSyncObj : public virtual Ant_Object
{
public:

    Ant_ProcSyncObj(const char * name = NULL, int value = 1);
    ~Ant_ProcSyncObj();

    bool sync_p();
    bool sync_v();

    void del();

public:

    static void del(const char * name);

protected:

#if defined(_WIN32) || defined(_WIN32_WCE)
#else
    sem_t*  m_psem;
    char    m_semname[260];
#endif

private:

    // Disable the copy constructor and operator =
    Ant_ProcSyncObj(const Ant_ProcSyncObj& src);
    Ant_ProcSyncObj& operator = (const Ant_ProcSyncObj& src);

};

/**
 * @brief helper class over Ant_ProcSync
 *
 * @detail helper class to automatically gain/release process-wide lock.
 */
class ANTLIB_API Ant_AutoProcSync : public virtual Ant_Object
{
public:

    Ant_AutoProcSync(Ant_ProcSyncObj& syncobj);
    ~Ant_AutoProcSync();

private:

    Ant_ProcSyncObj& m_syncobj;

private:

    // Disable the copy constructor and operator =
    Ant_AutoProcSync(const Ant_AutoProcSync& cSync);
    Ant_AutoProcSync& operator=(const Ant_AutoProcSync& cSync);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_AUTOPROCSYNC_H

/*---------------------------------------------------------------------------*/
/* EOF */
