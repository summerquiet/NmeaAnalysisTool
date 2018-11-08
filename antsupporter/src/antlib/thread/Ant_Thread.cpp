/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files
#ifndef ANT_THREAD_H
#   include "Ant_Thread.h"
#endif

#if defined(_WIN32) || defined(_WIN32_WCE)
#ifndef ANT_THREADWIN_H
#   include "Ant_ThreadWin.h"
#endif
#else
#ifndef ANT_THREADUNIX_H
#   include "Ant_ThreadUnix.h"
#endif
#endif

#ifndef ANT_THREADBASE_H
#   include "Ant_ThreadBase.h"
#endif

#if defined(_WIN32) || defined(_WIN32_WCE)
#define Ant_ThreadImpl Ant_ThreadWin
#else
#define Ant_ThreadImpl Ant_ThreadUnix
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_Thread::Ant_Thread()
{
    m_pThreadImpl = new Ant_ThreadImpl(this);
    m_pThreadSystem = Ant_GetThreadSystem();
}

Ant_Thread::Ant_Thread(Ant_ThreadSystemIF* sp)
{
    m_pThreadImpl = new Ant_ThreadImpl(this);
    m_pThreadSystem = sp;
}

Ant_Thread::~Ant_Thread()
{
    delete m_pThreadImpl;
    m_pThreadImpl = NULL;
}

VOID Ant_Thread::extendStackSize()
{
    stopThread();
    m_pThreadImpl->extendStackSize();
}

VOID Ant_Thread::startThread(const XCHAR* p)
{
    m_pThreadImpl->startThread(p);
}

BOOL Ant_Thread::stopThread(DWORD msec)
{
    return m_pThreadImpl->stopThread(msec);
}

BOOL Ant_Thread::terminate()
{
    return m_pThreadImpl->terminate();
}

BOOL Ant_Thread::join(DWORD msec)
{
    return m_pThreadImpl->join(msec);
}

BOOL Ant_Thread::wait(DWORD msec)
{
    return m_pThreadImpl->wait(msec);
}

VOID Ant_Thread::notify()
{
    m_pThreadImpl->notify();
}

BOOL Ant_Thread::isAlive()
{
    return m_pThreadImpl->isAlive();
}

BOOL Ant_Thread::isWait()
{
    return m_pThreadImpl->isWait();
}

BOOL Ant_Thread::checkQuit()
{
    return m_pThreadImpl->checkQuit();
}

XCHAR* Ant_Thread::getName()
{
    return m_pThreadImpl->getName();
}

VOID Ant_Thread::resetName(const XCHAR* name)
{
    m_pThreadImpl->resetName(name);
}

DWORD Ant_Thread::getThreadID()
{
    return m_pThreadImpl->getThreadID();
}

INT Ant_Thread::getPriorityExt()
{
    return m_pThreadImpl->getPriorityExt();
}

BOOL Ant_Thread::setPriorityExt(INT pri)
{
    return m_pThreadImpl->setPriorityExt(pri);
}

BOOL Ant_Thread::setPriorityNormal()
{
    return m_pThreadImpl->setPriorityNormal();
}

BOOL Ant_Thread::setPriorityLow()
{
    return m_pThreadImpl->setPriorityLow();
}

BOOL Ant_Thread::setPriorityHigh()
{
    return m_pThreadImpl->setPriorityHigh();
}

BOOL Ant_Thread::notifyMsg(LPVOID pAddr)
{
    return m_pThreadImpl->notifyMsg(pAddr);
}

VOID Ant_Thread::run()
{

}

Ant_ThreadSystemIF* Ant_Thread::getThreadSystem()
{
    if (NULL == m_pThreadSystem) {
        m_pThreadSystem = Ant_GetThreadSystem();
    }
    return m_pThreadSystem;
}

uint64_t Ant_Thread::getCurrentThreadId()
{
    return Ant_ThreadImpl::getCurrentThreadId();
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
