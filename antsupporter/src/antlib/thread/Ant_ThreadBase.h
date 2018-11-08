/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_THREADBASE_H
#define ANT_THREADBASE_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_THREADSYSTEM_H
#   include "Ant_ThreadSystem.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declaration
class Ant_Thread;

#define ANT_THREAD_NAME_LEN    64

class Ant_ThreadBase
{
public:

    Ant_ThreadBase(Ant_Thread *);
    virtual ~Ant_ThreadBase();
    
    virtual VOID extendStackSize();

    virtual VOID startThread(const XCHAR *name = XTEXT("Unknown Thread"));

    virtual BOOL stopThread(DWORD msec = INFINITE);

    virtual BOOL terminate();

    virtual BOOL join(DWORD msec = INFINITE);

    virtual BOOL wait(DWORD msec = INFINITE);

    virtual VOID notify();

    virtual BOOL isAlive();

    virtual BOOL isWait();

    virtual BOOL checkQuit();

    virtual XCHAR* getName();

    virtual VOID resetName(const XCHAR* name);

    virtual DWORD getThreadID();

    virtual INT getPriorityExt();

    virtual BOOL setPriorityExt(INT pri);

    virtual BOOL setPriorityNormal();

    virtual BOOL setPriorityLow();

    virtual BOOL setPriorityHigh();

    virtual BOOL notifyMsg(LPVOID pAddr);

protected:

    static VOID* threadProc(VOID* p);

    virtual VOID execRun();

    BOOL     m_bQuitFlg;
    XCHAR       m_szThreadName[ANT_THREAD_INFO_NAME_LEN];
    DWORD       m_dwThreadID;
    INT         m_nPrioIdx;
    DWORD       m_aPrioTable[3];
    DWORD       m_dwSanityInterval;
    Ant_Thread* m_pThreadIF;

private:

    // Disable the copy constructor and operator =
    Ant_ThreadBase(const Ant_ThreadBase&);
    Ant_ThreadBase& operator = (const Ant_ThreadBase&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_THREADBASE_H

/*---------------------------------------------------------------------------*/
/* EOF */
