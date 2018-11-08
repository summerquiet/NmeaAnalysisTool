/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_THREADWIN_H
#define ANT_THREADWIN_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_THREADBASE_H
#   include "Ant_ThreadBase.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declaration
class Ant_ThreadWin;
class Ant_Thread;

class Ant_ThreadWin : public Ant_ThreadBase
{
public:

    Ant_ThreadWin(Ant_Thread *);
    virtual ~Ant_ThreadWin();
    
    virtual VOID extendStackSize(){}

    VOID startThread(const XCHAR *name = XTEXT("Unknown Thread"));

    virtual BOOL stopThread(DWORD msec = INFINITE);

    virtual BOOL terminate();
                            
    virtual BOOL join(DWORD msec = INFINITE);
    
    virtual BOOL wait(DWORD msec = INFINITE);

    virtual VOID notify();

    virtual INT getPriorityExt();

    virtual BOOL setPriorityExt(INT pri);

    virtual BOOL setPriorityNormal();
    
    virtual BOOL setPriorityLow();

    virtual BOOL setPriorityHigh();

    virtual BOOL isAlive();

    virtual BOOL isWait();

    static uint64_t getCurrentThreadId();

protected:

    virtual VOID execRun();

private:

    HANDLE m_Handle;    
    HANDLE m_hEvent;

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_THREADWIN_H

/*---------------------------------------------------------------------------*/
/* EOF */
