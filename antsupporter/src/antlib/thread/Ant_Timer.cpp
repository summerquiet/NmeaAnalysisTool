/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_TIMER_H
#   include "Ant_Timer.h"
#endif
#ifndef ANT_DEBUG_H
#   include "Ant_Debug.h"
#endif
#ifndef ANT_TIMEDEF_H
#   include "Ant_TimeDef.h"
#endif
#ifndef ANT_TIMERMANAGER_H
#if defined(ANT_HAVE_WIN32_API) && ANT_HAVE_WIN32_API
#   include "Ant_TimerManager_Win.h"
#elif defined(ANT_HAVE_POSIX_API) && ANT_HAVE_POSIX_API
#   include "Ant_TimerManager_Unix.h"
#endif
#endif
#ifdef _FOR_APPLE_
#   include "Ant_ThreadMac.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_TimerManager& Ant_Timer::getTimerManager()
{
    static Ant_TimerManager _tm;
    return _tm;
}

Ant_Timer::Ant_Timer(LONG tm, BOOL it)
: msec(tm)
, m_dTimerid(0)
, m_bIterate(it)
, used_timer_id(0)
, signal(0)
, m_pRequest(NULL)
{
    m_pRequest          = new Ant_TimerRequest();
    m_pRequest->timer   = this;
}

Ant_Timer::~Ant_Timer()
{
    stop();

    if (NULL != m_pRequest) {
        delete m_pRequest;
    }
}

BOOL Ant_Timer::initialize(Ant_TimerCallback * cTimerCB)
{
    if (NULL != cTimerCB) {
        getTimerManager().setTimerCallback(cTimerCB);
    }

    return TRUE;
}

BOOL Ant_Timer::start()
{
    if (m_dTimerid) {
        return TRUE;
    }
#ifdef _FOR_ANDROID_
    if (getpid() == gettid()) {
        ANT_PERROR("[Ant_Timer]start a timer in invalid thread (pid: %d)(tid: %d).", getpid(), gettid());
        return FALSE;
    }
#elif defined _FOR_APPLE_
    if (isMainThread()) {
        ANT_PERROR("[Ant_Timer]start a timer in invalid thread which is the main thread.");
        return FALSE;
    }
#endif
    signal = 0;
    BOOL ret = getTimerManager().registerTimer(this);
    used_timer_id = m_dTimerid;

    return ret;
}

VOID Ant_Timer::stop()
{
    if (!m_dTimerid) return;

    signal = 0;

    getTimerManager().removeTimer(this);
}

BOOL Ant_Timer::restart()
{
    return restart(msec);
}

BOOL Ant_Timer::restart(LONG time)
{
    msec = time;
    if (m_dTimerid) {
        signal = 0;
        getTimerManager().removeTimer(this);
        BOOL ret = getTimerManager().registerTimer(this);
        used_timer_id = m_dTimerid;
        return ret;
    }
    else {
        return(start());
    }

    return FALSE;
}

VOID Ant_Timer::doAction()
{
#ifdef _FOR_ANDROID_
    DWORD tc = ::GetTickCount();
    DWORD tt = ::GetThreadTime();
    ANT_PRINTF(("Ant_Timer[pid: %d][tid: %d] DoAction(id: %lu, msec: %d, iter: %d).", getpid(), gettid(), m_dTimerid, msec, m_bIterate));
#endif
    if (!m_bIterate)
    {
        getTimerManager().setInvalid(m_dTimerid);
        m_dTimerid = 0;
    }

    onTimer();

    signal = 0;

#ifdef _FOR_ANDROID_
    ANT_PRINTF(("Ant_Timer[pid: %d][tid: %d] OnTimer() d_tc: %d, d_tt: %d.", getpid(), gettid(), ::GetTickCount()-tc, ::GetThreadTime()-tt));
#endif
}

BOOL Ant_Timer::isValid(INT id)
{
    return getTimerManager().isValid(id);
}

VOID Ant_Timer::onTimer()
{
    
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
