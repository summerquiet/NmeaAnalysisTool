/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_NEWTYPESDEFINE_H
#define ANT_NEWTYPESDEFINE_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANTLIBAPI_H
#   include "AntlibAPI.h"
#endif

/*---------------------------------------------------------------------------*/
// Global macro define

// Assume POSIX-compatible OS if we're not building for Windows.
#if defined(WIN32) || defined(WIN64)
#   define ANT_HAVE_WIN32_API    1       // Windows API (either 32bit or 64bit)
#   define ANT_HAVE_POSIX_API    0       // POSXI API (POSIX.1-2001)
#else
#   define ANT_HAVE_WIN32_API    0
#   define ANT_HAVE_POSIX_API    1
#endif
#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
#   if defined(ANT_HAVE_WIN32_API) && ANT_HAVE_WIN32_API
#       error "Inconsistent compiler option detected!!!"
#   endif
#endif

/*---------------------------------------------------------------------------*/
// Include files

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <wchar.h>
#include <errno.h>
#ifndef _MSC_VER
#   include <unistd.h>
#   include <utime.h>
#   include <stdint.h>
#endif
#ifdef _FOR_APPLE_
#   include <TargetConditionals.h>
#endif
#if defined(ANT_HAVE_WIN32_API) && ANT_HAVE_WIN32_API
#   include <Windows.h>
#   include <tchar.h>
#endif

#ifdef __cplusplus
#   define ANT_EXTERN_C extern "C"
#else
#   define ANT_EXTERN_C
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

#ifndef CONST
#   define CONST const
#endif
#ifndef VOID
#   define VOID void
#endif

/*---------------------------------------------------------------------------*/
// Compiler depedent types: the type definitions below this section are based
// on the C99 (aka. ISO/IEC 9899:1999), these type definitions make sure we
// work on a stable base.
#ifdef _MSC_VER
typedef __int64             int64_t;
typedef unsigned __int64    uint64_t;
#endif

/*---------------------------------------------------------------------------*/
// Integral types
typedef unsigned char       BYTE;
typedef signed short        SHORT;
typedef unsigned short      USHORT, UINT16, WORD;
typedef signed int          INT;
typedef unsigned int        UINT;
#ifdef COMPT_LOCSHELL_SURPOT
typedef unsigned int        ULONG, DWORD, *PDWORD;
typedef int                 LONG;
#else
typedef unsigned long       ULONG, DWORD, *PDWORD;
typedef long                LONG;
#endif
typedef int64_t             INT64;
typedef int64_t             LONGLONG;
typedef uint64_t            ULONGLONG;
typedef float               FLOAT;
typedef double              DOUBLE;

// Type to represent a boolean value.
#if defined(ANT_HAVE_WIN32_API) && ANT_HAVE_WIN32_API
    typedef int             BOOL;
#elif defined(_FOR_APPLE_) && !defined(OBJC_HIDE_64) && TARGET_OS_IPHONE && __LP64__
    typedef bool            BOOL;
#elif defined(_FOR_APPLE_)
    typedef signed char     BOOL;
#else
    typedef signed char     BOOL;
#endif

/*---------------------------------------------------------------------------*/
// Pointer types
typedef VOID               *PVOID, *LPVOID;
typedef const VOID         *PCVOID, *LPCVOID;
typedef BYTE               *PBYTE;
typedef INT                *PINT;
typedef DWORD              *PDWORD;
typedef VOID               *HANDLE;

/*---------------------------------------------------------------------------*/
// String types
typedef char                CHAR;
typedef unsigned char       UCHAR;
typedef wchar_t             WCHAR;
#if defined(ANT_HAVE_WIN32_API) && ANT_HAVE_WIN32_API
typedef WCHAR               UCHAR16;
#else
typedef UINT16              UCHAR16;
#endif
#if defined(UNICODE) || defined(_UNICODE)
typedef WCHAR               TCHAR;
#else
typedef CHAR                TCHAR;
#endif
typedef CHAR                *PSTR, *LPSTR;
typedef const CHAR          *LPCSTR;
typedef WCHAR               *PWSTR, *LPWSTR;
typedef const WCHAR         *PCWSTR, *LPCWSTR;
typedef TCHAR               *PTSTR, *LPTSTR;
typedef const TCHAR         *PCTSTR, *LPCTSTR;

