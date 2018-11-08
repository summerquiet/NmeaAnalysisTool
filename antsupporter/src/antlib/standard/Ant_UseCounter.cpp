/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_USECOUNTER_H
#   include "Ant_UseCounter.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_UseCounter::Ant_UseCounter()
{
    use_count = 0;
#if defined(_WIN32) || defined(_WIN32_WCE)
    ::InitializeCriticalSection(&cs);
#else
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
#endif
}

Ant_UseCounter::~Ant_UseCounter()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    ::DeleteCriticalSection(&cs);
#else
    pthread_mutexattr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
#endif
}

BOOL Ant_UseCounter::lock(LONG msec)
{
    if(msec != static_cast<LONG>(INFINITE)) {
        INT wait_count = msec/50;
        for(INT i = 0; i < wait_count; i++){
#if defined(_WIN32) || defined(_WIN32_WCE)
            if(::TryEnterCriticalSection(&cs))
#else
            if(0 == pthread_mutex_trylock(&mutex))
#endif
            {
                if(use_count <= 0) {
                    use_count = 0;
                    return TRUE;
                }
#if defined(_WIN32) || defined(_WIN32_WCE)
                ::LeaveCriticalSection(&cs);
#else
                pthread_mutex_unlock(&mutex);
#endif
            }
#if defined(_WIN32) || defined(_WIN32_WCE)
            Sleep(50);
#endif
        }
    } else {
        while(1){
#if defined(_WIN32) || defined(_WIN32_WCE)
            if(::TryEnterCriticalSection(&cs))
#else
            if(0 == pthread_mutex_trylock(&mutex))
#endif
            {
                if(use_count <= 0) {
                    use_count = 0;
                    return TRUE;
                }
#if defined(_WIN32) || defined(_WIN32_WCE)
                ::LeaveCriticalSection(&cs);
#else
                pthread_mutex_unlock(&mutex);
#endif
            }
#if defined(_WIN32) || defined(_WIN32_WCE)
            Sleep(50);
#endif
        }
    }
    return FALSE;
}

VOID Ant_UseCounter::unlock()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    ::LeaveCriticalSection(&cs);
#else
    pthread_mutex_unlock(&mutex);
#endif
}

VOID Ant_UseCounter::use()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    ::EnterCriticalSection(&cs);
#else
    pthread_mutex_lock(&mutex);
#endif
    use_count ++;
#if defined(_WIN32) || defined(_WIN32_WCE)
    ::LeaveCriticalSection(&cs);
#else
    pthread_mutex_unlock(&mutex);
#endif
    return ;
}

VOID Ant_UseCounter::unuse()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    ::EnterCriticalSection(&cs);
#else
    pthread_mutex_lock(&mutex);
#endif
    use_count --;
#if defined(_WIN32) || defined(_WIN32_WCE)
    ::LeaveCriticalSection(&cs);
#else
    pthread_mutex_unlock(&mutex);
#endif
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
