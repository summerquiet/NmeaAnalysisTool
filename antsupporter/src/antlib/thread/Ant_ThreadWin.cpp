/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_THREADWIN_H
#   include "Ant_ThreadWin.h"
#endif
#ifndef ANT_THREAD_H
#   include "Ant_Thread.h"
#endif
#ifndef ANT_DEBUG_H
#   include "Ant_Debug.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_ThreadWin::Ant_ThreadWin(Ant_Thread *pThread)
: Ant_ThreadBase(pThread)
{    
    m_Handle  = NULL;
    
    m_hEvent    = ::CreateEvent(NULL, FALSE, FALSE, NULL);

    m_nPrioIdx = ANT_THREAD_PRIORITY_IDX_NORMAL;
    m_aPrioTable[ANT_THREAD_PRIORITY_IDX_NORMAL] = 2;
    m_aPrioTable[ANT_THREAD_PRIORITY_IDX_LOW]    = 2;
    m_aPrioTable[ANT_THREAD_PRIORITY_IDX_HIGH]   = 2;
}

Ant_ThreadWin::~Ant_ThreadWin()
{
    stopThread();

    if (m_hEvent) {
        ::CloseHandle(m_hEvent);
    }
}

VOID Ant_ThreadWin::startThread(const XCHAR* p)
{
    if (m_Handle) {
        return;
    }

    if (p == NULL)  {
        p = TEXT("Unknown Thread");
        ANT_PERROR("<<<< [%s] thread cannot be started.", p);
        return;
    }

    xcsncpy(m_szThreadName, p, ANT_THREAD_NAME_LEN);

    m_szThreadName[ANT_THREAD_NAME_LEN - 1] = TEXT('\0');

    m_bQuitFlg  = FALSE;

    m_Handle = ::CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)threadProc, (LPVOID)this, CREATE_SUSPENDED, &m_dwThreadID);

    Ant_ThreadSystemIF* pThreadSystem = NULL;
    if ((m_pThreadIF != NULL) && ((pThreadSystem = m_pThreadIF->getThreadSystem()) != NULL)) {
        BOOL ret = pThreadSystem->getThreadTableInfo(m_szThreadName, m_aPrioTable, m_dwSanityInterval);
        if (!ret) {
            ANT_PERROR("<<<< [%s] Get thread table info failed.", p);
            return;
        }
        setPriorityNormal();
    }

    ::ResumeThread(m_Handle);

    if (m_Handle) {
        ANT_PRINTF("<<<< start thread [%s] : %d",  p, ::GetTickCount());
    }
}

BOOL Ant_ThreadWin::stopThread(DWORD msec)
{    
    if (!m_Handle)    return TRUE;
    
    m_bQuitFlg = TRUE;                                                    
    
    notify();                                                                    
    
    BOOL ret = join(msec);    
    
    if (ret == FALSE) {
        //TODO: write error
        ANT_PRINTF("<<<< stop thread [%s] (0x%x) join failed", m_szThreadName, m_dwThreadID);
    }

    return ret;
}

BOOL Ant_ThreadWin::terminate()
{
    HANDLE tmp_handle = m_Handle;

    m_Handle = NULL;

    m_dwThreadID = 0;

    ::CloseHandle(tmp_handle);
    
    BOOL ret  = ::TerminateThread(tmp_handle, 0);
    
    return (ret) ? TRUE : FALSE;
}

BOOL Ant_ThreadWin::join(DWORD msec)
{
    if (!m_Handle) {
        return TRUE;
    }

    switch(::WaitForSingleObject(m_Handle, msec)) {

    case WAIT_OBJECT_0:
        return TRUE;
    case WAIT_TIMEOUT:
        return FALSE;
    default:
        return FALSE;
    }
}

VOID Ant_ThreadWin::execRun()
{
    m_dwThreadID = ::GetCurrentThreadId();

    ANT_PRINTF("<<<< Thread [%s] Run (0x%x) : %d", m_szThreadName, m_dwThreadID, ::GetTickCount());

    MSG msg;
    ::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

    m_pThreadIF->run();

    ::CloseHandle(m_Handle);

    ANT_PRINTF("<<<< Thread [%s] Quit (0x%x) : %d", m_szThreadName, m_dwThreadID, ::GetTickCount());

    m_Handle = NULL;

    m_dwThreadID = 0;
}

