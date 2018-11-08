/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_DEBUG_H
#define ANT_DEBUG_H
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

/*---------------------------------------------------------------------------*/
// Function define

#ifdef _DEBUG
#   define ANT_PRINTF(...)          (Ant_Printf(__VA_ARGS__))
#   include <assert.h>
#   define ANT_ASSERT(expression)   assert(expression)
#else
#   define ANT_PRINTF(...)          ((VOID)0)
#   define ANT_ASSERT(expression)   ((VOID)0)
#endif

extern ANTLIB_API VOID Ant_Printf(const CHAR* pszString, ...);
extern ANTLIB_API VOID Ant_Printf(const WCHAR* pszString, ...);

#if defined(_FOR_ANDROID_) || defined(NUTSHELL)
#   include <android/log.h>
#   define ANT_PERROR(...)      (__android_log_print(ANDROID_LOG_ERROR, "ant", __VA_ARGS__))
#   define ANT_PWARNING(...)    (__android_log_print(ANDROID_LOG_WARN, "ant", __VA_ARGS__))
#else
#   define ANT_PERROR(...)      (Ant_Printf(__VA_ARGS__))
#   define ANT_PWARNING(...)    (Ant_Printf(__VA_ARGS__))
#endif

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_DEBUG_H

/*---------------------------------------------------------------------------*/
/* EOF */
