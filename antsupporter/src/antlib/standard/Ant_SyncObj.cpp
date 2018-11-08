/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_SYNCOBJ_H
#   include "Ant_SyncObj.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_SyncObj::Ant_SyncObj()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    ::InitializeCriticalSection(&cs);
#else
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
#endif
}

Ant_SyncObj::~Ant_SyncObj()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    ::DeleteCriticalSection(&cs);
#else
    pthread_mutexattr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
#endif
}

VOID Ant_SyncObj::syncStart()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    ::EnterCriticalSection(&cs);
#else
    pthread_mutex_lock(&mutex);
#endif
}

BOOL Ant_SyncObj::trySyncStart()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    BOOL blRet = ::TryEnterCriticalSection(&cs);
    return blRet?TRUE:FALSE;
#else
    int ret = pthread_mutex_trylock(&mutex);
    return (ret==0)?TRUE:FALSE;
#endif
}

VOID Ant_SyncObj::syncEnd()
{
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
