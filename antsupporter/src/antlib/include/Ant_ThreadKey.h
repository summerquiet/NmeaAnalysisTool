/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_THREADKEY_H
#define ANT_THREADKEY_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_OBJECT_H
#   include "Ant_Object.h"
#endif

#if defined(ANT_HAVE_POSIX_API) && ANT_HAVE_POSIX_API

#ifndef ANT_SYNCOBJ_H
#   include "Ant_SyncObj.h"
#endif
#ifdef _FOR_ANDROID_
#   include "jni.h"
#endif

#include <pthread.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

/**
 * @brief helper class interact with thread local objects/attributes.
 *
 * @detail help us setting thread priority/thread local storage, etc.
 */
class ANTLIB_API Ant_ThreadKey : public virtual Ant_Object
{
public:

    static Ant_ThreadKey* instance();
    static VOID destory();

#ifdef _FOR_ANDROID_
    VOID initJavaVM(JNIEnv* pcEnv);
    VOID attachThread();
    VOID detachThread();
    JNIEnv* getCurrentThreadJNIEnv();
#endif
    VOID setCurrentThreadPriority(INT Priority);
    INT  getCurrentThreadPriority();
    VOID* getThread();
    VOID setThread(VOID *pThread);

private:

    // Singalton
    Ant_ThreadKey();
    virtual ~Ant_ThreadKey();

    pthread_key_t m_key;
    static Ant_ThreadKey*   s_pInstance;
    static Ant_SyncObj      s_cSync;

#ifdef _FOR_ANDROID_
    pthread_key_t m_JNIEnvkey;
    static JavaVM* s_pcJVM;
    static jclass jProcessIntf;
    static jmethodID getThreadPriority_mid;
    static jmethodID setThreadPriority_mid;
#endif

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_HAVE_POSIX_API

/*---------------------------------------------------------------------------*/
#endif  // ANT_THREADKEY_H

/*---------------------------------------------------------------------------*/
/* EOF */
