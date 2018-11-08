/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_TIMERMANAGER_WIN_H
#   include "Ant_TimerManager_Win.h"
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
#ifndef ANT_DEBUG_H
#   include "Ant_Debug.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
BOOL Ant_TimerRequest::operator == (const Ant_TimerRequest& req) const
{
    return (timeout_tick == req.timeout_tick) ? TRUE : FALSE;
}

BOOL Ant_TimerRequest::operator < (const Ant_TimerRequest& req) const
{
    if (timeout_tick > req.timeout_tick 
        && timeout_tick - req.timeout_tick > 0x80000000) {
            return TRUE;
    }
    else if (timeout_tick < req.timeout_tick 
        && req.timeout_tick - timeout_tick > 0x80000000) {
            return FALSE;
    }
    else {
        return (timeout_tick < req.timeout_tick);
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
    if (timeout_tick < req.timeout_tick 
        && req.timeout_tick - timeout_tick > 0x80000000) {
            return TRUE;
    }
    else if (timeout_tick > req.timeout_tick 
        && timeout_tick - req.timeout_tick > 0x80000000) {
            return FALSE;
    }
    else {
        return (timeout_tick > req.timeout_tick);
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
    m_evHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_dNextid = 0;
    memset(timer_table, 0, ANT_TIMER_MAX_NUM);
    need_sanity_reply = FALSE;

    startThread(TEXT("Ant_TimerManagerThread"));
}

Ant_TimerManager::~Ant_TimerManager()
{
    ::CloseHandle(m_evHandle);
    terminate();
}

DWORD  Ant_TimerManager::nextID()
{
    for (INT i = 0; i < ANT_TIMER_MAX_NUM; i++) {
        m_dNextid++;
        m_dNextid %= ANT_TIMER_MAX_NUM;

        if (m_dNextid != 0 && timer_table[m_dNextid] == 0) {
            return m_dNextid;
        }
    }

    return 0;
}

VOID Ant_TimerManager::setTimerCallback(Ant_TimerCallback * cTimerCB)
{
    if (NULL != m_ptimercallback) {
        ANT_PRINTF("Ant_TimerManager::SetTimerCallback --->>> the m_ptimercallback is changed!");
        delete m_ptimercallback;
    }
    m_ptimercallback = cTimerCB;
}

VOID  Ant_TimerManager::run()
{
    INT ret = 0;
    Ant_TimerRequest* tmp_req = NULL;
    Ant_TimerRequest current_time;
    DWORD sleep_time = INFINITE;

    setPriorityExt(240);

    while(!checkQuit()) {

        ::WaitForSingleObject(m_evHandle, sleep_time);

        m_cSyncObj.syncStart();

        if (need_sanity_reply) {
            //TODO: not implement
            need_sanity_reply = FALSE;
        }

        if (m_cReqList.hasData()) {
            while ((tmp_req = m_cReqList.pop()) != NULL) {

                current_time.timeout_tick = ::GetTickCount();

                if (*tmp_req <= current_time) {
                    int retry = 0;
                    if (!(tmp_req->timer->m_bIterate && tmp_req->timer->signal)) {

                        InterlockedExchange((long*)&tmp_req->timer->signal, 1);
                        if (NULL != m_ptimercallback) {
                            intptr_t *ptr = new intptr_t[2];
                            ptr[0] = tmp_req->timer->m_dTimerid;
                            ptr[1] = (intptr_t)tmp_req->timer;
                            m_ptimercallback->callback((LPVOID)ptr);
                        }
                        else {
                            while(!::PostThreadMessage(tmp_req->thread_id, ANT_WM_TIMER, (WPARAM)tmp_req->timer->m_dTimerid, (LPARAM)tmp_req->timer)) {
                                if (++retry > 5) {
                                    InterlockedExchange((long*)&tmp_req->timer->signal, 0);
                                    break;
                                }
                                Sleep(1);
                            }
                        }
                    }

                    if (tmp_req->timer->m_bIterate && retry < 5) {
                        tmp_req->timeout_tick += tmp_req->timer->msec;
                        addRequest(tmp_req);
                    }

                    sleep_time = INFINITE;
                }
                else {
                    m_cReqList.insert(tmp_req);
                    sleep_time = tmp_req->timeout_tick - current_time.timeout_tick;
                    break;
                }
            }
        }
        else {
            sleep_time = INFINITE;
        }

        m_cSyncObj.syncEnd();
    }
}

VOID Ant_TimerManager::addRequest(Ant_TimerRequest* new_req)
{
    INT pos = 0;

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
        Ant_TimerRequest*tmp_req = iter.current();
        if (tmp_req->timer->m_dTimerid == id) {
            break;
        }
    }
    iter.removeCurrent();
}

BOOL Ant_TimerManager::registerTimer(Ant_Timer* timer)
{
    m_cSyncObj.syncStart();

    timer->m_pRequest->timeout_tick = ::GetTickCount() + timer->msec;
    timer->m_pRequest->thread_id = ::GetCurrentThreadId();
    timer->m_dTimerid = nextID();

    BOOL ret = (timer->m_dTimerid != 0);
    if (ret == TRUE) {
        timer_table[timer->m_dTimerid] = 1;
        addRequest(timer->m_pRequest);
    }

    m_cSyncObj.syncEnd();

    return ret;
}

VOID Ant_TimerManager::removeTimer(Ant_Timer* timer)
{
    m_cSyncObj.syncStart();
    timer_table[timer->m_dTimerid] = 0;
    removeRequest(timer->m_dTimerid);
    timer->m_dTimerid = 0;
    m_cSyncObj.syncEnd();
}

BOOL Ant_TimerManager::isValid(INT id)
{
    if (id <= 0 || id >= ANT_TIMER_MAX_NUM) {
        return FALSE;
    }
    else {
        return timer_table[id];
    }
}

VOID Ant_TimerManager::setInvalid(INT id)
{
    if (id > 0 && id < ANT_TIMER_MAX_NUM) {
        timer_table[id] = 0;
    }
}

BOOL Ant_TimerManager::requestSanity()
{
    m_cSyncObj.syncStart();
    need_sanity_reply = TRUE;
    m_cSyncObj. syncEnd();

    ::SetEvent(m_evHandle);

    return FALSE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
