/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_DEBUG_H
#   include "Ant_Debug.h"
#endif
#include <stdio.h>
#include <stdarg.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

const INT       OUTPUT_BUFF_SIZE        = 256;
#if defined(_WIN32) || defined(_WIN32_WCE)
const INT       TERMINATE_CODE_SIZE     = 3;
#else
const INT       TERMINATE_CODE_SIZE     = 0;
#endif
const CHAR      CHAR_CODE_CR            = '\r';
const CHAR      CHAR_CODE_LF            = '\n';
const WCHAR     WCHAR_CODE_CR           = L'\r';
const WCHAR     WCHAR_CODE_LF           = L'\n';

/*---------------------------------------------------------------------------*/
// Functions
VOID Ant_Printf(const CHAR* pszString, ...)
{
    CHAR szLogInfo[OUTPUT_BUFF_SIZE];
    memset(szLogInfo, 0, sizeof(CHAR)*OUTPUT_BUFF_SIZE);

    va_list list;
    va_start(list, pszString);
    INT iRes = _vsnprintf(szLogInfo, (OUTPUT_BUFF_SIZE - TERMINATE_CODE_SIZE), pszString, list);
    va_end(list);

#if defined(_WIN32) || defined(_WIN32_WCE)
    CHAR *pchTerminate = NULL;
    if (-1 != iRes) {
        pchTerminate = (szLogInfo + iRes);
    }
    else {
        pchTerminate = (szLogInfo + OUTPUT_BUFF_SIZE - TERMINATE_CODE_SIZE);
    }
    *pchTerminate++ = CHAR_CODE_CR;
    *pchTerminate++ = CHAR_CODE_LF;
    *pchTerminate = NULL;
#endif

#if defined(COMP_OPT_FOR_MACHINE) && defined(_WIN32_WCE)
    //not implement OutputDebugStringA in SDK
    INT len_conv = ::MultiByteToWideChar(0, 0, szLogInfo, OUTPUT_BUFF_SIZE, NULL, 0);
    if (len_conv > 0) {
        WCHAR* wszLogInfo = new WCHAR[len_conv];
        memset(wszLogInfo, 0, sizeof(WCHAR)*len_conv);
        ::MultiByteToWideChar(0, 0, szLogInfo, OUTPUT_BUFF_SIZE, wszLogInfo, len_conv);
        ::OutputDebugStringW(wszLogInfo);
        delete[] wszLogInfo;
    }
#else
    OutputDebugStringA(szLogInfo);
#endif
}

VOID Ant_Printf(const WCHAR* pszString, ...)
{
    WCHAR szLogInfo[OUTPUT_BUFF_SIZE];
    memset(szLogInfo, 0, sizeof(WCHAR)*OUTPUT_BUFF_SIZE);

    va_list list;
    va_start(list, pszString);
    INT iRes = _vsnwprintf(szLogInfo, (OUTPUT_BUFF_SIZE - TERMINATE_CODE_SIZE), pszString, list);
    va_end(list);

#if defined(_WIN32) || defined(_WIN32_WCE)
    WCHAR    *pchTerminate = NULL;
    if (-1 != iRes) {
        pchTerminate = (szLogInfo + iRes);
    }
    else {
        pchTerminate = (szLogInfo + OUTPUT_BUFF_SIZE - TERMINATE_CODE_SIZE);
    }
    *pchTerminate++ = WCHAR_CODE_CR;
    *pchTerminate++ = WCHAR_CODE_LF;
    *pchTerminate = NULL;
#endif
    OutputDebugStringW(szLogInfo);
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