#ifndef TEXT
#   if defined(UNICODE) || defined(_UNICODE)
#       define TEXT(quote)  L##qutoe
#   else
#       define TEXT(quote)  quote
#   endif
#endif
#ifndef _T
#   define _T(x)            TEXT(x)
#endif

#include "xchar.h"

/*---------------------------------------------------------------------------*/
// Platform dependent types.
// missing pid_t on windows
#if defined(ANT_HAVE_WIN32_API) && ANT_HAVE_WIN32_API
typedef DWORD               pid_t;
#endif

/*---------------------------------------------------------------------------*/
// Global Constants
#ifndef MAXLONG
#   define MAXLONG          0x7fffffffL
#endif
#ifndef MAXWORD
#   define MAXWORD          0xffff
#endif
#ifndef INFINITE
#   define INFINITE         0xffffffffL
#endif

#ifndef FALSE
#   define FALSE            (0)
#endif
#ifndef TRUE
#   define TRUE             (1)
#endif

#ifndef MAX_PATH
#   define MAX_PATH         (260)
#   define _MAX_PATH        MAX_PATH
#endif

/*---------------------------------------------------------------------------*/
// Microsoft string function emulation layer.
#if defined(ANT_HAVE_POSIX_API) && ANT_HAVE_POSIX_API
#   define _vsnprintf       vsnprintf
#   define _vsnwprintf      vswprintf
#   define _strupr          strupr
#   define _wcsupr          wcsupr
#   define _strlwr          strlwr
#   define _wcslwr          wcslwr
#   define _stricmp         strcasecmp
#   define _wcsicmp         wcscasecmp
#   define _wcsnicmp        wcsncasecmp
#   define _strnicmp        strncasecmp
#endif

/*---------------------------------------------------------------------------*/
// Microsoft "tchar.h" compatibility layer.
#if defined(ANT_HAVE_POSIX_API) && ANT_HAVE_POSIX_API
#   if defined(UNICODE) || defined(_UNICODE)
#       define _tcschr      wcschr
#       define _tcslen      wcslen
#       define _tcscpy      wcscpy
#       define _tcsncpy     wcsncpy
#       define _tcschr      wcschr
#       define _tcsrchr     wcsrchr
#       define _tcscmp      wcscmp
#       define _tcsncmp     wcsncmp
#       define _stprintf    swprintf
#       define _sntprintf   swprintf
#       define _vsntprintf  vswprintf
#       define lstrcmp      wcscmp
#       define lstrcmpi     wcscmp
#       define lstrcpy      wcscpy
#   else // UNICODE || _UNICODE
#       define _tscchr      strchr
#       define _tcslen      strlen
#       define _tcscpy      strcpy
#       define _tcsncpy     strncpy
#       define _tcschr      strchr
#       define _tcsrchr     strrchr
#       define _tcscmp      strcmp
#       define _tcsncmp     strncmp
#       define _stprintf    sprintf
#       define _snprintf    snprintf
#       define _sntprintf   snprintf
#       define _vsntprintf  vsnprintf
#       define lstrcmp      strcmp
#       define lstrcmpi     strcmp
#       define lstrcpy      strcpy
#   endif // UNICODE || _UNICODE
#endif

/*---------------------------------------------------------------------------*/
// redefinition of Linux
#if defined(ANT_HAVE_POSIX_API) && ANT_HAVE_POSIX_API
ANT_EXTERN_C ANTLIB_API VOID OutputDebugStringA(LPCSTR lpOutputString);
ANT_EXTERN_C ANTLIB_API VOID OutputDebugStringW(LPCWSTR lpOutputString);
ANT_EXTERN_C ANTLIB_API VOID Sleep(DWORD msec);
#endif

#ifndef _FOR_ANDROID_
ANT_EXTERN_C ANTLIB_API pid_t gettid();       // implementation of Android specific I/F
#endif

#if defined(UNICODE) || defined(_UNICODE)
#    define OutputDebugString  OutputDebugStringW
#else
#    define OutputDebugString  OutputDebugStringA
#endif  // UNICODE || _UNICODE

#ifndef PAI
#   define PAI (3.141592654F)
#endif

#ifndef MAX
#   define MAX(a,b)    (((a)>(b))?(a):(b))
#endif

#ifndef MIN
#   define MIN(a,b)    (((a)<(b))?(a):(b))
#endif

/*---------------------------------------------------------------------------*/
// Ant supporter define
typedef LONG    ANT_ERROR;

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_NEWTYPESDEFINE_H

/*---------------------------------------------------------------------------*/
/* EOF */
