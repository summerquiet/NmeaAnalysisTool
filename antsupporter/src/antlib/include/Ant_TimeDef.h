/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_TIMEDEF_H
#define ANT_TIMEDEF_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

#if defined(ANT_HAVE_WIN32_API) && ANT_HAVE_WIN32_API

/*---------------------------------------------------------------------------*/
// include of Win32

typedef SYSTEMTIME ANT_SYSTEMTIME, *ANT_PSYSTEMTIME;

#elif defined(ANT_HAVE_POSIX_API) && ANT_HAVE_POSIX_API

/*---------------------------------------------------------------------------*/
// include of Linux

#include <sys/time.h>
#include <sys/stat.h>

/*---------------------------------------------------------------------------*/
// redefinition of Linux

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} ANT_SYSTEMTIME, *ANT_PSYSTEMTIME, SYSTEMTIME;

/**
 * Common time-related system I/Fs.
 *
 * The following 7 I/Fs are supposed to be with C language linkage. But to
 * avoid conflict with file "NCTimeDefine.h", we can't do it here. As a
 * consequence, they will be C++ only (inaccessible from C code).
 */
void gettimespec(struct timespec *time,  int msec);
void settimespec(struct timespec *time,  int msec);
int  subtimespec(struct timespec *time1, struct timespec *time2);

void ANTLIB_API GetSystemTime(ANT_SYSTEMTIME *systime);
void ANTLIB_API GetLocalTime(ANT_SYSTEMTIME *systime);
DWORD ANTLIB_API GetTickCount();
DWORD ANTLIB_API GetThreadTime();

#endif // ANT_HAVE_POSIX_API

struct Ant_TimeStruct
{
    WORD wYear;             // The year. The valid values for this member are 1601 through 30827.
    WORD wMonth;            // The month. This member can be one of the following values. (1-January ~ 12-December)
    WORD wDayOfWeek;        // The day of the week. This member can be one of the following values. (0-Sunday ~ 6-Saturday)
    WORD wDay;              // The day of the month. The valid values for this member are 1 through 31.
    WORD wHour;             // The hour. The valid values for this member are 0 through 23.
    WORD wMinute;           // The minute. The valid values for this member are 0 through 59.
    WORD wSecond;           // The second. The valid values for this member are 0 through 59.
    WORD wMilliseconds;     // The millisecond. The valid values for this member are 0 through 999.
};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_TIMEDEF_H

/*---------------------------------------------------------------------------*/
/* EOF */
