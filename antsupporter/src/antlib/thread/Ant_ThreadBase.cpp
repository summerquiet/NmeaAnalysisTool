/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_THREADBASE_H
#   include "Ant_ThreadBase.h"
#endif
#ifndef ANT_THREAD_H
#   include "Ant_Thread.h"
#endif

#if defined(_WIN32) || defined(_WIN32_WCE)
#else
#include "Ant_ThreadKey.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
VOID* Ant_ThreadBase::threadProc(VOID* p)
{
    if(p == NULL) return NULL;

#if defined(_WIN32) || defined(_WIN32_WCE)

#else
    Ant_ThreadKey::instance()->setThread(p);
#ifdef _FOR_ANDROID_
    Ant_ThreadKey::instance()->attachThread();
#endif
#endif

    (static_cast<Ant_ThreadBase*>(p))->execRun();

#if defined(_WIN32) || defined(_WIN32_WCE)

#else
    Ant_ThreadKey::instance()->setThread(NULL);
#ifdef _FOR_ANDROID_
    Ant_ThreadKey::instance()->detachThread();
#endif
#endif

    return NULL;
}

Ant_ThreadBase::Ant_ThreadBase(Ant_Thread *pThread)
: m_bQuitFlg(FALSE)
, m_dwThreadID(-1)
, m_nPrioIdx(0)
, m_dwSanityInterval(0)
, m_pThreadIF(pThread)
{
    memset(m_szThreadName, 0, sizeof(m_szThreadName));
    memset(m_aPrioTable, 0, sizeof(m_aPrioTable));
}

Ant_ThreadBase::~Ant_ThreadBase()
{

}

VOID
Ant_ThreadBase::extendStackSize()
{
    
}

VOID Ant_ThreadBase::startThread(const XCHAR* p)
{

}

BOOL Ant_ThreadBase::stopThread(DWORD msec)
{
    return FALSE;
}

BOOL Ant_ThreadBase::terminate()
{
    return FALSE;
}

BOOL Ant_ThreadBase::join(DWORD msec)
{
    return FALSE;
}

VOID Ant_ThreadBase::execRun()
{

}

BOOL Ant_ThreadBase::wait(DWORD msec)
{
    return FALSE;
}

VOID Ant_ThreadBase::notify()
{

}

BOOL Ant_ThreadBase::isAlive()
{
    return FALSE;
}

BOOL Ant_ThreadBase::isWait()
{
    return FALSE;
}

BOOL Ant_ThreadBase::checkQuit()
{
    return m_bQuitFlg;
}

XCHAR* Ant_ThreadBase::getName()
{
    return m_szThreadName;
}

VOID Ant_ThreadBase::resetName(const XCHAR* name)
{

}

DWORD Ant_ThreadBase::getThreadID()
{
    return m_dwThreadID;
}

INT Ant_ThreadBase::getPriorityExt()
{
    return 0;
}

BOOL Ant_ThreadBase::setPriorityExt(INT pri)
{
    return FALSE;
}

BOOL Ant_ThreadBase::setPriorityNormal()
{
    return FALSE;
}

BOOL Ant_ThreadBase::setPriorityLow()
{
    return FALSE;
}

BOOL Ant_ThreadBase::setPriorityHigh()
{
    return FALSE;
}

BOOL Ant_ThreadBase::notifyMsg(LPVOID pAddr)
{
    return FALSE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
