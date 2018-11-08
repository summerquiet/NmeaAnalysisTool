/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

/*---------------------------------------------------------------------------*/
// Include files

#import <Foundation/Foundation.h>
#import "Ant_ThreadMac.h"
#import "pthread.h"

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Funcation
void pool_alloc(void **pPool)
{
	NSAutoreleasePool *pool  = [[NSAutoreleasePool alloc] init];
    *pPool = static_cast<void*> (pool);
}

void pool_release(void **pPool)
{
	NSAutoreleasePool *pool = static_cast<NSAutoreleasePool*> (*pPool);
	[pool release];
}

bool isMainThread(void)
{
    return [[NSThread currentThread] isMainThread];
}

extern "C" pid_t gettid(void)
{
    uint64_t tid;
    pthread_t curThread = pthread_self();
    pthread_threadid_np(curThread, &tid);
    return tid;
}

void setCurrentThreadPriority(double prio)
{
    [[NSThread currentThread] setThreadPriority:prio];
}

double getCurrentThreadPriority()
{
    return [[NSThread currentThread] threadPriority];
}

void setCurrentThreadName(char *threadname)
{
    @autoreleasepool {
        NSString *tn = [[NSString alloc] initWithUTF8String:threadname];
        [[NSThread currentThread] setName:tn];
        [tn release];
    }
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
