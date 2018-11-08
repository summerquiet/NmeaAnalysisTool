/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_THREADUNIX_H
#   include "Ant_ThreadUnix.h"
#endif
#ifndef ANT_ERROR_H
#   include "Ant_Error.h"
#endif
#ifndef ANT_THREAD_H
#   include "Ant_Thread.h"
#endif
#ifndef ANT_TIMER_H
#   include "Ant_Timer.h"
#endif
#ifndef ANT_TIME_H
#   include "Ant_Time.h"
#endif
#ifndef ANT_DEBUG_H
#   include "Ant_Debug.h"
#endif
#ifndef ANT_THREADKEY_H
#   include "Ant_ThreadKey.h"
#endif
#ifndef ANT_AUTOSYNC_H
#   include "Ant_AutoSync.h"
#endif

#if defined(_LINUX)
#   define _GUN_SOURCE //open GUN features
#   define TASKNAME_COMLEN 16 //thread name could be only 16 byte long
#   include <sys/resource.h>
#   include <sys/types.h>
#elif defined(_FOR_ANDROID_)
#   include <sys/resource.h>
#   include <sys/prctl.h>
#elif defined(_FOR_APPLE_)
#   include "Ant_ThreadMac.h"
#endif

#include <string.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_ThreadUnix::Ant_ThreadUnix(Ant_Thread *pThread)
: Ant_ThreadBase(pThread)
, m_bSignalFlg(FALSE)
, m_isAlive(FALSE)
, m_tid(0)
{
    m_dwThreadID = 0;
    
#if defined(_FOR_APPLE_)
    m_pPool = NULL;
#endif
    INT ret = pthread_attr_init(&m_attr);
    if (0 != ret) {
        ANT_PERROR("Thread m_attr init failed %d [%s].", ret, strerror(ret));
    }

    ret = pthread_attr_setstacksize(&m_attr, 64*1024);
    if (0 != ret) {
        ANT_PERROR("Thread m_attr stacksize init failed %d [%s].", ret, strerror(ret));
    }

    ret = pthread_mutex_init(&m_mutex, NULL);
    if (0 != ret) {
        ANT_PERROR("Thread m_mutex init failed %d [%s].", ret, strerror(ret));
    }

#if defined(_LINUX)
    pthread_condattr_init(&m_condattr);
    int err = pthread_condattr_setclock(&m_condattr, CLOCK_MONOTONIC);
    if (err) {
        ANT_PERROR("Thread m_condattr setting clock: %d [%s]\n", err, strerror(err));
    }
    ret = pthread_cond_init(&m_cond, &m_condattr);
#else
    ret = pthread_cond_init(&m_cond, NULL);
#endif
    if (0 != ret) {
        ANT_PERROR("Thread m_cond init failed %d [%s].", ret, strerror(ret));
    }

    m_nPrioIdx = ANT_THREAD_PRIORITY_IDX_NORMAL;
    m_aPrioTable[ANT_THREAD_PRIORITY_IDX_NORMAL] = 0;//251;
    m_aPrioTable[ANT_THREAD_PRIORITY_IDX_LOW]    = 0;//251;
    m_aPrioTable[ANT_THREAD_PRIORITY_IDX_HIGH]   = 0;//251;
}

Ant_ThreadUnix::~Ant_ThreadUnix()
{
    stopThread();
    pthread_attr_destroy(&m_attr);
    pthread_cond_destroy(&m_cond);
#if defined(_LINUX)
    pthread_condattr_destroy(&m_condattr);
#endif
}

VOID Ant_ThreadUnix::extendStackSize()
{
    pthread_attr_setstacksize(&m_attr, 128*1024);
}

VOID Ant_ThreadUnix::startThread(const XCHAR* name)
{
    // make sure do not chock in the Ant_AutoSync while called by mutithread.
    if (m_isAlive) {
        return;
    }

    Ant_AutoSync cSync(m_cSyncObj);
    
    if (m_tid) {
        return;
    }

    //thread already started, meanwhile not ended, do not start again
    //thread already ran, but quit, then join the thread to release the resource
    if (m_isAlive) {
        return;
    }
    else if (m_tid) {
        join();
    }

    if (name == NULL) {
        name = XTEXT("Unknown Thread");
        ANT_PERROR("<<<< [%s] thread cannot be started.", name);
        return;
    }

    xcsncpy(m_szThreadName, name, ANT_THREAD_NAME_LEN);
    m_szThreadName[ANT_THREAD_NAME_LEN - 1] = TEXT('\0');

    m_bQuitFlg = FALSE;
    m_bSignalFlg = FALSE;

    Ant_ThreadSystemIF* pThreadSystem = NULL;
    if ((m_pThreadIF != NULL) && ((pThreadSystem = m_pThreadIF->getThreadSystem()) != NULL)) {
        BOOL ret = pThreadSystem->getThreadTableInfo(m_szThreadName, m_aPrioTable, m_dwSanityInterval);
        if (!ret) {
            ANT_PERROR("<<<< [%s] Get thread table info failed.", name);
            return;
        }
    }

    INT ret = pthread_create(&m_tid, &m_attr, threadProc, this);

    if (0 == ret) {
        m_isAlive = TRUE;
        ANT_PRINTF("<<<< Start thread [%s] TC: %lu.",  name, ::GetTickCount());
    }
}

