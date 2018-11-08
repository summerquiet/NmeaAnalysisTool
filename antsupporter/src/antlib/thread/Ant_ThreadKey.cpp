/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_THREADKEY_H
#   include "Ant_ThreadKey.h"
#endif

#if defined(ANT_HAVE_POSIX_API) && ANT_HAVE_POSIX_API

#ifndef ANT_DEBUG_H
#   include "Ant_Debug.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

#ifdef _FOR_ANDROID_
JavaVM*         Ant_ThreadKey::s_pcJVM                  = NULL;
jclass          Ant_ThreadKey::jProcessIntf             = NULL;
jmethodID       Ant_ThreadKey::getThreadPriority_mid    = NULL;
jmethodID       Ant_ThreadKey::setThreadPriority_mid    = NULL;
#endif //_FOR_ANDROID_

Ant_ThreadKey*  Ant_ThreadKey::s_pInstance              = NULL;
Ant_SyncObj     Ant_ThreadKey::s_cSync;

/*---------------------------------------------------------------------------*/
// Functions
Ant_ThreadKey::Ant_ThreadKey()
{
    INT ret = pthread_key_create(&m_key, NULL);
    if (0 != ret)
    {
        ANT_PERROR(("thread key create failed\n"));
    }
#ifdef _FOR_ANDROID_
    ret = pthread_key_create(&m_JNIEnvkey, NULL);
    if (0 != ret)
    {
        ANT_PERROR(("JNIEnv* key create failed\n"));
    }
#endif
}

Ant_ThreadKey::~Ant_ThreadKey()
{

}

Ant_ThreadKey* Ant_ThreadKey::instance()
{
    s_cSync.syncStart();
    if (NULL == s_pInstance)
    {
        s_pInstance = new Ant_ThreadKey();
    }
    s_cSync.syncEnd();
    return s_pInstance;
}

VOID Ant_ThreadKey::destory()
{
    s_cSync.syncStart();
    if (NULL != s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = NULL;
    }
    s_cSync.syncEnd();
}

VOID Ant_ThreadKey::setThread(VOID *pThread)
{
    pthread_setspecific(m_key, pThread);
}

VOID* Ant_ThreadKey::getThread()
{
    return pthread_getspecific(m_key);
}

#ifdef _FOR_ANDROID_
VOID Ant_ThreadKey::initJavaVM(JNIEnv* pcEnv)
{
    pcEnv->GetJavaVM(&s_pcJVM);
    ANT_PRINTF(("[tid: %d]InitJavaVM s_pcJVM is %p.",  gettid(),  s_pcJVM));

    jProcessIntf = pcEnv->FindClass("android/os/Process");
    if (jProcessIntf == NULL) {
        ANT_PERROR("thread key FindClass(android/os/Process) failed\n");
        return;
    }
    jProcessIntf = (jclass)(pcEnv->NewGlobalRef(jProcessIntf));

    getThreadPriority_mid = pcEnv->GetStaticMethodID(jProcessIntf, "getThreadPriority", "(I)I");
    setThreadPriority_mid = pcEnv->GetStaticMethodID(jProcessIntf, "setThreadPriority", "(II)V");
}

VOID Ant_ThreadKey::attachThread()
{
    if (s_pcJVM == NULL) {
        ANT_PRINTF(("[tid: %d]AttachThread s_pcJVM is NULL.",  gettid()));
        return;
    }
    JNIEnv* pcEnv = NULL;
    s_pcJVM->AttachCurrentThread(&pcEnv, NULL);
    pthread_setspecific(m_JNIEnvkey, pcEnv);
    ANT_PRINTF(("[tid: %d]AttachThread Set JNIEnv [%x]",  gettid(), pcEnv));
}

VOID Ant_ThreadKey::detachThread()
{
    if (s_pcJVM == NULL) {
        ANT_PRINTF(("[tid: %d]DetachThread s_pcJVM is NULL.",  gettid()));
        return;
    }
    s_pcJVM->DetachCurrentThread();
    pthread_setspecific(m_JNIEnvkey, NULL);
    ANT_PRINTF(("[tid: %d]DetachThread Set JNIEnv NULL",  gettid()));
}

JNIEnv* Ant_ThreadKey::getCurrentThreadJNIEnv()
{
    if (s_pcJVM == NULL) {
        ANT_PERROR("[tid: %d]getCurrentThreadJNIEnv s_pcJVM is NULL.",  gettid());
        return NULL;
    }
    JNIEnv *pcEnv = (JNIEnv*)pthread_getspecific(Ant_ThreadKey::Instance()->m_JNIEnvkey);
    ANT_PRINTF(("[tid: %d]Get JNIEnv [%x]",  gettid(), pcEnv));
    return pcEnv;
}
#endif

VOID Ant_ThreadKey::setCurrentThreadPriority(INT Priority)
{
#ifdef _FOR_ANDROID_
    JNIEnv *pcEnv = getCurrentThreadJNIEnv();
    if (pcEnv != NULL) {
        pcEnv->CallStaticVoidMethod(jProcessIntf, setThreadPriority_mid, gettid(), Priority);
    }
#endif
}

INT Ant_ThreadKey::getCurrentThreadPriority()
{
#ifdef _FOR_ANDROID_
    JNIEnv *pcEnv = getCurrentThreadJNIEnv();
    if (pcEnv != NULL) {
        INT Priority = pcEnv->CallStaticIntMethod(jProcessIntf, getThreadPriority_mid, gettid());
        return Priority;
    }
#endif
    return 0;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_HAVE_POSIX_API

/*---------------------------------------------------------------------------*/
/* EOF */
