/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_THREADUNIX_H
#define ANT_THREADUNIX_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_THREADBASE_H
#   include "Ant_ThreadBase.h"
#endif
#ifndef ANT_LIST_H
#   include "Ant_List.h"
#endif
#ifndef ANT_SYNCOBJ_H
#   include "Ant_SyncObj.h"
#endif

#include <pthread.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declaration
class Ant_Thread;

class Ant_ThreadUnix : public Ant_ThreadBase
{
public:

    Ant_ThreadUnix(Ant_Thread*);
    virtual ~Ant_ThreadUnix();
    
    virtual VOID extendStackSize();

    VOID startThread(const XCHAR *name = XTEXT("Unknown Thread"));

    virtual BOOL stopThread(DWORD msec = INFINITE);

    virtual BOOL terminate();

    virtual BOOL join(DWORD msec = INFINITE);

    virtual BOOL wait(DWORD msec = INFINITE);

    virtual VOID notify();

    virtual VOID resetName(const XCHAR* name);

    virtual INT getPriorityExt();

    virtual BOOL setPriorityExt(INT pri);

    virtual BOOL setPriorityNormal();

    virtual BOOL setPriorityLow();

    virtual BOOL setPriorityHigh();

    virtual BOOL isAlive();

    virtual BOOL isWait();

    virtual BOOL notifyMsg(LPVOID pAddr);

    static uint64_t getCurrentThreadId();

protected:

    virtual VOID execRun();

    VOID setThreadName();

private:

    Ant_SyncObj         m_cSyncObj;
    BOOL                m_bSignalFlg;
    BOOL                m_isAlive;
    pthread_t           m_tid;
    pthread_attr_t      m_attr;
    pthread_mutex_t     m_mutex;
    pthread_cond_t      m_cond;
#if defined(_LINUX)
    pthread_condattr_t  m_condattr;
#endif
#ifdef _FOR_APPLE_
    VOID*               m_pPool;
#endif
    Ant_List <VOID>     m_cMsgQue;
    Ant_SyncObj         m_cSyncMSg;

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_THREADUNIX_H

/*---------------------------------------------------------------------------*/
/* EOF */
