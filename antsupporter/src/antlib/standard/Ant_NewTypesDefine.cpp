/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif

/*---------------------------------------------------------------------------*/
// Include files

#if defined(_FOR_ANDROID_)
#   include <android/log.h>
#endif
#ifdef _LINUX
#   include <syscall.h>
#endif

#include <time.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)

void Sleep(DWORD msec)
{
// TEST by summer
#if 0   
	timespec time;
	time.tv_sec = 0;
	time.tv_nsec = msec * 1000000L;
    nanosleep(&time, NULL);
#else
    usleep(msec * 1000);
#endif
}

#ifdef _LINUX
pid_t gettid()
{
    return syscall(__NR_gettid);
}
#endif

void OutputDebugStringA(LPCSTR lpOutputString)
{
#if defined(_FOR_ANDROID_)
    __android_log_print(ANDROID_LOG_DEBUG, "Navi", "%s", lpOutputString);
#else
    printf("%s\n", lpOutputString);
#endif
}

void OutputDebugStringW(LPCWSTR lpOutputString)
{
    printf("%ls\n", lpOutputString);
}


/*---------------------------------------------------------------------------*/
#endif //_LINUX

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
