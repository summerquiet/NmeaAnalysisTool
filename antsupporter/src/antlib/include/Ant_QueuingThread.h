/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef  ANT_QUEUINGTHREAD_H
#define  ANT_QUEUINGTHREAD_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_THREAD_H
#   include "Ant_Thread.h"
#endif
#ifndef ANT_SYNCOBJ_H
#   include "Ant_SyncObj.h"
#endif
#ifndef ANT_LIST_H
#   include "Ant_List.h"
#endif
#ifndef ANT_STRING_H
#   include "Ant_String.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

/**
 * sample code:
 * @code
    class MyReq {
        VOID doAction(Ant_QueuingThread<MyReq>* pThread)
        {
        //do something
        }
    };
    class CSample {
        Ant_QueuingThread<MyReq> m_cThread;
    public:
        CSample() : m_cThread(XTEXT("SAMPLE_TRHEAD_NAME")) {}
        VOID AddReq()
        {
            MyReq* req = new MyReq();
            m_cThread.postRequest(req);
        }
    };
 * @endcode
 *
 * sample code 2:
 * @code
    class CSample {
        class MyThread : public Ant_QueuingThread<MyReq>
        {
        public:
            MyThread() : Ant_QueuingThread<MyReq>(XTEXT("SAMPLE_TRHEAD_NAME")) {}
        };
        MyThread m_cThread;
    public:
        VOID AddReq()
        {
            MyReq* req = new MyReq();
            m_cThread.postRequest(req);
        }
    };
 * @endcode
 */
template <typename REQ>
class Ant_QueuingThread : public Ant_Thread
{
public:

    Ant_QueuingThread(const XCHAR* thread_name)
    : m_strThreadName(thread_name)
    {

    }

    virtual ~Ant_QueuingThread() {m_cReqList.clearData();}

    VOID postRequest(REQ* pcReq);
    VOID pushRequest(REQ *pcReq);

protected:

    virtual VOID run();

    REQ* popRequest();

    Ant_List<REQ>   m_cReqList;
    Ant_SyncObj     m_cSyncObj;
    Ant_String      m_strThreadName;

};

template <typename REQ>
VOID Ant_QueuingThread<REQ>::postRequest(REQ* pcReq)
{
    startThread(m_strThreadName);
    m_cSyncObj.syncStart();
    m_cReqList.append(pcReq);
    m_cSyncObj.syncEnd();
    notify();
}

template <typename REQ>
VOID Ant_QueuingThread<REQ>::pushRequest(REQ* pcReq)
{
    startThread(m_strThreadName);
    m_cSyncObj.syncStart();
    m_cReqList.push(pcReq);
    m_cSyncObj.syncEnd();
    notify();
}

template <typename REQ>
REQ* Ant_QueuingThread<REQ>::popRequest()
{
    m_cSyncObj.syncStart();
    REQ* req = m_cReqList.pop();
    m_cSyncObj.syncEnd();
    return req;
}

template <typename REQ>
VOID Ant_QueuingThread<REQ>::run()
{
    REQ* req = NULL;
    while(!checkQuit()) {
        while(NULL != (req = popRequest())) {
            req->doAction(this);
            delete req;
        }
        wait();
    }
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif // ANT_QUEUINGTHREAD_H

/*---------------------------------------------------------------------------*/
/* EOF */
