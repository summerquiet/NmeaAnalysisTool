/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_THREAD_H
#define ANT_THREAD_H
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
#ifndef ANT_THREADSYSTEMIF_H
#   include "Ant_ThreadSystemIF.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class define

// Class declaration
class Ant_ThreadBase;

/**
 * @brief class represents a thread.
 *
 * @detail helper class to create thread and run code in the thread.
 */
class ANTLIB_API Ant_Thread : public virtual Ant_Object
{
public:

    Ant_Thread();
    Ant_Thread(Ant_ThreadSystemIF*);
    virtual ~Ant_Thread();

    virtual VOID extendStackSize();

    virtual VOID startThread(const XCHAR *name = XTEXT("Unknown Thread"));

    virtual BOOL stopThread(DWORD msec = INFINITE);

    virtual BOOL terminate();

    virtual BOOL join(DWORD msec);

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

    virtual VOID run();

    virtual Ant_ThreadSystemIF* getThreadSystem();

    static uint64_t getCurrentThreadId();

private:

    Ant_ThreadBase*     m_pThreadImpl;
    Ant_ThreadSystemIF* m_pThreadSystem;

private:

    // Disable the copy constructor and operator =
    Ant_Thread(const Ant_Thread&);
    Ant_Thread& operator = (const Ant_Thread&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_THREAD_H

/*---------------------------------------------------------------------------*/
/* EOF */
