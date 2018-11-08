/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_THREADSYSTEMIF_H
#define ANT_THREADSYSTEMIF_H
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

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

#define ANT_THREAD_INFO_NAME_LEN        (64)

#define ANT_THREAD_PRIORITY_IDX_NORMAL  0
#define ANT_THREAD_PRIORITY_IDX_LOW     1
#define ANT_THREAD_PRIORITY_IDX_HIGH    2

// Thread Table
struct ANT_THREAD_TABLE
{
    XCHAR*  thread_name;        // thread name
    INT     priority[3];        // thread priority, include {normal, low, high}
    DWORD   sanity_interval;    // (s)
};

struct ANTLIB_API Ant_ThreadSystemIF : public virtual Ant_Object
{
    virtual ~Ant_ThreadSystemIF() {};

    virtual BOOL getThreadTableInfo(const XCHAR* name, DWORD* priority, DWORD& sanity) = 0;

    virtual BOOL addThreadTable(const ANT_THREAD_TABLE*) = 0;

    //return thread No.
    virtual INT registerThread(XCHAR* name, HANDLE handle, DWORD thread_id) = 0;

    virtual VOID unregisterThread(INT thread_no) = 0;
};

/*---------------------------------------------------------------------------*/
// Function declare

extern ANTLIB_API VOID Ant_SetThreadSystem(Ant_ThreadSystemIF* sp);
extern ANTLIB_API Ant_ThreadSystemIF* Ant_GetThreadSystem();

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_THREADSYSTEMIF_H

/*---------------------------------------------------------------------------*/
/* EOF */
