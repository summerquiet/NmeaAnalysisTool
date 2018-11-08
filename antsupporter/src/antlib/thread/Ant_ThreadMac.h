/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_THREADMAC_H
#define ANT_THREADMAC_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

// #ifdef _FOR_APPLE_
// #   include <objc/objc.h>
// #endif

/*---------------------------------------------------------------------------*/
// Functions
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _FOR_APPLE_
void pool_alloc(void **pPool);
void pool_release(void **pPool);
bool isMainThread(void);
void setCurrentThreadPriority(double);
double getCurrentThreadPriority();
void setCurrentThreadName( char* threadname);
#endif // _FOR_APPLE_

#ifdef __cplusplus
}
#endif

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_THREADMAC_H

/*---------------------------------------------------------------------------*/
/* EOF */
