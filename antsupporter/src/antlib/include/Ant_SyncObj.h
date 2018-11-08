/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_SYNCOBJ_H
#define ANT_SYNCOBJ_H
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
#if !defined(_WIN32) && !defined(_WIN32_WCE)
#   include <pthread.h>
#endif //_LINUX

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class define

/**
 *  class Ant_SyncObj
 *
 * The class of Synchronizing object
 *
 */
class ANTLIB_API Ant_SyncObj : public virtual Ant_Object
{
public:

    /**
     * Construction.
     */
    Ant_SyncObj();

    /**
     * Destruction.
     */
    virtual ~Ant_SyncObj();

    /**
     * Synchronize start.
     */
    VOID syncStart();

    /**
     * Try synchronize start
     *
     * @return BOOL : TRUE means synchronize succeed, and FALSE failed.
     */
    BOOL trySyncStart();

    /**
     * Synchronize end.
     */
    VOID syncEnd();

protected:

#if defined(_WIN32) || defined(_WIN32_WCE)
    CRITICAL_SECTION    cs;
#else
    pthread_mutex_t     mutex;
    pthread_mutexattr_t attr;
#endif  //_LINUX

private:

    // Disable the copy constructor and operator =
    Ant_SyncObj(const Ant_SyncObj& src);
    Ant_SyncObj& operator = (const Ant_SyncObj& src);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_SYNCOBJ_H

/*---------------------------------------------------------------------------*/
/* EOF */