BOOL Ant_ThreadUnix::stopThread(DWORD msec)
{
    if (!m_tid) {
        return TRUE;
    }

    m_bQuitFlg = TRUE;

    notify();

    return join();
}

BOOL Ant_ThreadUnix::terminate()
{
#if defined(_LINUX)
    if (!m_tid) {
        return TRUE;
    }

    m_bQuitFlg = TRUE;

    notify();

    if (!m_tid) {
        return TRUE;
    }

    INT ret = pthread_cancel(m_tid);

    if (0 == ret) {
        return TRUE;
    }
    else {
        ANT_PERROR("<<<< Stop thread [%s] (%lu, 0x%lx) join failed %d [%s].", m_szThreadName, m_dwThreadID, m_dwThreadID, ret, strerror(ret));
        return FALSE;
    }
    return TRUE;
#else
    return FALSE;
#endif
}

BOOL Ant_ThreadUnix::join(DWORD msec)
{
    if (!m_tid) {
        return TRUE;
    }

    INT ret = pthread_join(m_tid, NULL);

    if (0 == ret) {
        //for thread ended ;set thread_t threadid to initliaze stats
        m_tid = 0;
        return TRUE;
    }
    else {
        ANT_PERROR("<<<< Stop thread [%s] (%lu, 0x%lx) join failed %d [%s].", m_szThreadName, m_dwThreadID, m_dwThreadID, ret, strerror(ret));
        return FALSE;
    }
}

VOID Ant_ThreadUnix::execRun()
{
#if defined (_FOR_ANDROID_) || defined (_FOR_APPLE_) || defined (_LINUX)
    m_dwThreadID = getCurrentThreadId();
#else
    m_dwThreadID = (DWORD)m_tid;
#endif

#if defined(_LINUX)
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);        //允许退出线程
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);   //设置立即取消
#endif

    setPriorityNormal();
    setThreadName();

    ANT_PRINTF("<<<< Thread [%s] run (%lu, 0x%x) prio: %d, TC: %lu.", m_szThreadName, m_dwThreadID, m_dwThreadID, getPriorityExt(), ::GetTickCount());

#ifdef _FOR_APPLE_
    m_pPool = NULL;
    pool_alloc(&m_pPool);
#endif

    m_pThreadIF->run();
 
#ifdef _FOR_APPLE_
    pool_release(&m_pPool);
#endif

    ANT_PRINTF("<<<< Thread [%s] quit (%lu, 0x%x) TC: %lu.", m_szThreadName, m_dwThreadID, m_dwThreadID, ::GetTickCount());

    m_isAlive = FALSE;
    m_dwThreadID = 0;
}

BOOL Ant_ThreadUnix::wait(DWORD msec)
{
    INT ret = 0;

    pthread_mutex_lock(&m_mutex);

    if (m_bSignalFlg == FALSE) {
        if (INFINITE != msec) {
#ifdef _FOR_APPLE_
            struct timespec nptime;
            nptime.tv_sec = msec/1000;
            nptime.tv_nsec = msec%1000*1000000;

            ret = pthread_cond_timedwait_relative_np(&m_cond, &m_mutex, &nptime);

#elif defined(_FOR_ANDROID_)
            struct timespec nptime;
            gettimespec(&nptime, msec);

            ret = pthread_cond_timedwait_monotonic_np(&m_cond, &m_mutex, &nptime);

#else //_LINUX
            struct timespec nptime;
            gettimespec(&nptime, msec);

            ret = pthread_cond_timedwait(&m_cond, &m_mutex, &nptime);
#endif
        }
        else {
            ret = pthread_cond_wait(&m_cond, &m_mutex);
        }
    }

    m_bSignalFlg = FALSE;

    pthread_mutex_unlock(&m_mutex);

    //DWORD *pAddr = NULL; //[0] timer id; [1] timer pointer.
    while(1) {
        m_cSyncMSg.syncStart();
        intptr_t* pAddr = (intptr_t*)m_cMsgQue.pop();
        m_cSyncMSg.syncEnd();
        if (pAddr != NULL) {
            if (Ant_Timer::isValid(*pAddr) == TRUE) {
                reinterpret_cast<Ant_Timer*>(*(pAddr+1))->doAction();
            }
            delete[] pAddr;
        }
        else {
            break;
        }
    }

    // ETIMEDOUT
    if (0 == ret) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

VOID Ant_ThreadUnix::notify()
{
    pthread_mutex_lock(&m_mutex);
    m_bSignalFlg = TRUE;
    INT ret = pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_mutex);
}

