/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_TIMERMANAGER_UNIX_H
#   include "Ant_TimerManager_Unix.h"
#endif
#ifndef ANT_TIMER_H
#   include "Ant_Timer.h"
#endif
#ifndef ANT_TIME_H
#   include "Ant_Time.h"
#endif
#ifndef ANT_THREAD_H
#   include "Ant_Thread.h"
#endif
#ifndef ANT_THREADKEY_H
#   include "Ant_ThreadKey.h"
#endif
#ifndef ANT_DEBUG_H
#   include "Ant_Debug.h"
#endif
#   include <string.h>
#if defined(_FOR_ANDROID_) || defined (_LINUX)
#   include <sys/resource.h>
#   include <sys/prctl.h>
#endif //_FOR_ANDROID_

#ifdef _FOR_APPLE_
#   include "Ant_ThreadMac.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define
#define TASKNAME_COMLEN 16 //thread name could be only 16 byte long

/*---------------------------------------------------------------------------*/
// Functions
BOOL Ant_TimerRequest::operator == (const Ant_TimerRequest& req) const
{
    INT ret = subtimespec((timespec*)&timeout_tick, (timespec*)&req.timeout_tick);
    if (0 == ret) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

BOOL Ant_TimerRequest::operator < (const Ant_TimerRequest& req) const
{
    INT ret = subtimespec((timespec*)&timeout_tick, (timespec*)&req.timeout_tick);

    if (ret < 0) {
        return TRUE;
    }
    else {
        return FALSE;
    }

}

BOOL Ant_TimerRequest::operator <= (const Ant_TimerRequest& req) const
{
    if ((*this < req) || (*this == req)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

BOOL Ant_TimerRequest::operator > (const Ant_TimerRequest& req) const
{
    INT ret = subtimespec((timespec*)&timeout_tick, (timespec*)&req.timeout_tick);

    if (ret > 0) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

BOOL Ant_TimerRequest::operator >= (const Ant_TimerRequest& req) const
{
    if ((*this > req) || (*this == req)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

Ant_TimerManager::Ant_TimerManager()
: m_ptimercallback(NULL)
{
    INT ret = 0;
    ret = pthread_mutex_init(&m_mutex, NULL);
    if (0 != ret) {
        ANT_PERROR("thread mutex init failed %d [%s].", ret, strerror(ret));
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
        ANT_PERROR("thread cond init failed %d [%s].", ret, strerror(ret));
    }

    m_dNextid = 0;
    memset(timer_table, 0, NTIMER_MAX_NUM);

    need_sanity_reply = FALSE;

    m_bQuitflg = FALSE;

    m_bSignalflg = FALSE;

    ret = pthread_attr_init(&m_attr);
    if (0!= ret) {
        ANT_PERROR("Ant_TimerManager attr init failed %d [%s].", ret, strerror(ret));
    }

    ret = pthread_attr_setscope(&m_attr, PTHREAD_SCOPE_SYSTEM);
    if (0!= ret) {
        ANT_PERROR("thread attr scope init failed %d [%s].", ret, strerror(ret));
    }

    ret = pthread_attr_setschedpolicy(&m_attr, SCHED_RR);
    if (0!= ret) {
        ANT_PERROR("NTimerManager attr policy init failed %d [%s].", ret, strerror(ret));
    }

    ret = pthread_attr_setstacksize(&m_attr, 64*1024);
    if (0 != ret ) {
        ANT_PERROR("NTimerManager attr stacksize init failed %d [%s].", ret, strerror(ret));
    }

    ret = pthread_create(&m_tid,&m_attr, threadProc, this);
    if (0 != ret) {
        ANT_PERROR("start thread NTimerManager failed %d [%s].", ret, strerror(ret));
    }

    m_dwThreadID = 0;
}

Ant_TimerManager::~Ant_TimerManager()
{
    if (0 == m_tid) {
        return;
    }

    m_bQuitflg = TRUE;

    pthread_cond_signal(&m_cond);

    INT ret = pthread_join(m_tid, NULL);

    pthread_mutex_destroy(&m_mutex);

    pthread_cond_destroy(&m_cond);

#if defined(_LINUX)
    pthread_condattr_destroy(&m_condattr);
#endif
}

VOID Ant_TimerManager::setTimerCallback(Ant_TimerCallback* cTimerCB)
{
    m_ptimercallback = cTimerCB;
}

VOID* Ant_TimerManager::threadProc(VOID* p)
{
    if (p == NULL) {
        return NULL;
    }

    (static_cast<Ant_TimerManager*>(p))->run();

    pthread_exit((VOID*)0);
    return NULL;
}

DWORD Ant_TimerManager::nextId()
{
    for (INT i = 0; i < NTIMER_MAX_NUM; i++) {
        m_dNextid++;
        m_dNextid %= NTIMER_MAX_NUM;

        if (m_dNextid != 0 && timer_table[m_dNextid] == 0) {
            return m_dNextid;
        }
    }

    return 0;
}

VOID Ant_TimerManager::notify()
{
    pthread_mutex_lock(&m_mutex);
    m_bSignalflg = TRUE;
    INT ret;
    ret = pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_mutex);
}
void Ant_TimerManager::setThreadName(char* thread_name)
{
#if defined (_LINUX)
    //if thread is running  do reset the running threadname
    if (!m_tid) return;
    //for task name length can be only 16 byte
    char threadname[TASKNAME_COMLEN];
    strncpy(threadname, thread_name, TASKNAME_COMLEN);
    threadname[TASKNAME_COMLEN - 1] = '\0';
    if (pthread_setname_np(m_tid, threadname)) {
        ANT_PERROR("<<<< Set thread [%lu] [%s] name to [%s] failed : %s.", m_dwThreadID, thread_name, threadname, strerror(errno));
    }
    else {
        ANT_PRINTF("<<<< Set thread [%lu] [%s] name to [%s] TC : %lu.", m_dwThreadID, thread_name, threadname, ::GetTickCount());
    }
#elif defined(_FOR_ANDROID_)
    if (m_dwThreadID != (DWORD)gettid()) {
        ANT_PERROR("<<<< Set thread [%s][%lu, 0x%x] name failed, NOT in current Thread.", thread_name, m_dwThreadID, m_dwThreadID);
        return;
    }
    prctl(PR_SET_NAME, (unsigned long) thread_name, 0, 0, 0);
    ANT_PRINTF("<<<< Set thread [%s] (%lu, 0x%x) name TC: %lu.", thread_name, m_dwThreadID, m_dwThreadID, ::GetTickCount());
#elif defined(_FOR_APPLE_)
    if (m_dwThreadID != (DWORD)gettid()) {
        ANT_PERROR("<<<< Set thread [%s][%lu, 0x%x] name failed, NOT in current Thread.", thread_name, m_dwThreadID, m_dwThreadID);
        return;
    }
    setCurrentThreadName(thread_name);
#endif
}
VOID Ant_TimerManager::run()
{
#if defined (_FOR_ANDROID_) || defined (_FOR_APPLE_) || defined (_LINUX)
    m_dwThreadID = gettid();
#else
    m_dwThreadID = (DWORD)m_tid;
#endif
    
    char cCLTimerMgrThreadName[] = "CLTimerThread";
    setThreadName(cCLTimerMgrThreadName);
    INT                 ret = 0;
    Ant_TimerRequest*   tmp_req = NULL;
    Ant_TimerRequest    current_time;
    DWORD               sleep_time = INFINITE;

#if defined(_FOR_ANDROID_) || defined (_LINUX)
    int pri = -19; //highest
    ret = setpriority(PRIO_PROCESS, gettid(), pri);
    if (ret < 0) {
        ANT_PERROR("<<<< Set thread [Ant_TimerManager][%d] priority : %d failed, ret = %d [%s].", gettid(), pri, ret, strerror(ret));
    }
    pri = getpriority(PRIO_PROCESS, gettid());
    ANT_PRINTF("<<<< Thread [Ant_TimerManager] run (%d) prio: %d, TC: %lu.", gettid(), pri, ::GetTickCount());
    //set thread name
    prctl(PR_SET_NAME, (unsigned long) "Ant_TimerManager", 0, 0, 0);
#endif

    while (!checkQuit()) {

        pthread_mutex_lock(&m_mutex);

        if (m_bSignalflg == FALSE) {
            if (INFINITE != sleep_time && sleep_time != 0) {
#ifdef _FOR_APPLE_
                struct timespec nptime;
                nptime.tv_sec = sleep_time/1000;
                nptime.tv_nsec = sleep_time%1000*1000000;

                ret = pthread_cond_timedwait_relative_np(&m_cond, &m_mutex, &nptime);

#elif defined(_FOR_ANDROID_)
                struct timespec nptime;
                gettimespec(&nptime, sleep_time);

                ANT_PRINTF(("Ant_TimerManager[pid: %d][tid: %d] wait(%lu) TC: %d, TT: %d.", getpid(), gettid(), sleep_time, GetTickCount(), GetThreadTime()));
                ret = pthread_cond_timedwait_monotonic_np(&m_cond, &m_mutex, &nptime);
                ANT_PRINTF(("Ant_TimerManager[pid: %d][tid: %d] weakup TC: %d, TT: %d.", getpid(), gettid(), sleep_time, GetTickCount(), GetThreadTime()));
#else //_LINUX
                struct timespec nptime;
                gettimespec(&nptime, sleep_time);

                ANT_PRINTF(("Ant_TimerManager[pid: %d][tid: %d] wait(%lu) TC: %d, TT: %d.", getpid(), gettid(), sleep_time, GetTickCount(), GetThreadTime()));
                ret = pthread_cond_timedwait(&m_cond, &m_mutex, &nptime);
                ANT_PRINTF(("Ant_TimerManager[pid: %d][tid: %d] weakup TC: %d, TT: %d.", getpid(), gettid(), sleep_time, GetTickCount(), GetThreadTime()));
#endif
            }
            else {
                ret = pthread_cond_wait(&m_cond, &m_mutex);
            }
        }

        m_bSignalflg = FALSE;

        pthread_mutex_unlock(&m_mutex);

        syncStart();

        if (m_cReqList.hasData()) {

            while ((tmp_req = m_cReqList.pop())) {

                gettimespec(&current_time.timeout_tick, 0);

                if (*tmp_req <= current_time) {

                    if (!(tmp_req->timer->m_bIterate && tmp_req->timer->signal)) {

                        tmp_req->timer->signal = 1;
                        ANT_PRINTF(("Ant_TimerManager[pid: %d][tid: %d] NotifyMsg(id: %lu).", getpid(), gettid(), tmp_req->timer->m_dTimerid));
                        intptr_t *ptr = new intptr_t[2];
                        ptr[0] = tmp_req->timer->m_dTimerid;
                        ptr[1] = (intptr_t)tmp_req->timer;
                        if (NULL != m_ptimercallback) {
                            m_ptimercallback->callback((LPVOID)ptr);
                        }
                        else {
                            ((Ant_Thread*)(tmp_req->pthread))->notifyMsg((LPVOID)ptr);
                        }
                    }
                    if (tmp_req->timer->m_bIterate) {
                        gettimespec(&tmp_req->timeout_tick, tmp_req->timer->msec);
                        addRequest(tmp_req);
                    }

                    sleep_time = INFINITE;
                }
                else {
                    m_cReqList.insert(tmp_req);
                    sleep_time = subtimespec(&tmp_req->timeout_tick, &current_time.timeout_tick)+1;
                    break;
                }
            }
        }
        else {
            sleep_time = INFINITE;
        }

        syncEnd();
    }
}

VOID Ant_TimerManager::addRequest(Ant_TimerRequest* new_req)
{
    INT pos;

    Ant_List<Ant_TimerRequest>::Iterator iter = m_cReqList.begin();

    for (pos = 0; pos < m_cReqList.length(); pos++, iter()) {
        Ant_TimerRequest* tmp_req = iter.current();
        if (*new_req < *tmp_req) {
            break;
        }
    }

    m_cReqList.insertPos(new_req, pos);

    if (pos == 0) {
        notify();
    }
}

VOID Ant_TimerManager::removeRequest(DWORD id)
{
    Ant_List<Ant_TimerRequest>::Iterator iter = m_cReqList.begin();

    for (INT i = 0; i < m_cReqList.length(); i++, iter()) {
        Ant_TimerRequest* tmp_req = iter.current();
        if (tmp_req->timer->m_dTimerid == id) {
            break;
        }
    }
    iter.removeCurrent();
}

BOOL Ant_TimerManager::registerTimer(Ant_Timer* timer)
{
    syncStart();

    gettimespec(&timer->m_pRequest->timeout_tick, timer->msec);

    timer->m_pRequest->pthread = reinterpret_cast<Ant_Thread*>(Ant_ThreadKey::instance()->getThread());

    timer->m_dTimerid = nextId();

    BOOL ret = (timer->m_dTimerid != 0);

    if (ret == TRUE) {
        ANT_PRINTF(("Ant_TimerManager[pid: %d][tid: %d] RegisterTimer(id: %lu, msec: %d, iter: %d).", getpid(), gettid(), timer->m_dTimerid, timer->msec, timer->m_bIterate));
        timer_table[timer->m_dTimerid] = 1;
        addRequest(timer->m_pRequest);
    }
    syncEnd();

    return ret;
}

VOID Ant_TimerManager::removeTimer(Ant_Timer* timer)
{
    syncStart();
    ANT_PRINTF(("Ant_TimerManager[pid: %d][tid: %d] RemoveTimer(id: %lu).", getpid(), gettid(), timer->m_dTimerid));
    timer_table[timer->m_dTimerid] = 0;
    removeRequest(timer->m_dTimerid);
    timer->m_dTimerid = 0;
    syncEnd();
}

BOOL Ant_TimerManager::isValid(INT id)
{
    if (id <= 0 || id >= NTIMER_MAX_NUM) {
        return FALSE;
    }
    else {
        return timer_table[id];
    }
}

VOID Ant_TimerManager::setInvalid(INT id)
{
    if (id > 0 && id < NTIMER_MAX_NUM) {
        timer_table[id] = 0;
    }
}

BOOL Ant_TimerManager::requestSanity()
{
    syncStart();
    need_sanity_reply = TRUE;
    syncEnd();

    INT ret;
    ret = pthread_cond_signal(&m_cond);

    return FALSE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