BOOL Ant_ThreadWin::wait(DWORD msec)
{
    DWORD start_time = ::GetTickCount();

    while(1) {
        DWORD wait_time;

        if (msec == INFINITE) {
            wait_time = INFINITE;
        }
        else {
            DWORD loss = ::GetTickCount() - start_time;
            wait_time = (msec > loss) ? (msec - loss) : 0;
        }

        switch(::MsgWaitForMultipleObjectsEx(1, &m_hEvent, wait_time, QS_ALLINPUT, MWMO_INPUTAVAILABLE)) {

        case WAIT_OBJECT_0:
            return TRUE;
        case WAIT_TIMEOUT:
            return FALSE;
        default:
            break;
        }
    }
}

VOID Ant_ThreadWin::notify()
{    
    ::SetEvent(m_hEvent);
}

INT Ant_ThreadWin::getPriorityExt()
{
    INT pri = 0;

    if (m_Handle) {
#if defined(UNDER_CE)
        pri = ::CeGetThreadPriority(m_Handle);            
#else    
        INT win_pri = 0;
        win_pri = ::GetThreadPriority(m_Handle);

        switch(win_pri){

        case THREAD_PRIORITY_HIGHEST:
            pri = 249;                                        
            break;
        case THREAD_PRIORITY_ABOVE_NORMAL:
            pri = 250;
            break;
        case THREAD_PRIORITY_NORMAL:
            pri = 251;
            break;
        case THREAD_PRIORITY_BELOW_NORMAL:
            pri = 252;
            break;
        case THREAD_PRIORITY_LOWEST:
            pri = 253;
            break;
        case THREAD_PRIORITY_IDLE:
            pri = 255;                                            
            break;
        default:
            pri = 251;                                        
            break;
        }
#endif
    }

    pri = pri - 249;
    return pri;
}

BOOL Ant_ThreadWin::setPriorityExt(INT pri)
{
    if (pri < 0) return FALSE;
    
    pri = pri + 249;

    if (m_Handle) {
#if defined(UNDER_CE)
        ::CeSetThreadPriority(m_Handle, pri);            
#else
        switch(pri){

        case 247:
        case 248:
        case 249:
            pri = THREAD_PRIORITY_HIGHEST;                                        
            break;
        case 250:
            pri = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case 251:
            pri = THREAD_PRIORITY_NORMAL;
            break;
        case 252:
            pri = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case 253:
            pri = THREAD_PRIORITY_LOWEST;
            break;
        case 254:
        case 255:
            pri = THREAD_PRIORITY_IDLE;                                            
            break;
        default:
            pri = THREAD_PRIORITY_NORMAL;                                        
            break;
        }
        ::SetThreadPriority(m_Handle, pri);
#endif
    } 
    return TRUE;
}

BOOL Ant_ThreadWin::setPriorityNormal()
{
    m_nPrioIdx = ANT_THREAD_PRIORITY_IDX_NORMAL;
    return setPriorityExt(m_aPrioTable[m_nPrioIdx]);
}
    
BOOL Ant_ThreadWin::setPriorityLow()
{
    m_nPrioIdx = ANT_THREAD_PRIORITY_IDX_LOW;
    return setPriorityExt(m_aPrioTable[m_nPrioIdx]);
}

BOOL Ant_ThreadWin::setPriorityHigh()
{
    m_nPrioIdx = ANT_THREAD_PRIORITY_IDX_HIGH;
    return setPriorityExt(m_aPrioTable[m_nPrioIdx]);
}

BOOL Ant_ThreadWin::isAlive()
{
    if (NULL != m_Handle) {
        return TRUE;
    } 
    else {
        return FALSE;
    }
}

BOOL Ant_ThreadWin::isWait()
{
    return FALSE;
}

uint64_t Ant_ThreadWin::getCurrentThreadId()
{
    return ::GetCurrentThreadId();
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