VOID Ant_ThreadUnix::resetName(const XCHAR* name)
{
    if (!name) return;

    xcsncpy(m_szThreadName, name, ANT_THREAD_NAME_LEN);
    m_szThreadName[ANT_THREAD_NAME_LEN - 1] = TEXT('\0');
    setThreadName();
}

VOID Ant_ThreadUnix::setThreadName()
{
#if defined (_LINUX)
    //if thread is running  do reset the running threadname
    if (!m_tid) {
        return;
    }

    //for task name length can be only 16 byte
    char threadname[TASKNAME_COMLEN];
    strncpy(threadname, m_szThreadName, TASKNAME_COMLEN);
    threadname[TASKNAME_COMLEN - 1] = '\0';

    if (pthread_setname_np(m_tid, threadname)) {
        ANT_PERROR("<<<< Set thread [%lu] [%s] name to [%s] failed : %s.", m_dwThreadID, m_szThreadName, threadname, strerror(errno));
    }
    else {
        ANT_PRINTF("<<<< Set thread [%lu] [%s] name to [%s] TC : %lu.", m_dwThreadID, m_szThreadName, threadname, ::GetTickCount());
    }
#elif defined(_FOR_ANDROID_)
    if (m_dwThreadID != (DWORD)gettid()) {
        ANT_PERROR("<<<< Set thread [%s][%lu, 0x%x] name failed, NOT in current Thread.", m_szThreadName, m_dwThreadID, m_dwThreadID);
        return;
    }
    prctl(PR_SET_NAME, (unsigned long) m_szThreadName, 0, 0, 0);
    ANT_PRINTF("<<<< Set thread [%s] (%lu, 0x%x) name TC: %lu.", m_szThreadName, m_dwThreadID, m_dwThreadID, ::GetTickCount());
#elif defined(_FOR_APPLE_)
    if (m_dwThreadID != (DWORD)gettid()) {
        ANT_PERROR("<<<< Set thread [%s][%lu, 0x%x] name failed, NOT in current Thread.", m_szThreadName, m_dwThreadID, m_dwThreadID);
        return;
    }
    setCurrentThreadName(m_szThreadName);
#endif
}

INT Ant_ThreadUnix::getPriorityExt()
{
    int Priority = 0;
#if defined(_FOR_ANDROID_) || defined (_LINUX)
    Priority = getpriority(PRIO_PROCESS, m_dwThreadID);

#elif defined(_FOR_APPLE_)
    if (m_dwThreadID != (DWORD)gettid()) return -1;
    double macproi = getCurrentThreadPriority();
    Priority =  255 - macproi * (255 - 249);
#endif

    return Priority;
}

BOOL Ant_ThreadUnix::setPriorityExt(INT pri)
{
    if (!m_tid) {
        return FALSE;
    }

#if defined(_FOR_ANDROID_) || defined (_LINUX)
    int ret = setpriority(PRIO_PROCESS, m_dwThreadID, pri);
    if (ret < 0) {
        ANT_PERROR("<<<< Set thread [%s][%lu, 0x%x] priority : %d failed, ret = %d [%s].", m_szThreadName, m_dwThreadID, static_cast<UINT>(m_dwThreadID), pri, ret, strerror(ret));
    }
#elif defined _FOR_APPLE_
    if (m_dwThreadID != (DWORD)gettid()) {
        return FALSE;
    }
    double macpri = (double)pri;
    setCurrentThreadPriority(macpri);
#endif

    return TRUE;
}

BOOL Ant_ThreadUnix::setPriorityNormal()
{
    m_nPrioIdx = ANT_THREAD_PRIORITY_IDX_NORMAL;
    return setPriorityExt(m_aPrioTable[m_nPrioIdx]);
}

BOOL Ant_ThreadUnix::setPriorityLow()
{
    m_nPrioIdx = ANT_THREAD_PRIORITY_IDX_LOW;
    return setPriorityExt(m_aPrioTable[m_nPrioIdx]);
}

BOOL Ant_ThreadUnix::setPriorityHigh()
{
    m_nPrioIdx = ANT_THREAD_PRIORITY_IDX_HIGH;
    return setPriorityExt(m_aPrioTable[m_nPrioIdx]);
}

BOOL Ant_ThreadUnix::isAlive()
{
    return m_isAlive;
}

BOOL Ant_ThreadUnix::isWait()
{
    // pthread_mutex_lock(&m_mutex);
    BOOL ret = m_bSignalFlg;
    // pthread_mutex_unlock(&m_mutex);
    return (FALSE == ret);
}

BOOL Ant_ThreadUnix::notifyMsg(LPVOID pAddr)
{
    m_cSyncMSg.syncStart();
    m_cMsgQue.add(pAddr);
    m_cSyncMSg.syncEnd();

    pthread_mutex_lock(&m_mutex);
    m_bSignalFlg = TRUE;
    INT ret = pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_mutex);

    return TRUE;
}

uint64_t Ant_ThreadUnix::getCurrentThreadId()
{
    return gettid();
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
