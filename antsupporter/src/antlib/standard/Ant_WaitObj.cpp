/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_WAITOBJ_H
#   include "Ant_WaitObj.h"
#endif
#ifndef ANT_TIME_H
#   include "Ant_Time.h"
#endif
#ifndef ANT_DEBUG_H
#   include "Ant_Debug.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_WaitObj::Ant_WaitObj(BOOL manual)
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    hEvent = ::CreateEvent(NULL, manual, FALSE, NULL);
#else
    pthread_mutex_init(&mutex, NULL);
#if defined(_LINUX)
    pthread_condattr_t condattr;
    pthread_condattr_init(&condattr);
    pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
    pthread_cond_init(&cond, &condattr);
#else
    pthread_cond_init(&cond, NULL);
#endif
    signal_flag = FALSE;
    manual_flag = manual;
#endif
}

Ant_WaitObj::~Ant_WaitObj()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    if(hEvent) ::CloseHandle(hEvent);
#else
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
#if defined(_LINUX)
    pthread_condattr_destroy(&condattr);
#endif
#endif
}

BOOL Ant_WaitObj::wait(DWORD msec)
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    if(::WaitForSingleObject(hEvent, msec) == WAIT_OBJECT_0) {
        return TRUE;
    }
    else {
        return FALSE;
    }
#else
    pthread_mutex_lock(&mutex);
    int ret = 0;
    if(signal_flag == FALSE) {
        if(INFINITE != msec) {
#ifdef _FOR_APPLE_
            struct timespec nptime;
            nptime.tv_sec = msec/1000;
            nptime.tv_nsec = msec%1000*1000000;
            ret = pthread_cond_timedwait_relative_np(&cond, &mutex, &nptime);
#elif defined(_FOR_ANDROID_)
            struct timespec nptime;
            gettimespec(&nptime, msec);
            ret = pthread_cond_timedwait_monotonic_np(&cond, &mutex, &nptime);
#else //_LINUX
            struct timespec nptime;
            gettimespec(&nptime, msec);
            ret = pthread_cond_timedwait(&cond, &mutex, &nptime);
#endif
            if (ret && ret != ETIMEDOUT) {
                ANT_PERROR("Ant_WaitObj wait failed (%d), [%s]\n", ret, strerror(ret));
            }
        }
        else {
            ret = pthread_cond_wait(&cond, &mutex);
        }
    }
    if (!manual_flag) signal_flag = FALSE;
    pthread_mutex_unlock(&mutex);

    return (ret==0)?TRUE:FALSE;
#endif
}

VOID Ant_WaitObj::notify()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    ::SetEvent(hEvent);
#else
    pthread_mutex_lock(&mutex);
    signal_flag = TRUE;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
#endif
}

VOID Ant_WaitObj::reset()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    ::ResetEvent(hEvent);
#else
    pthread_mutex_lock(&mutex);
    signal_flag = FALSE;
    pthread_mutex_unlock(&mutex);
#endif
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
